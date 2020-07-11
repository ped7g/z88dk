//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "CmdArgs.h"
#include "config.h"
#include "legacy.h"
#include "z80asm_usage.h"

#include <iostream>

CmdArgs theCmdArgs;		// singleton

CmdArgs::CmdArgs()
{
}

CmdArgs::~CmdArgs()
{
}
