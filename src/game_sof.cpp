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

#include "version.h"
#include "game.h"

#include "util.h"
#include "main.h"


intptr_t GAME_vmMain(intptr_t cmd, intptr_t* args) {
    // this is called to give the mod level-placed entity info at the start of the map
    // unfortunately, stripper may modify weapons if it gets loaded AFTER rocketmod. no real way to fix this
    // also, don't do this if rocketmod_enabled is 0
    if (cmd == GAME_SPAWN_ENTITIES && QMM_GETINTCVAR("rocketmod_enabled")) {
        // change spawn objects:
        // weapon_* -> item_weapon_rocketlauncher
        // ammo_* -> item_ammo_rockets

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
                    token = "item_ammo_rockets";
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
    }

    QMM_RET_IGNORED(0);
}


intptr_t GAME_syscall(intptr_t cmd, intptr_t* args) {
    QMM_RET_IGNORED(0);
}


intptr_t GAME_vmMain_Post(intptr_t cmd, intptr_t* args) {
    QMM_RET_IGNORED(0);
}


intptr_t GAME_syscall_Post(intptr_t cmd, intptr_t* args) {
    static bool pmove = false;
    if (cmd == G_PMOVE) {
        pmove = true;
    }

    // this is called near the end of PutClientInServer (respawn) after dm->clientRespawn(*ent) which sets
    // gamemode-based weapons. this is also called in ClientThink regularly, so we check a few things to
    // verify that this is a respawn event and not during a regular Think
    else if (cmd == G_LINKENTITY) {
        // skip the first linkentity after a pmove, which happens during ClientThink 
        if (pmove) {
            pmove = false;
            QMM_RET_IGNORED(0);
        }

        edict_t* ent = (edict_t*)args[0];

        // make sure ent is not null, it is in use, and has a client
        if (!ent || !ent->inuse || !ent->client)
            QMM_RET_IGNORED(0);

        // make sure ent is a player
        if (strcmp(ent->classname, "player") != 0)
            QMM_RET_IGNORED(0);

        // make sure entity is solid and not dead (this avoids linkentity in ClientDisconnect and player_die)
        if (ent->solid == SOLID_NOT || ent->deadflag)
            QMM_RET_IGNORED(0);

        // make sure client is not a spectator
        if (ent->client->pers.spectator)
            QMM_RET_IGNORED(0);

        // bail if rocketmod isn't enabled (this actually takes more cpu time than the above checks)
        if (!QMM_GETINTCVAR("rocketmod_enabled"))
            QMM_RET_IGNORED(0);

        // remove all weapons and ammo from inventory
        sharedEdict_t sh;
        sh.inv = (inven_c*)ent->client->inv;
        sh.edict = ent;
        sh.inv->setOwner(&sh);
        sh.inv->clearInv(true);
        sh.inv->addWeaponType(SFW_ROCKET);
        sh.inv->addAmmoType(AMMO_ROCKET, (int)QMM_GETINTCVAR("rocketmod_ammo"));
        sh.inv->selectWeapon(SFW_ROCKET);
    }

    QMM_RET_IGNORED(0);
}

#endif // GAME_SOF
