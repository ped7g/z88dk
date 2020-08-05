//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

%top{
	// silence warnings from RE-flex
	#ifdef _MSC_VER
	#pragma warning(disable:4102)
	#pragma warning(disable:4800)
	#else
	#ifdef __GNUC__
	//#pragma GCC   diagnostic ignored "-Wignored-qualifiers"
	#else
	#ifdef __clang__
	//#pragma clang diagnostic ignored "-Wignored-qualifiers"
	#endif
	#endif
	#endif

	#include "Arch.h"
	#include "Cpu.h"
	#include "Options.h"

	#include <string>
	#include <utility>
	#include <vector>

	#include <cassert>
	#include <climits>
	#include <cstring>
	#include <cstdlib>
}

%{
	static int parseNum(const std::string& text);
	static void optionDefine(const std::string& text);
	static void optionInludePath(const std::string& text);
	static void optionLibraryPath(const std::string& text);
%}

%option freespace dotall

ident		[a-zA-Z_] [a-zA-Z0-9_]*
num10		[0-9]+
num16		0 [xX] [0-9a-fA-F]+  |  \$ [0-9a-fA-F]+  |  [0-9] [0-9a-fA-F]* [hH]
num			{num16} | {num10}


%%

-v \z 			{ theOptions.verbose = true; return true; }
-\? \z | -h \z 	{ ExitManual(); }

-mz80   \z		{ theCpu.Init(Cpu::Type::Z80); return true; }
-mz80n  \z		{ theCpu.Init(Cpu::Type::Z80N); return true; }
-mz180  \z 	 	{ theCpu.Init(Cpu::Type::Z180); return true; }
-mr2k   \z 		{ theCpu.Init(Cpu::Type::R2K); return true; }
-mr3k   \z		{ theCpu.Init(Cpu::Type::R3K); return true; }
-m8080  \z		{ theCpu.Init(Cpu::Type::I8080); return true; }
-m8085  \z 		{ theCpu.Init(Cpu::Type::I8085); return true; }
-mgbz80 \z 		{ theCpu.Init(Cpu::Type::GBZ80); return true; }
-mti83  \z 		{ theCpu.Init(Cpu::Type::Z80); 
				  theArch.Init(Arch::Type::TI83); 
				  return true; }
-mti83plus \z 	{ theCpu.Init(Cpu::Type::Z80);
				  theArch.Init(Arch::Type::TI83PLUS);
				  return true; }
-IXIY  \z | \
--IXIY \z		{ theOptions.swapIxIy = true; return true; /* TODO: remove --IXIY after master is merged */ }
-opt-speed	\z	{ theOptions.optimizeSpeed = true; return true; }
-debug \z		{ theOptions.debugInfo = true; 
				  theOptions.doMapFile = true; 
				  return true; }
-m \z			{ theOptions.doMapFile = true; return true; }
-I .+ \z		{ optionInludePath(text() + 2);  return true; }
-L .+ \z		{ optionLibraryPath(text() + 2);  return true; }
-D {ident} ( = {num} )? \z	{ 
				  optionDefine(text() + 2); return true; }

.				{ return false; }

%%

// parse NUM
static int parseNum(const std::string& text) {
	char *end = nullptr;
	const char *p = text.c_str();
	long lval;
	int radix;
	char suffix = '\0';
	
	if (p[0] == '\0') {		// empty
		return -1;
	}
	else if (p[0] == '$') {
		p++;
		radix = 16;
	}
	else if (p[0] == '0' && tolower(p[1]) == 'x') {
		p += 2;
		radix = 16;
	}
	else if (isdigit(p[0]) && tolower(p[strlen(p)-1]) == 'h') {
		suffix = p[strlen(p) - 1];
		radix = 16;
	}
	else {
		radix = 10;
	}

	lval = strtol(p, &end, radix);
	if (end == nullptr || *end != suffix || errno == ERANGE || lval < 0 || lval > INT_MAX)
		return -1;
	else
		return static_cast<int>(lval);
}

static void optionDefine(const std::string& text) {
	using namespace std;

	auto p = text.find('=');
	if (p == string::npos) {		// -Dvar
		theOptions.defines.push_back({ text, 1});
	}
	else {							// -Dvar=value
		string name = text.substr(0, p);
		int num = parseNum(text.c_str() + p + 1);
		assert(num >= 0);

		theOptions.defines.push_back({ name, num});
	}
}

static void optionInludePath(const std::string& text) {
	theOptions.includePath.push_back(text);
}

static void optionLibraryPath(const std::string& text) {
	theOptions.libraryPath.push_back(text);
}
