/*
RocketMod - Rocket Launchers-Only Plugin
Copyright 2004-2024
https://github.com/thecybermind/rocketmod_qmm/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#define _CRT_SECURE_NO_WARNINGS 1

#include <qmmapi.h>

#include "version.h"
#include "game.h"

#include <stdio.h>
#include <string.h>

pluginres_t* g_result = nullptr;
plugininfo_t g_plugininfo = {
	QMM_PIFV_MAJOR,										// plugin interface version major
	QMM_PIFV_MINOR,										// plugin interface version minor
	"RocketMod",										// name of plugin
	ROCKETMOD_QMM_VERSION,								// version of plugin
	"Rockets everywhere!",								// description of plugin
	ROCKETMOD_QMM_BUILDER,								// author of plugin
	"https://github.com/thecybermind/rocketmod_qmm",	// website of plugin
	"ROCKET",											// log tag
};
eng_syscall_t g_syscall = nullptr;
mod_vmMain_t g_vmMain = nullptr;
pluginfuncs_t* g_pluginfuncs = nullptr;
intptr_t g_vmbase = 0;
pluginvars_t* g_pluginvars = nullptr;

gentity_t* g_gents = nullptr;
intptr_t g_numgents = 0;
intptr_t g_gentsize = 0;
gclient_t* g_clients = nullptr;
intptr_t g_clientsize = sizeof(gclient_t);


C_DLLEXPORT void QMM_Query(plugininfo_t** pinfo) {
	QMM_GIVE_PINFO();
}


C_DLLEXPORT int QMM_Attach(eng_syscall_t engfunc, mod_vmMain_t modfunc, pluginres_t* presult, pluginfuncs_t* pluginfuncs, pluginvars_t* pluginvars) {
	QMM_SAVE_VARS();

	// make sure this DLL is loaded only in the right engine
	if (strcmp(QMM_GETGAMEENGINE(), GAME_STR) != 0)
		return 0;

	return 1;
}


C_DLLEXPORT void QMM_Detach() {
}


C_DLLEXPORT intptr_t QMM_vmMain(intptr_t cmd, intptr_t* args) {
	if (cmd == GAME_INIT) {
		// init msg
		QMM_WRITEQMMLOG("RocketMod v" ROCKETMOD_QMM_VERSION " by " ROCKETMOD_QMM_BUILDER " is loaded\n", QMMLOG_INFO);

		// register cvars
		g_syscall(G_CVAR_REGISTER, nullptr, "rocketmod_version", ROCKETMOD_QMM_VERSION, CVAR_ROM | CVAR_SERVERINFO);
		g_syscall(G_CVAR_REGISTER, nullptr, "rocketmod_enabled", "1", CVAR_SERVERINFO | CVAR_ARCHIVE);
		g_syscall(G_CVAR_REGISTER, nullptr, "rocketmod_gauntlet", "1", CVAR_ARCHIVE);
		g_syscall(G_CVAR_REGISTER, nullptr, "rocketmod_ammo", "10", CVAR_ARCHIVE);
	}

	return GAME_vmMain(cmd, args);
}


C_DLLEXPORT intptr_t QMM_syscall(intptr_t cmd, intptr_t* args) {
	// store client data
	if (cmd == G_LOCATE_GAME_DATA) {
		g_gents = (gentity_t*)args[0];
		g_numgents = args[1];
		g_gentsize = args[2];
		g_clients = (gclient_t*)args[3];
		g_clientsize = args[4];
	}

	return GAME_syscall(cmd, args);
}


C_DLLEXPORT intptr_t QMM_vmMain_Post(intptr_t cmd, intptr_t* args) {
	return GAME_vmMain_Post(cmd, args);
}


C_DLLEXPORT intptr_t QMM_syscall_Post(intptr_t cmd, intptr_t* args) {
	return GAME_syscall_Post(cmd, args);
}
