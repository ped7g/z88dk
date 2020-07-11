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
	CmdArgs();
	virtual ~CmdArgs();
};

extern CmdArgs theCmdArgs;		// singleton
