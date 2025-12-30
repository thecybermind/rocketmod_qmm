/*
RocketMod - Rocket Launchers-Only Plugin
Copyright 2004-2024
https://github.com/thecybermind/rocketmod_qmm/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>


// "safe" strncpy that always null-terminates
char* strncpyz(char* dest, const char* src, size_t count) {
	char* ret = strncpy(dest, src, count);
	dest[count - 1] = '\0';
	return ret;
}


// break an entstring into a list of string tokens
std::vector<std::string> tokenlist_from_entstring(const char* entstring) {
	std::vector<std::string> tokenlist;
	std::string build;
	bool buildstr = false;

	for (const char* c = entstring; c && *c; c++) {
		// end if null (shouldn't happen)
		if (!*c)
			break;
		// skip whitespace outside strings
		else if (std::isspace(*c) && !buildstr)
			continue;
		// handle opening braces
		else if (*c == '{')
			tokenlist.push_back("{");
		// handle closing braces
		else if (*c == '}')
			tokenlist.push_back("}");
		// handle quote, start of a key or value
		else if (*c == '"' && !buildstr) {
			build.clear();
			buildstr = true;
		}
		// handle quote, end of a key or value
		else if (*c == '"' && buildstr) {
			tokenlist.push_back(build);
			build.clear();
			buildstr = false;
		}
		// all other chars, add to build string
		else
			build.push_back(*c);
	}

	return tokenlist;
}


// generate an entstring from a list of string tokens
const char* entstring_from_tokenlist(std::vector<std::string> tokenlist) {
	static std::string entstring;
	entstring = "";

	auto iter = tokenlist.begin();
	if (*iter != "{")
		return nullptr;

	while (iter != tokenlist.end()) {
		if (*iter == "{" || *iter == "}") {
			entstring += (*iter + "\n");
		}
		else {
			entstring += ("\"" + *iter + "\" ");
			iter++;
			entstring += ("\"" + *iter + "\"\n");
		}
		iter++;
	}

	return entstring.c_str();
}
