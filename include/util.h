/*
RocketMod - Rocket Launchers-Only Plugin
Copyright 2004-2024
https://github.com/thecybermind/rocketmod_qmm/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#ifndef ROCKETMOD_QMM_UTIL_H
#define ROCKETMOD_QMM_UTIL_H

#include <vector>
#include <string>

// "safe" strncpy that always null-terminates
char* strncpyz(char* dest, const char* src, size_t count);

// break an entstring into a list of string tokens
std::vector<std::string> tokenlist_from_entstring(const char* entstring);

// generate an entstring from a list of string tokens
const char* entstring_from_tokenlist(std::vector<std::string> tokenlist);

#endif // ROCKETMOD_QMM_UTIL_H
