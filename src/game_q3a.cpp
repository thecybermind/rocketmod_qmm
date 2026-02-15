/*
RocketMod - Rocket Launchers-Only Plugin
Copyright 2004-2024
https://github.com/thecybermind/rocketmod_qmm/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#if defined(GAME_Q3A)

#include <qmmapi.h>

#include "version.h"
#include "game.h"

#include "util.h"
#include "main.h"


intptr_t GAME_vmMain(intptr_t cmd, intptr_t* args) {
    QMM_RET_IGNORED(0);
}


intptr_t GAME_syscall(intptr_t cmd, intptr_t* args) {
	if (cmd == G_GET_USERCMD) {
		// this is a good place to hook when a player respawns.
		// weird, i know, but if you look through ClientSpawn you'll
		// see this is called after the starting machine gun is set

		gclient_t* client = CLIENT_FROM_NUM(args[0]);

		// if the user just respawned, and he has a machine gun, we need to
		// remove it and give him a rocket launcher
		if (((client->ps.pm_flags & PMF_RESPAWNED) == PMF_RESPAWNED)
			&& ((client->ps.stats[STAT_WEAPONS] & (1 << WP_MACHINEGUN)) == (1 << WP_MACHINEGUN))
			&& QMM_GETINTCVAR("rocketmod_enabled")	// moved this here to run it less
			) {

			// give user rocket launcher and gauntlet only
			client->ps.stats[STAT_WEAPONS] = 1 << WP_ROCKET_LAUNCHER;

			if (QMM_GETINTCVAR("rocketmod_gauntlet"))
				client->ps.stats[STAT_WEAPONS] |= 1 << WP_GAUNTLET;

			// give rockets and clear machinegun ammo
			client->ps.ammo[WP_ROCKET_LAUNCHER] = (int)QMM_GETINTCVAR("rocketmod_ammo");
			if (client->ps.ammo[WP_ROCKET_LAUNCHER] <= 0)
				client->ps.ammo[WP_ROCKET_LAUNCHER] = 10;
			client->ps.ammo[WP_MACHINEGUN] = 0;
			client->ps.ammo[WP_GAUNTLET] = -1;
			client->ps.ammo[WP_GRAPPLING_HOOK] = -1;

			//  set rocket launcher as the default weapon and set to ready
			client->ps.weapon = WP_ROCKET_LAUNCHER;
			client->ps.weaponstate = WEAPON_READY;
		}
	}
	
	QMM_RET_IGNORED(0);
}


intptr_t GAME_vmMain_Post(intptr_t cmd, intptr_t* args) {
    QMM_RET_IGNORED(0);
}


intptr_t GAME_syscall_Post(intptr_t cmd, intptr_t* args) {
	// the next token is the entity's classname
	static bool is_classname = false;

	// this is called to get level-placed entity info at the start of the map
	// moved to syscall_Post so that QMM or the engine has already written the entity token into buf
	// also, don't do this if rocketmod_enabled is 0
	if (cmd == G_GET_ENTITY_TOKEN && QMM_GETINTCVAR("rocketmod_enabled")) {
		// change spawn objects:
		// weapon_* -> weapon_rocketlauncher
		// ammo_* -> ammo_rockets

		char* buf = (char*)args[0];
		intptr_t buflen = args[1];

		// if this is the value string for a "classname" key, check it
		if (is_classname) {
			is_classname = false;

			// if its a weapon entity, make it a rocket launcher
			if (!strncmp(buf, "weapon_", 7)) {
				strncpyz(buf, "weapon_rocketlauncher", buflen);
			}
			// if its an ammo entity, make it a rocket ammo pack
			else if (!strncmp(buf, "ammo_", 5)) {
				strncpyz(buf, "ammo_rockets", buflen);
			}
		}
		// if this token is "classname", then the next token is the actual class name
		else if (!strcmp(buf, "classname")) {
			is_classname = true;
		}
	}
	
	QMM_RET_IGNORED(0);
}

#endif // GAME_Q3A
