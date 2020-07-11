//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

#include "options.yy.h"

#include <string>
#include <vector>

class CmdArgs
{
public:
	CmdArgs();
	virtual ~CmdArgs();

	bool ParseEnv(const std::string& envVariable = "Z80ASM") { return lexer.ParseEnv(envVariable); }
	bool ParseArgs(int argc, char* argv[]) { return lexer.ParseArgs(argc, argv); }

	// access options from OptionsLexer
	bool IsVerbose() const { return lexer.IsVerbose(); }
	const char* GetEnvPendingOpts() const { return lexer.GetEnvPendingOpts(); }
	int GetCpu() const { return lexer.GetCpu(); }
	const std::string& GetCpuName() const { return lexer.GetCpuName(); }
	bool IsTi83Plus() const { return lexer.IsTi83Plus(); }
	auto cbeginDefines() const { return lexer.cbeginDefines(); }
	auto cendDefines() const { return lexer.cendDefines(); }

private:
	OptionsLexer lexer;
};

extern CmdArgs theCmdArgs;		// singleton
