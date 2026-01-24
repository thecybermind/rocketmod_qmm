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

#if defined(GAME_Q3A)
    #include <q3a/game/q_shared.h>
    #include <q3a/game/g_local.h>
    #define GAME_STR "Q3A"
#elif defined(GAME_QUAKE2)
    #include <quake2/game/g_local.h>
    #include <game_quake2.h>
    #define GAME_STR "QUAKE2"
#elif defined(GAME_Q2R)
    #include <q2r/rerelease/g_local.h>
    #include <game_q2r.h>
    #define GAME_STR "Q2R"
#else
    #error Only supported in Quake 2 and Quake 3!
#endif

intptr_t GAME_vmMain(intptr_t cmd, intptr_t* args);
intptr_t GAME_syscall(intptr_t cmd, intptr_t* args);
intptr_t GAME_vmMain_Post(intptr_t cmd, intptr_t* args);
intptr_t GAME_syscall_Post(intptr_t cmd, intptr_t* args);

#endif // ROCKETMOD_QMM_GAME_H
