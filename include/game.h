/*
RocketMod - Rocket Launchers-Only Plugin
Copyright 2004-2025
https://github.com/thecybermind/rocketmod_qmm/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#ifndef __ROCKETMOD_QMM_GAME_H__
#define __ROCKETMOD_QMM_GAME_H__

#if defined(GAME_Q3A)
    #include <q3a/game/q_shared.h>
    #include <q3a/game/g_local.h>
#elif defined(GAME_QUAKE2)
    #include <quake2/game/g_local.h>
    #include <game_quake2.h>
#elif defined(GAME_Q2R)
    #include <q2r/rerelease/g_local.h>
    #include <game_q2r.h>
#else
    #error Only supported in Quake 2 and Quake 3!
#endif

#endif // __ROCKETMOD_QMM_GAME_H__
