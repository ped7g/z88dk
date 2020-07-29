//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

#include <string>
#include <utility>
#include <vector>

struct Options {
	bool	verbose;		// true to be verbose
	bool	swapIxIy;		// swap IX and IY
	bool	optimizeSpeed;	// true to optimize for speed
	bool	debugInfo;		// add debug info to map file 
	bool	doMapFile;		// generate map file

	std::vector<std::pair<std::string, int>>
								defines;		// list of -D defines
	std::vector<std::string>	includePath;	// where to search for source files
	std::vector<std::string>	libraryPath;	// where to search for library files
};

// singleton
extern Options theOptions;

void ExitUsage();
void ExitManual();
std::string ExpandEnvironmentVars(std::string str);
