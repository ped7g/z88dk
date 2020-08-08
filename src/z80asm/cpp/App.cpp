//-----------------------------------------------------------------------------
// z80asm restart - program driver
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "App.h"
#include "legacy.h"
#include "z80asm_manual.h"
#include "z80asm_usage.h"
#include "options.yy.h"

#include <cstring>
#include <iostream>
#include <sstream>

#ifndef Z88DK_VERSION
#define Z88DK_VERSION "build " __DATE__
#endif

static const char copyrightMsg[]{
	"Z80 Module Assembler " Z88DK_VERSION "\n"
	"(c) InterLogic 1993-2009, Paulo Custodio 2011-2020"
};


// sigleton
App app;

App::App() {
	optionsLexer = new OptionsLexer;
}

App::App(const App & rhs) {
	optionsLexer = new OptionsLexer;
	files = rhs.files;
}

App::~App() {
	delete optionsLexer;
}

App & App::operator=(const App & rhs) {
	optionsLexer = new OptionsLexer;
	files = rhs.files;
	return *this;
}

bool App::ParseEnv(const std::string & envVariable)
{
	using namespace std;

	ClearEnvPendingOptions();	
	const char *env = getenv(envVariable.c_str());
	if (env) {
		stringstream iss{ string(env) };
		string arg;

		while (iss >> arg) {
			arg = App::ExpandEnvironmentVars(arg);
			optionsLexer->in(arg);
			if (!optionsLexer->lex()) {					// TODO: error handling
				AppendEnvPendingOptions(arg.c_str());	// pass options not parsed to the C code
			}
		}
	}
	return true;
}

bool App::ParseArgs(int argc, char * argv[]) {
	using namespace std;

	// if no arguments, just show usage and exit
	if (argc == 1)
		ExitUsage();

	// parse options
	for (int i = 1; i < argc; ++i) {
		string arg = App::ExpandEnvironmentVars(argv[i]);
		optionsLexer->in(arg);
		if (optionsLexer->lex())
			argv[i][0] = '\0';		// cancel this argument for next pass
		else {						// TODO: error handling
		}
	}

	// add CPU defines
	for (auto define = app.options.cpu.GetDefinesBegin();
		define != app.options.cpu.GetDefinesEnd();
		++define) {
		options.defines.push_back({ *define, 1 });
	}

	// add ARCH defines
	for (auto define = app.options.arch.GetDefinesBegin();
		define != app.options.arch.GetDefinesEnd();
		++define) {
		options.defines.push_back({ define->first, define->second });
	}

	return true;
}

void App::ExitUsage() {
	using namespace std;

	cout << copyrightMsg << endl
		<< endl << z80asm_usage;
	exit(EXIT_SUCCESS);
}

void App::ExitManual() {
	using namespace std;

	cout << z80asm_manual;
	exit(EXIT_SUCCESS);
}

std::string App::ExpandEnvironmentVars(std::string str)
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
