/*
RocketMod - Rocket Launchers-Only Plugin
Copyright 2004-2025
https://github.com/thecybermind/rocketmod_qmm/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#ifndef __ROCKETMOD_QMM_VERSION_H__
#define __ROCKETMOD_QMM_VERSION_H__

#define STRINGIFY(x) STRINGIFY2(x)
#define STRINGIFY2(x) #x

#define ROCKETMOD_QMM_VERSION_MAJOR		2
#define ROCKETMOD_QMM_VERSION_MINOR		4
#define ROCKETMOD_QMM_VERSION_REV		2

#define ROCKETMOD_QMM_VERSION			STRINGIFY(ROCKETMOD_QMM_VERSION_MAJOR) "." STRINGIFY(ROCKETMOD_QMM_VERSION_MINOR) "." STRINGIFY(ROCKETMOD_QMM_VERSION_REV)

#if defined(_WIN32)
#define ROCKETMOD_QMM_OS        		"Windows"
#ifdef _WIN64
#define ROCKETMOD_QMM_ARCH      		"x86_64"
#else
#define ROCKETMOD_QMM_ARCH      		"x86"
#endif
#elif defined(__linux__)
#define ROCKETMOD_QMM_OS        		"Linux"
#ifdef __LP64__
#define ROCKETMOD_QMM_ARCH      		"x86_64"
#else
#define ROCKETMOD_QMM_ARCH      		"x86"
#endif
#endif

#define ROCKETMOD_QMM_VERSION_DWORD		ROCKETMOD_QMM_VERSION_MAJOR , ROCKETMOD_QMM_VERSION_MINOR , ROCKETMOD_QMM_VERSION_REV , 0
#define ROCKETMOD_QMM_COMPILE			__TIME__ " " __DATE__
#define ROCKETMOD_QMM_BUILDER			"Kevin Masterson"

#endif // __ROCKETMOD_QMM_VERSION_H__
