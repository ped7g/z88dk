//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

#include "options.yy.h"

#include <string>
#include <vector>

class CmdArgs : public OptionsLexer
{
public:
	bool ParseEnv(const std::string& envVariable = "Z80ASM");	// parse options from environment
	bool ParseArgs(int argc, char* argv[]);						// parse options from ARGV
	const char* GetEnvPendingOpts() const { return envPendingOpts.c_str(); }

private:
	std::string envPendingOpts;	// options from environment to parse by the C code
								// TODO: to remove
};

// singleton
extern CmdArgs theCmdArgs;		
