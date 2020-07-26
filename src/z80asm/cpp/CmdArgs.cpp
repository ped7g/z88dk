//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

//#include "legacy.h"

#include "CmdArgs.h"

#include "Arch.h"
#include "Cpu.h"
#include "Options.h"

#include <iostream>

CmdArgs theCmdArgs;

bool CmdArgs::ParseEnv(const std::string & envVariable)
{
	using namespace std;

	envPendingOpts.clear();

	const char *opts = getenv(envVariable.c_str());
	if (!opts)
		return true;
	stringstream iss{ string(opts) };
	string arg;

	while (iss >> arg) {
		in(arg);
		if (!lex()) {						// TODO: error handling
			envPendingOpts += arg + " ";	// pass options not parsed to the C code
		}
	}

	return true;
}

bool CmdArgs::ParseArgs(int argc, char * argv[])
{
	using namespace std;

	// if no arguments, just show usage and exit
	if (argc == 1) 
		ExitUsage();

	// parse options
	for (int i = 1; i < argc; ++i) {
		in(argv[i]);
		if (lex())
			argv[i][0] = '\0';		// cancel this argument for next pass
		else {						// TODO: error handling
		}
	}

	// add CPU defines
	for (auto define = theCpu.GetDefinesBegin(); 
		define != theCpu.GetDefinesEnd(); 
		++define) {
		theOptions.defines.push_back({ *define, 1 });
	}

	// add ARCH defines
	for (auto define = theArch.GetDefinesBegin();
		define != theArch.GetDefinesEnd();
		++define) {
		theOptions.defines.push_back({ define->first, define->second });
	}

	return true;
}
