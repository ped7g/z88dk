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
}

%option freespace dotall

ident		[a-zA-Z_] [a-zA-Z0-9_]*

%%

-v        \z | \
--verbose \z	{ theOptions.verbose = true; return true; }

-\?    \z | \
-h     \z | \
--help \z		{ ExitManual(); }

-m    =? z80 \z | \
--cpu =? z80 \z { theCpu.Init(Cpu::Type::Z80); return true; }

-m    =? z80n \z | \
--cpu =? z80n \z { theCpu.Init(Cpu::Type::Z80N); return true; }

-m    =? z180 \z | \
--cpu =? z180 \z { theCpu.Init(Cpu::Type::Z180); return true; }

-m    =? r2k \z | \
--cpu =? r2k \z { theCpu.Init(Cpu::Type::R2K); return true; }

-m    =? r3k \z | \
--cpu =? r3k \z { theCpu.Init(Cpu::Type::R3K); return true; }

-m    =? 8080 \z | \
--cpu =? 8080 \z { theCpu.Init(Cpu::Type::I8080); return true; }

-m    =? 8085 \z | \
--cpu =? 8085 \z { theCpu.Init(Cpu::Type::I8085); return true; }

-m    =? gbz80 \z | \
--cpu =? gbz80 \z { theCpu.Init(Cpu::Type::GBZ80); return true; }

-m    =? ti83 \z | \
--cpu =? ti83 \z {	theCpu.Init(Cpu::Type::Z80); 
					theArch.Init(Arch::Type::TI83); 
					return true; }

-m    =? ti83plus \z | \
--cpu =? ti83plus \z { 
					theCpu.Init(Cpu::Type::Z80);
					theArch.Init(Arch::Type::TI83PLUS);
					return true; }

-IXIY  \z | \
--IXIY \z		{ theOptions.swapIxIy = true; return true; }

--opt=speed	\z	{ theOptions.optimizeSpeed = true; return true; }

--debug \z		{	theOptions.debugInfo = true; 
					theOptions.doMapFile = true; 
					return true; }

-m    \z |
--map \z		{ theOptions.doMapFile = true; return true; }

-I = .+			{ theOptions.includePath.push_back(text() + 3);  return true; }
-I   .+			{ theOptions.includePath.push_back(text() + 2);  return true; }
--inc-path = .+	{ theOptions.includePath.push_back(text() + 11); return true; }
--inc-path   .+	{ theOptions.includePath.push_back(text() + 10); return true; }

-L = .+			{ theOptions.libraryPath.push_back(text() + 3);  return true; }
-L   .+			{ theOptions.libraryPath.push_back(text() + 2);  return true; }
--lib-path = .+	{ theOptions.libraryPath.push_back(text() + 11); return true; }
--lib-path   .+	{ theOptions.libraryPath.push_back(text() + 10); return true; }

-D = 	   {ident} \z	{ theOptions.defines.push_back({ text() + 3, 1}); return true; }
-D		   {ident} \z	{ theOptions.defines.push_back({ text() + 2, 1}); return true; }
--define = {ident} \z	{ theOptions.defines.push_back({ text() + 9, 1}); return true; }
--define   {ident} \z	{ theOptions.defines.push_back({ text() + 8, 1}); return true; }

.				{ return false; }

%%
