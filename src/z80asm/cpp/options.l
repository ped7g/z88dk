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
}


%{
#include "legacy.h"
#include "App.h"

#include <string>
#include <utility>
#include <vector>

#include <cassert>
#include <climits>
#include <cstring>
#include <cstdlib>

static const char* SkipEqual(const char* text);
static int ParseNum(const std::string& text);
static void OptionDefine(const std::string& text);
static void OptionFiller(const std::string& text);
%}

%option freespace dotall

ident		[a-zA-Z_] [a-zA-Z0-9_]*
num10		[0-9]+
num16		0 [xX] [0-9a-fA-F]+  |  \$ [0-9a-fA-F]+  |  [0-9] [0-9a-fA-F]* [hH]
num			{num16} | {num10}


%%

-\? \z | -h \z 	{ App::ExitManual(); }
-v \z 			{ app.options.verbose = true; return true; }
-m \z			{ app.options.mapfile = true; return true; }
-s \z			{ app.options.symtable = true; return true; }
-l \z			{ app.options.listfile = true; return true; }
-g \z			{ app.options.globaldef = true; return true; }
-b \z			{ app.options.makeBinary = true; return true; }
-d \z			{ app.options.update = true; return true; }
-R \z			{ app.options.relocatable = true; return true; }
-reloc-info \z	{ app.options.relocInfo = true; return true; }
-split-bin \z	{ app.options.splitBinary = true; return true; }

-m =? z80   \z	{ app.options.cpu.Init(Cpu::Type::Z80); return true; }
-m =? z80n  \z	{ app.options.cpu.Init(Cpu::Type::Z80N); return true; }
-m =? z180  \z	{ app.options.cpu.Init(Cpu::Type::Z180); return true; }
-m =? r2k   \z	{ app.options.cpu.Init(Cpu::Type::R2K); return true; }
-m =? r3k   \z	{ app.options.cpu.Init(Cpu::Type::R3K); return true; }
-m =? 8080  \z	{ app.options.cpu.Init(Cpu::Type::I8080); return true; }
-m =? 8085  \z	{ app.options.cpu.Init(Cpu::Type::I8085); return true; }
-m =? gbz80 \z	{ app.options.cpu.Init(Cpu::Type::GBZ80); return true; }
-m =? ti83  \z	{ app.options.cpu.Init(Cpu::Type::Z80); 
				  app.options.arch.Init(Arch::Type::TI83); 
				  return true; }
-m =? ti83plus \z { 
				  app.options.cpu.Init(Cpu::Type::Z80);
				  app.options.arch.Init(Arch::Type::TI83PLUS);
				  return true; }
-IXIY \z		{ app.options.swapIxIy = true; return true; }
-opt-speed	\z	{ app.options.optimizeSpeed = true; return true; }
-debug \z		{ app.options.SetDebug(); return true; }
-I =? .+ \z		{ app.options.includePath.push_back(SkipEqual(text() + 2));  return true; }

-L =? .+ \z		{ app.options.libraryPath.push_back(SkipEqual(text() + 2));  return true; }
-x =? .+ \z		{ app.options.outputLibrary = SkipEqual(text() + 2); return true; }
-l =? .+ \z		{ app.options.libraries.push_back(SkipEqual(text() + 2)); return true; }

-O =? .+ \z		{ app.options.outputDirectory = SkipEqual(text() + 2); return true; }
-o =? .+ \z		{ app.options.outputFile = SkipEqual(text() + 2); return true; }

-D =? {ident} ( = {num} )? \z	{ 
				  OptionDefine(SkipEqual(text() + 2)); return true; }

\+ zx   \z		{ app.options.SetAppmake(Appmake::ZX); return true; }
\+ zx81 \z		{ app.options.SetAppmake(Appmake::ZX81); return true; }

-r =? {num} \z	{ set_origin_option(ParseNum(SkipEqual(text() + 2))); return true; }
-f =? {num} \z	{ OptionFiller(SkipEqual(text() + 2)); return true; }

-- \z			{ return true; }
\z				{ return true; }

[^-+] .* \z		{ process_file(text()); return true; }

.				{ return false; }

%%

static const char* SkipEqual(const char* text) {
	if (*text == '=')
		text++;
	return text;
}

// parse NUM
static int ParseNum(const std::string& text) {
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

static void OptionDefine(const std::string& text) {
	using namespace std;

	auto p = text.find('=');
	if (p == string::npos) {		// -Dvar
		app.options.defines.push_back({ text, 1});
	}
	else {							// -Dvar=value
		string name = text.substr(0, p);
		int num = ParseNum(text.c_str() + p + 1);
		assert(num >= 0);

		app.options.defines.push_back({ name, num});
	}
}

static void OptionFiller(const std::string& text) {
	int value = ParseNum(text);
	if (value < 0 || value > 0xFF)
		error_invalid_filler_option(text.c_str());
	else
		app.options.filler = value;
}
