//-----------------------------------------------------------------------------
// z80asm restart - program driver
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

#include "Options.h"

#include <vector>
#include <string>

class OptionsLexer;		// cannot include generated options.yy.h

class App {
public:
	App();
	App(const App& rhs);		// rule of 3
	virtual ~App();
	App& operator=(const App& rhs);

	bool ParseEnv(const std::string & envVariable = "Z80ASM");
	bool ParseArgs(int argc, char* argv[]);

	static void ExitUsage();
	static void ExitManual();
	static std::string ExpandEnvironmentVars(std::string str);
	
	// public access to options
	Options options;

private:
	OptionsLexer*	optionsLexer{ nullptr };
	std::vector<std::string> files;
};

// sigleton
extern App app;
