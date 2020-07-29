//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "Options.h"
#include "z80asm_manual.h"
#include "z80asm_usage.h"

#include <iostream>
#include <cstring>

#ifndef Z88DK_VERSION
#define Z88DK_VERSION "build " __DATE__
#endif

static const char copyrightMsg[]{
	"Z80 Module Assembler " Z88DK_VERSION "\n"
	"(c) InterLogic 1993-2009, Paulo Custodio 2011-2020"
};

Options theOptions;

void ExitUsage() 
{
	using namespace std;

	cout << copyrightMsg << endl
		<< endl << z80asm_usage;
	exit(EXIT_SUCCESS);
}

void ExitManual() 
{
	using namespace std;
	cout << z80asm_manual;
	exit(EXIT_SUCCESS);
}

std::string ExpandEnvironmentVars(std::string str)
{
	using namespace std;

	size_t pos1 = str.find("${");
	while (pos1 != string::npos) {
		size_t pos2 = str.find("}", pos1 + 2);
		if (pos2 != string::npos) {		// found ${ENVVAR}
			string var = str.substr(pos1 + 2, pos2 - pos1 - 2);
			const char *value = getenv(var.c_str());
			if (value == nullptr)
				value = "";
			str = str.substr(0, pos1) + value + str.substr(pos2 + 1);
			pos1 += strlen(value);
		}
		else
			pos1 += 2;

		// search next
		pos1 = str.find("${", pos1);
	}

	return str;
}
