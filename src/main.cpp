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

#include <string.h>

plugin_info g_plugininfo = {
	QMM_PIFV_MAJOR,										// plugin interface version major
	QMM_PIFV_MINOR,										// plugin interface version minor
	"RocketMod",										// name of plugin
	ROCKETMOD_QMM_VERSION,								// version of plugin
	"Rockets everywhere!",								// description of plugin
	ROCKETMOD_QMM_BUILDER,								// author of plugin
	"https://github.com/thecybermind/rocketmod_qmm",	// website of plugin
	"ROCKET",											// log tag
};
plugin_funcs g_pluginfuncs;
plugin_vars g_pluginvars;

gentity_t* g_gents = nullptr;
intptr_t g_numgents = 0;
intptr_t g_gentsize = 0;
gclient_t* g_clients = nullptr;
intptr_t g_clientsize = 0;


C_DLLEXPORT void QMM_Query(plugin_info** plugininfo, plugin_funcs** pluginfuncs, plugin_vars** pluginvars) {
	QMM_GIVE_PINFO();
}


C_DLLEXPORT int QMM_Attach() {
	// make sure this DLL is loaded only in the right engine
	if (strcmp(QMM_GETGAMEENGINE(), GAME_STR) != 0)
		return 0;

	return 1;
}


C_DLLEXPORT void QMM_Detach() {
}


C_DLLEXPORT intptr_t QMM_vmMain(result_vars* results, intptr_t cmd, intptr_t* args) {
	if (cmd == GAME_INIT) {
		// init msg
		QMM_WRITEQMMLOG(QMMLOG_INFO, "RocketMod v" ROCKETMOD_QMM_VERSION " by " ROCKETMOD_QMM_BUILDER " is loaded\n");

		// register cvars
		QMM_SYSCALL(G_CVAR_REGISTER, nullptr, "rocketmod_version", ROCKETMOD_QMM_VERSION, CVAR_ROM | CVAR_SERVERINFO);
		QMM_SYSCALL(G_CVAR_REGISTER, nullptr, "rocketmod_enabled", "1", CVAR_SERVERINFO | CVAR_ARCHIVE);
		QMM_SYSCALL(G_CVAR_REGISTER, nullptr, "rocketmod_gauntlet", "1", CVAR_ARCHIVE);
		QMM_SYSCALL(G_CVAR_REGISTER, nullptr, "rocketmod_ammo", "10", CVAR_ARCHIVE);
	}

	return GAME_vmMain(results, cmd, args);
}


C_DLLEXPORT intptr_t QMM_syscall(result_vars* results, intptr_t cmd, intptr_t* args) {
	// store client data
	if (cmd == G_LOCATE_GAME_DATA) {
		g_gents = (gentity_t*)args[0];
		g_numgents = args[1];
		g_gentsize = args[2];
		g_clients = (gclient_t*)args[3];
		g_clientsize = args[4];
	}

	return GAME_syscall(results, cmd, args);
}


C_DLLEXPORT intptr_t QMM_vmMain_Post(result_vars* results, intptr_t cmd, intptr_t* args) {
	return GAME_vmMain_Post(results, cmd, args);
}


C_DLLEXPORT intptr_t QMM_syscall_Post(result_vars* results, intptr_t cmd, intptr_t* args) {
	return GAME_syscall_Post(results, cmd, args);
}
