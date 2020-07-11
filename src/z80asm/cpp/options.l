//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

%top{
	// silence warnings from RE-flex
	#ifdef _MSC_VER
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


	#include "legacy.h"

	#include "config.h"
	#include "z80asm_manual.h"
	#include "z80asm_usage.h"

	#include <iostream>
	#include <string>
	#include <vector>

	#ifndef Z88DK_VERSION
	#define Z88DK_VERSION "build " __DATE__
	#endif
}

%option freespace dotall

%class{
	private:
		static const char copyrightmsg[];

		bool verbose;
		int cpu;					// TODO: replace with class enum
		bool isTi83Plus;			// true for the TI83Plus
		std::string cpuName;		// used to search libraries
		std::string envPendingOpts;	// options from environment to parse by the C code
									// TODO: to remove
		std::vector<std::string> defines;	// list of -D defines

	public:
		bool ParseEnv(const std::string& envVariable);	// parse options from environment
		bool ParseArgs(int argc, char* argv[]);			// parse options from ARGV

		bool IsVerbose() const { return verbose; }
		const char* GetEnvPendingOpts() const { return envPendingOpts.c_str(); }
		int GetCpu() const { return cpu; }
		const std::string& GetCpuName() const { return cpuName; }
		bool IsTi83Plus() const { return isTi83Plus; }
		auto cbeginDefines() const { return defines.cbegin(); }
		auto cendDefines() const { return defines.cend(); }

	private:
		void ShowManual() const;
		void SetCpuZ80();
		void SetCpuZ80n();
		void SetCpuZ180();
		void SetCpuR2k();
		void SetCpuR3k();
		void SetCpu8080();
		void SetCpu8085();
		void SetCpuGbz80();
		void SetCpuTi83();
		void SetCpuTi83Plus();
}

%%

-v        \z | \
--verbose \z	{ verbose = true; return true; }

-h     \z | \
--help \z		{ ShowManual(); return true; }

-m    =? z80 \z | \
--cpu =? z80 \z { SetCpuZ80(); return true; }

-m    =? z80n \z | \
--cpu =? z80n \z { SetCpuZ80n(); return true; }

-m    =? z180 \z | \
--cpu =? z180 \z { SetCpuZ180(); return true; }

-m    =? r2k \z | \
--cpu =? r2k \z { SetCpuR2k(); return true; }

-m    =? r3k \z | \
--cpu =? r3k \z { SetCpuR3k(); return true; }

-m    =? 8080 \z | \
--cpu =? 8080 \z { SetCpu8080(); return true; }

-m    =? 8085 \z | \
--cpu =? 8085 \z { SetCpu8085(); return true; }

-m    =? gbz80 \z | \
--cpu =? gbz80 \z { SetCpuGbz80(); return true; }

-m    =? ti83 \z | \
--cpu =? ti83 \z { SetCpuTi83(); return true; }

-m    =? ti83plus \z | \
--cpu =? ti83plus \z { SetCpuTi83Plus(); return true; }

.				{ return false; }

%%

const char OptionsLexer::copyrightmsg[]{
	"Z80 Module Assembler " Z88DK_VERSION "\n"
		"(c) InterLogic 1993-2009, Paulo Custodio 2011-2020"
};

bool OptionsLexer::ParseEnv(const std::string& envVariable)
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

bool OptionsLexer::ParseArgs(int argc, char* argv[])
{
	using namespace std;

	// if no arguments, just show usage and exit
	if (argc == 1) {
		cout << copyrightmsg << endl << endl
			<< z80asm_usage;
		exit(EXIT_SUCCESS);
	}

	// parse options
	for (int i = 1; i < argc; ++i) {
		in(argv[i]);
		if (lex())
			argv[i][0] = '\0';		// cancel this argument for next pass
		else {						// TODO: error handling
		}
	}

	// if no --cpu option was seen, define default
	if (cpu == 0)
		SetCpuZ80();

	return true;
}

void OptionsLexer::ShowManual() const
{
	using namespace std;
	cout << z80asm_manual;
	exit(EXIT_SUCCESS);
}

void OptionsLexer::SetCpuZ80()
{
	cpu = CPU_Z80;
	cpuName = CPU_Z80_NAME;
	defines.push_back(CPU_Z80_DEFINE);
	defines.push_back(CPU_ZILOG_DEFINE);
}

void OptionsLexer::SetCpuZ80n()
{
	cpu = CPU_Z80N;
	cpuName = CPU_Z80N_NAME;
	defines.push_back(CPU_Z80N_DEFINE);
	defines.push_back(CPU_ZILOG_DEFINE);
}

void OptionsLexer::SetCpuZ180()
{
	cpu = CPU_Z180;
	cpuName = CPU_Z180_NAME;
	defines.push_back(CPU_Z180_DEFINE);
	defines.push_back(CPU_ZILOG_DEFINE);
}

void OptionsLexer::SetCpuR2k()
{
	cpu = CPU_R2K;
	cpuName = CPU_R2K_NAME;
	defines.push_back(CPU_R2K_DEFINE);
	defines.push_back(CPU_RABBIT_DEFINE);
}

void OptionsLexer::SetCpuR3k()
{
	cpu = CPU_R3K;
	cpuName = CPU_R3K_NAME;
	defines.push_back(CPU_R3K_DEFINE);
	defines.push_back(CPU_RABBIT_DEFINE);
}

void OptionsLexer::SetCpu8080()
{
	cpu = CPU_8080;
	cpuName = CPU_8080_NAME;
	defines.push_back(CPU_8080_DEFINE);
	defines.push_back(CPU_INTEL_DEFINE);
}

void OptionsLexer::SetCpu8085()
{
	cpu = CPU_8085;
	cpuName = CPU_8085_NAME;
	defines.push_back(CPU_8085_DEFINE);
	defines.push_back(CPU_INTEL_DEFINE);
}

void OptionsLexer::SetCpuGbz80()
{
	cpu = CPU_GBZ80;
	cpuName = CPU_GBZ80_NAME;
	defines.push_back(CPU_GBZ80_DEFINE);
}

void OptionsLexer::SetCpuTi83()
{
	SetCpuZ80();
	isTi83Plus = false;
}

void OptionsLexer::SetCpuTi83Plus()
{
	SetCpuZ80();
	isTi83Plus = true;
}
