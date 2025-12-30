/*
RocketMod - Rocket Launchers-Only Plugin
Copyright 2004-2024
https://github.com/thecybermind/rocketmod_qmm/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#if defined(GAME_QUAKE2)

#include <qmmapi.h>

#include "version.h"
#include "game.h"

#include "util.h"
#include "main.h"

// pointer to start of item list
static gitem_t* s_itemlist = nullptr;


// find item in s_itemlist by classname
gitem_t* FindItemByClassname(const char* classname) {
    if (!s_itemlist)
        return nullptr;

    // skip the first blank entry
    gitem_t* item = s_itemlist + 1;
    while (item->classname) {
        if (!strcmp(item->classname, classname))
            return item;
        item++;
    }

    return nullptr;
}


intptr_t GAME_vmMain(intptr_t cmd, intptr_t* args) {
    // this is called to give the mod level-placed entity info at the start of the map
    // unfortunately, stripper may modify weapons if it gets loaded AFTER rocketmod. no real way to fix this
    // also, don't do this if rocketmod_enabled is 0
    if (cmd == GAME_SPAWN_ENTITIES && QMM_GETINTCVAR(PLID, "rocketmod_enabled")) {
        // change spawn objects:
        // weapon_* -> weapon_rocketlauncher
        // ammo_* -> ammo_rockets

        // if entstring is null or empty, cancel
        const char* entstring = (const char*)(args[1]);
        if (!entstring || !*entstring)
            QMM_RET_IGNORED(0);

        std::vector<std::string> tokenlist = tokenlist_from_entstring(entstring);

        QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "%d tokens loaded from engine\n", tokenlist.size()), QMMLOG_DEBUG);

        // the next token is the entity's classname
        bool is_classname = false;
        int weapons = 0;
        int ammo = 0;

        for (auto& token : tokenlist) {
            // if this is the value string for a "classname" key, check it
            if (is_classname) {
                is_classname = false;

                // if its a weapon entity, make it a rocket launcher
                if (token.substr(0, 7) == "weapon_") {
                    token = "weapon_rocketlauncher";
                    weapons++;
                }
                // if its an ammo entity, make it a rocket ammo pack
                else if (token.substr(0, 5) == "ammo_") {
                    token = "ammo_rockets";
                    ammo++;
                }
            }
            // if this token is "classname", then the next token is the actual class name
            else if (token == "classname") {
                is_classname = true;
            }
        }

        QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "%d weapons set to rocket launcher\n", weapons), QMMLOG_DEBUG);
        QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "%d ammo set to rockets\n", ammo), QMMLOG_DEBUG);

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
    if (cmd == G_LINKENTITY && QMM_GETINTCVAR(PLID, "rocketmod_enabled")) {
        // pointer to rocket launcher item
        static gitem_t* item_rocketlauncher = nullptr;
        // default values based on baseq2 source, but we will find later with FindItemByClassname
        static int item_index_rockets = 21;
        static int item_index_rocketlauncher = 14;

        gentity_t* ent = (gentity_t*)args[0];
        // make sure ent is not null and it is in use
        if (!ent || !ent->inuse)
            QMM_RET_IGNORED(0);

        // make sure ent is a player
        if (strcmp(ent->classname, "player") != 0)
            QMM_RET_IGNORED(0);

        // make sure ent has a valid client pointer
        gclient_t* client = ent->client;
        if (!client)
            QMM_RET_IGNORED(0);

        // most everything after this references the client persistant data, so get a shortcut
        client_persistant_t* pers = &client->pers;

        // make sure client is not a spectator
        if (pers->spectator)
            QMM_RET_IGNORED(0);

        // find the rocketlauncher item based on first spawned player's blaster:
        // the item list starts and ends with empty gitem_t objects, so grab a valid pointer to a blaster,
        // and scan back to the beginning null entry. then use FindItemByClassname to find items in the list
        if (!item_rocketlauncher) {
            gitem_t* item_weapon = pers->weapon;
            if (item_weapon) {
                gitem_t* item = item_weapon;
                // go back to the beginning of the item list
                while (item->classname)
                    item--;
                // save for lookups
                s_itemlist = item;
                // look up items and indexes
                item_rocketlauncher = FindItemByClassname("weapon_rocketlauncher");
                item_index_rocketlauncher = item_rocketlauncher - s_itemlist;
                item_index_rockets = FindItemByClassname("ammo_rockets") - s_itemlist;
            }
        }

        // still couldn't find rocketlauncher item, cancel
        if (!item_rocketlauncher) {
            QMM_RET_IGNORED(0);
        }

        // check for respawn
        // at respawn, ent->s.skinnum is set to a 0-based client index:
        //      ent->s.skinnum = ent - g_edicts - 1;
        // AFTER LinkEntity is called, ent->s.skinnum is changed in ChangeWeapon():
        //      ent->s.skinnum |= ((ent->client->pers.weapon->weapmodel & 0xff) << 8);
        // so if ent->s.skinnum is > 0xff, it's not a respawn event
        if (ent->s.skinnum > 0xff)
            QMM_RET_IGNORED(0);

        // remove current weapon from inventory
        pers->inventory[pers->selected_item] = 0;
        // set current item to hold to rocket launcher
        pers->selected_item = item_index_rocketlauncher;
        // set inventory count for rocket launcher to 1
        pers->inventory[pers->selected_item] = 1;
        // set weapon to rocket launcher gitem_t
        pers->weapon = item_rocketlauncher;
        // set ammo to cvar (cap at max)
        int start_ammo = QMM_GETINTCVAR(PLID, "rocketmod_ammo");
        if (start_ammo > pers->max_rockets)
            start_ammo = pers->max_rockets;
        pers->inventory[item_index_rockets] = start_ammo;
        // set gun model
        client->ps.gunindex = g_syscall(G_MODELINDEX, pers->weapon->view_model);
    }

    QMM_RET_IGNORED(0);
}

#endif // GAME_QUAKE2
