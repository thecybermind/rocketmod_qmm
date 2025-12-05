/*
RocketMod - Rocket Launchers-Only Plugin
Copyright 2004-2024
https://github.com/thecybermind/rocketmod_qmm/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < cybermind@gmail.com >

*/

#define _CRT_SECURE_NO_WARNINGS 1

#include <qmmapi.h>

#include "version.h"
#include "game.h"

#include <stdio.h>
#include <string.h>

pluginres_t* g_result = nullptr;
plugininfo_t g_plugininfo = {
	QMM_PIFV_MAJOR,									 // plugin interface version major
	QMM_PIFV_MINOR,									 // plugin interface version minor
	"RocketMod",									 // name of plugin
	ROCKETMOD_QMM_VERSION,							 // version of plugin
	"Rockets everywhere!",							 // description of plugin
	ROCKETMOD_QMM_BUILDER,							 // author of plugin
	"https://github.com/thecybermind/rocketmod_qmm", // website of plugin
};
eng_syscall_t g_syscall = nullptr;
mod_vmMain_t g_vmMain = nullptr;
pluginfuncs_t* g_pluginfuncs = nullptr;
intptr_t g_vmbase = 0;
pluginvars_t* g_pluginvars = nullptr;

gclient_t* g_clients = nullptr;
intptr_t g_clientsize = sizeof(gclient_t);


// "safe" strncpy that always null-terminates
char* strncpyz(char* dest, const char* src, size_t count) {
	char* ret = strncpy(dest, src, count);
	dest[count - 1] = '\0';
	return ret;
}


C_DLLEXPORT void QMM_Query(plugininfo_t** pinfo) {
	QMM_GIVE_PINFO();
}

C_DLLEXPORT int QMM_Attach(eng_syscall_t engfunc, mod_vmMain_t modfunc, pluginres_t* presult, pluginfuncs_t* pluginfuncs, pluginvars_t* pluginvars) {
	QMM_SAVE_VARS();

	// check game engine and cancel load
	if (strcmp(QMM_GETGAMEENGINE(), "Q3A") != 0) {
		QMM_WRITEQMMLOG("RocketMod is only designed to be run in Quake 3!\n", QMMLOG_INFO, "ROCKETMOD");
		return 0;
	}

	return 1;
}

C_DLLEXPORT void QMM_Detach() {
}

bool s_enabled = false;
C_DLLEXPORT intptr_t QMM_vmMain(intptr_t cmd, intptr_t* args) {
	if (cmd == GAME_INIT) {
		// init msg
		QMM_WRITEQMMLOG("RocketMod v" ROCKETMOD_QMM_VERSION " by " ROCKETMOD_QMM_BUILDER " is loaded\n", QMMLOG_INFO, "ROCKETMOD");

		// register cvars
		g_syscall(G_CVAR_REGISTER, nullptr, "rocketmod_version", ROCKETMOD_QMM_VERSION, CVAR_ROM | CVAR_SERVERINFO);
		g_syscall(G_CVAR_SET, "rocketmod_version", ROCKETMOD_QMM_VERSION);
		g_syscall(G_CVAR_REGISTER, nullptr, "rocketmod_enabled", "1", CVAR_SERVERINFO | CVAR_ARCHIVE);
		g_syscall(G_CVAR_REGISTER, nullptr, "rocketmod_gauntlet", "1", CVAR_ARCHIVE);
		g_syscall(G_CVAR_REGISTER, nullptr, "rocketmod_ammo", "10", CVAR_ARCHIVE);

		// cache this in an int so we don't have to check it every time
		// G_GET_ENTITY_TOKEN comes around. player spawning still checks the cvar
		s_enabled = (bool)QMM_GETINTCVAR("rocketmod_enabled");
	}
	QMM_RET_IGNORED(1);
}

C_DLLEXPORT intptr_t QMM_syscall(intptr_t cmd, intptr_t* args) {
	switch (cmd) {
		// store client data
		case G_LOCATE_GAME_DATA:
			g_clients = (gclient_t*)args[3];
			g_clientsize = args[4];
			break;

		// this is a good place to hook when a player respawns.
		// weird, i know, but if you look through ClientSpawn you'll
		// see this is called after the starting machine gun is set
		// also, don't do this if rocketmod_enabled is 0
		case G_GET_USERCMD:
			if (!QMM_GETINTCVAR("rocketmod_enabled"))
				break;

			gclient_t* client = CLIENT_FROM_NUM(args[0]);

			// if the user just respawned, and he has a machine gun, we need to
			// remove it and give him a rocket launcher
			if (((client->ps.pm_flags & PMF_RESPAWNED) == PMF_RESPAWNED) && ((client->ps.stats[STAT_WEAPONS] & (1 << WP_MACHINEGUN)) == (1 << WP_MACHINEGUN))) {

				// give user rocket launcher and gauntlet only
				client->ps.stats[STAT_WEAPONS] = 1 << WP_ROCKET_LAUNCHER;

				if (QMM_GETINTCVAR("rocketmod_gauntlet"))
					client->ps.stats[STAT_WEAPONS] |= 1 << WP_GAUNTLET;

				// give rockets and clear machinegun ammo
				client->ps.ammo[WP_ROCKET_LAUNCHER] = QMM_GETINTCVAR("rocketmod_ammo");
				if (client->ps.ammo[WP_ROCKET_LAUNCHER] <= 0)
					client->ps.ammo[WP_ROCKET_LAUNCHER] = 10;
				client->ps.ammo[WP_MACHINEGUN] = 0;
				client->ps.ammo[WP_GAUNTLET] = -1;
				client->ps.ammo[WP_GRAPPLING_HOOK] = -1;

				//  set rocket launcher as the default weapon and set to ready
				client->ps.weapon = WP_ROCKET_LAUNCHER;
				client->ps.weaponstate = WEAPON_READY;
			}

			break;
	}

	QMM_RET_IGNORED(1);
}

C_DLLEXPORT intptr_t QMM_vmMain_Post(intptr_t cmd, intptr_t* args) {
	QMM_RET_IGNORED(1);
}

C_DLLEXPORT intptr_t QMM_syscall_Post(intptr_t cmd, intptr_t* args) {
	static bool is_classname = false;
	
	// this is called to get level-placed entity info at the start of the map
	// moved to syscall_Post to not interfere with stripper_qmm
	// also, don't do this if rocketmod_enabled is 0
	if (cmd == G_GET_ENTITY_TOKEN && s_enabled) {
			// change spawn objects:
			// weapon_* -> weapon_rocketlauncher
			// ammo_* -> ammo_rockets

			char* buf = (char*)args[0];
			intptr_t buflen = args[1];

			//if this is the value string for a "classname" key, check it
			if (is_classname) {
				is_classname = false;

				//if its a weapon entity, make it a rocket launcher
				if (!strncmp(buf, "weapon_", 7)) {
					strncpyz(buf, "weapon_rocketlauncher", buflen);
				}
				//if its an ammo entity, make it a rocket ammo pack
				else if (!strncmp(buf, "ammo_", 5)) {
					strncpyz(buf, "ammo_rockets", buflen);
				}
			// if this token is "classname", then the next token is the actual class name
			} else if (!strcmp(buf, "classname")) {
				is_classname = true;
			}
	}

	QMM_RET_IGNORED(1);
}
