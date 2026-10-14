/*
RocketMod - Rocket Launchers-Only Plugin
Copyright 2004-2024
https://github.com/thecybermind/rocketmod_qmm/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#if defined(GAME_SOF)

#include <qmmapi.h>

#include <inttypes.h>
#include <stdint.h>
#include "version.h"
#include "game.h"

#include <sof/player/w_weapons.h>
#include <sof/qcommon/w_types.h>
#include <sof/gamecpp/dm_private.h>
#include <sof/gamecpp/q_sh_interface.h>

#include "vhook.hpp"
#include "util.hpp"
#include "main.h"

static player_export_t* pe_api = nullptr;
static gamemode_ic** pdm = nullptr;

#ifdef _WIN32
#define THISCALL(obj, func, ...) func(obj, 0, ## __VA_ARGS__)
#define FASTCALL __fastcall
#define SELFARG self, void*
#else
#define THISCALL(obj, func, ...) func(obj, ## __VA_ARGS__ )
#define FASTCALL
#define SELFARG self 
#endif

typedef void(FASTCALL* gamemode_ic_clientRespawn)(gamemode_ic* SELFARG, edict_t& ent);
gamemode_ic_clientRespawn clientRespawnOld = nullptr;
void FASTCALL clientRespawnHook(gamemode_ic* SELFARG, edict_t& ent) {
    QMM_WRITEQMMLOG(QMMLOG_INFO, "clientRespawnHook called!\n");

    if (clientRespawnOld) {
        QMM_WRITEQMMLOG(QMMLOG_INFO, "clientRespawnOld called!\n");
        THISCALL(self, clientRespawnOld, ent);
        QMM_WRITEQMMLOG(QMMLOG_INFO, "clientRespawnOld returned!\n");
    }

    if (pe_api && pe_api->isClient && !*pe_api->isClient) {
        ent.client->ps.stats[STAT_WEAPON] = SFW_ROCKET;
        ent.client->ps.stats[STAT_CLIP_AMMO] = 4;
        ent.client->ps.stats[STAT_AMMO] = 10;
        ent.client->ps.stats[STAT_ARMOR] = 100;
    }

    // re-precache all weapons
    // if (pe_api)
    //     pe_api->PrecacheViewWeaponModels(0xFFFFFFFF);

    QMM_WRITEQMMLOG(QMMLOG_INFO, "clientRespawnHook returning!\n");
}

typedef char* (FASTCALL* gamemode_ic_getGameName)(gamemode_ic* SELFARG);
gamemode_ic_getGameName getGameNameOld = nullptr;
char* FASTCALL getGameNameHook(gamemode_ic* SELFARG) {
    QMM_WRITEQMMLOG(QMMLOG_INFO, "getGameNameHook called!\n");

    if (getGameNameOld)
        THISCALL(self, getGameNameOld);

    QMM_WRITEQMMLOG(QMMLOG_INFO, "getGameNameHook returning!\n");

    return (char*)"Test!";
}

intptr_t GAME_vmMain(result_vars* results, intptr_t cmd, intptr_t* args) {
    // this is called to give the mod level-placed entity info at the start of the map
    // unfortunately, stripper may modify weapons if it gets loaded AFTER rocketmod. no real way to fix this
    // also, don't do this if rocketmod_enabled is 0
    if (cmd == GAME_SPAWN_ENTITIES && QMM_GETINTCVAR("rocketmod_enabled")) {
        // change spawn objects:
        // item_weapon_* -> item_weapon_rocketlauncher
        // item_ammo_* -> item_ammo_rocket

        // if entstring is null or empty, cancel
        const char* entstring = (const char*)(args[1]);
        if (!entstring || !*entstring)
            QMM_RET_IGNORED(0);

        std::vector<std::string> tokenlist = tokenlist_from_entstring(entstring);

        // the next token is the entity's classname
        bool is_classname = false;
        int weapons = 0;
        int ammo = 0;

        for (auto& token : tokenlist) {
            // if this is the value string for a "classname" key, check it
            if (is_classname) {
                is_classname = false;

                // if its a weapon entity, make it a rocket launcher
                if (token.substr(0, 12) == "item_weapon_") {
                    token = "item_weapon_rocketlauncher";
                    weapons++;
                }
                // if its an ammo entity, make it a rocket ammo pack
                else if (token.substr(0, 10) == "item_ammo_") {
                    token = "item_ammo_rocket";
                    ammo++;
                }
            }
            // if this token is "classname", then the next token is the actual class name
            else if (token == "classname") {
                is_classname = true;
            }
        }

        entstring = entstring_from_tokenlist(tokenlist);
        args[1] = (intptr_t)entstring;

        QMM_WRITEQMMLOG(QMMLOG_DEBUG, "Map entity modification complete: %d weapons and %d ammo changed.\n", weapons, ammo);
    }

    QMM_RET_IGNORED(0);
}


static VHook pdm_hook;

intptr_t GAME_syscall(result_vars* results, intptr_t cmd, intptr_t* args) {
    // grab dm pointer just after it is assigned and given a game type
    if (cmd == G_SP_REGISTER && !pdm) {
        const char* package = (const char*)args[0];
        if (package && strcmp(package, "dm_generic") == 0)
            pdm = (gamemode_ic**)(package + 0x17800);

        if (pdm && *pdm) {
            pdm_hook.copy_vtable(*pdm);

            clientRespawnOld = (gamemode_ic_clientRespawn)pdm_hook.set_vtable_index(48, (void*)clientRespawnHook);
            if (clientRespawnOld)
                QMM_WRITEQMMLOG(QMMLOG_INFO, "clientRespawn hook installed!\n");
        }
    }
#if 0
    // this is called near the end of PutClientInServer (respawn) after dm->clientRespawn(*ent) which sets
    // gamemode-based weapons. this is also called in ClientThink regularly, so we check a few things to
    // verify that this is a respawn event and not during a regular Think
    else if (0 && cmd == G_LINKENTITY) {
        // we need player api pointer
        if (!pe_api)
            QMM_RET_IGNORED(0);

        edict_t* ent = (edict_t*)args[0];

        // make sure ent is not null, it is in use, has a client, and inventory
        if (!ent || !ent->inuse || !ent->client || !ent->client->inv)
            QMM_RET_IGNORED(0);

        // make sure entity is solid and not dead (this avoids linkentity in ClientDisconnect and player_die)
        if (ent->solid == SOLID_NOT || ent->deadflag)
            QMM_RET_IGNORED(0);

        // make sure client is connected and not a spectator
        if (!ent->client->pers.connected || ent->client->pers.spectator)
            QMM_RET_IGNORED(0);

        // make sure ent is a player
        if (strcmp(ent->classname, "player") != 0)
            QMM_RET_IGNORED(0);

        // bail if rocketmod isn't enabled (this actually takes more cpu time than the above checks)
        if (!QMM_GETINTCVAR("rocketmod_enabled"))
            QMM_RET_IGNORED(0);

        ent->client->restart_count++;

        g_syscall(G_RELIABLE_WRITEBYTE_TOCLIENT, svc_restart_predn, ent->s.number - 1);
        g_syscall(G_RELIABLE_WRITEBYTE_TOCLIENT, ent->client->restart_count, ent->s.number - 1);

        sharedEdict_t sh;
        sh.edict = ent;
        sh.inv = (inven_c*)ent->client->inv;
        sh.inv->owner = &sh;
        int i;
        for (i = 0; i < MAXWEAPONS; i++)
            sh.inv->weapons[i].type = SFW_EMPTYSLOT;
        for (i = 0; i < MAXAMMOS; i++)
            sh.inv->ammo[i] = 0;
        for (i = 0; i < MAXITEMS; i++) {
            sh.inv->items.slots[i].itemType = SFE_EMPTYSLOT;
            sh.inv->items.slots[i].ammo = 0;
        }
        sh.inv->nextCommand = PWC_NOCOMMAND;

        sh.inv->rulesWeaponsUseAmmo = -1;
        memcpy(sh.inv->rulesBestWeaponMode, "unsa", 5);
        sh.inv->rulesSelectWithNoAmmo = -1;
        sh.inv->weapons[9].type = SFW_ROCKET;
        sh.inv->weapons[9].curClip = 0;
        sh.inv->weaponsOwned |= (1 << SFW_ROCKET);

        sh.inv->ammo[AMMO_ROCKET] = (int)QMM_GETINTCVAR("rocketmod_ammo");
        int ammo = sh.inv->ammo[AMMO_ROCKET];

        weapon_c* weaponToFill = &sh.inv->weapons[9];
        if (sh.inv->ammo[AMMO_ROCKET] > 4 - weaponToFill->curClip) {
            sh.inv->ammo[AMMO_ROCKET] -= 4 - weaponToFill->curClip;
            weaponToFill->curClip = 4;
        }
        else {
            weaponToFill->curClip = weaponToFill->curClip + sh.inv->ammo[AMMO_ROCKET];
            sh.inv->ammo[AMMO_ROCKET] = 0;
        }

        // re-precache all weapons
        pe_api->PrecacheViewWeaponModels(0xFFFFFFFF);

        sh.inv->curWeaponID = SFW_ROCKET;
        sh.inv->curWeaponSlot = 9;
        sh.inv->curWeaponType = SFW_ROCKET;
        sh.inv->selectedWeaponSlot = 9;

        if(!pe_api->isClient) {
            ent->client->ps.gunType= sh.inv->curWeaponID;
            ent->client->ps.gunClip=4;
            ent->client->ps.gunAmmo= sh.inv->ammo[AMMO_ROCKET];
        }

        if (!sh.inv->wAnim.inDisguise) {
            sh.inv->canHandleInput = 1;
            sh.inv->wAnim.animType = 0 /*WANIM_NORMAL*/;
        }

        IGhoulInst* gun = ent->client->ps.gun;
        if (gun)
            gun->SetAllPartsOnOff(true);
    }
#endif
    QMM_RET_IGNORED(0);
}


intptr_t GAME_vmMain_Post(result_vars* results, intptr_t cmd, intptr_t* args) {
    QMM_RET_IGNORED(0);
}


intptr_t GAME_syscall_Post(result_vars* results, intptr_t cmd, intptr_t* args) {
    // we need to call some functions on the player api, so we need to steal this return value
    if (cmd == G_SYS_GETPLAYERAPI) {
        pe_api = QMM_VAR_FINAL_RETURN(player_export_t*);
    }

    // handle when player api is unloaded
    else if (cmd == G_SYS_UNLOADPLAYER) {
        pe_api = nullptr;
    }

    QMM_RET_IGNORED(0);
}

#endif // GAME_SOF
