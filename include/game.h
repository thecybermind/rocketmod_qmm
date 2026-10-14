/*
RocketMod - Rocket Launchers-Only Plugin
Copyright 2004-2026
https://github.com/thecybermind/rocketmod_qmm/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#ifndef ROCKETMOD_QMM_GAME_H
#define ROCKETMOD_QMM_GAME_H

#include <qmmapi.h>
#include <cstddef>

#if defined(GAME_Q3A)
    #include <q3a/game/q_shared.h>
    #include <q3a/game/g_local.h>
#elif defined(GAME_QUAKE2)
    #include <quake2/game/g_local.h>
    #include <game_quake2.h>
#elif defined(GAME_Q2R)
    #include <q2r/rerelease/g_local.h>
    #include <game_q2r.h>
#elif defined(GAME_SOF)
    #include <sof/gamecpp/q_shared.h>
    #define GAME_INCLUDE
    #include <sof/gamecpp/g_local.h>
    #undef GAME_INCLUDE
    #include <game_sof.h>
    #error Not supported yet!
#else
    // #error Only supported in Quake 2, Quake 3, and Soldier of Fortune!
    #error Only supported in Quake 2, Quake 3!
#endif

intptr_t GAME_vmMain(result_vars* results, intptr_t cmd, intptr_t* args);
intptr_t GAME_syscall(result_vars* results, intptr_t cmd, intptr_t* args);
intptr_t GAME_vmMain_Post(result_vars* results, intptr_t cmd, intptr_t* args);
intptr_t GAME_syscall_Post(result_vars* results, intptr_t cmd, intptr_t* args);

#endif // ROCKETMOD_QMM_GAME_H
