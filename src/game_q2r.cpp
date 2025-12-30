/*
RocketMod - Rocket Launchers-Only Plugin
Copyright 2004-2024
https://github.com/thecybermind/rocketmod_qmm/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#if defined(GAME_Q2R)

#include <qmmapi.h>

#include "version.h"
#include "game.h"

#include "main.h"


intptr_t GAME_vmMain(intptr_t cmd, intptr_t* args) {
    QMM_RET_IGNORED(0);
}


intptr_t GAME_syscall(intptr_t cmd, intptr_t* args) {
    QMM_RET_IGNORED(0);
}


intptr_t GAME_vmMain_Post(intptr_t cmd, intptr_t* args) {
    QMM_RET_IGNORED(0);
}


intptr_t GAME_syscall_Post(intptr_t cmd, intptr_t* args) {
    QMM_RET_IGNORED(0);
}

#endif // GAME_Q2R
