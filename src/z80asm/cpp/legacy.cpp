//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "legacy.h"

#include "Arch.h"
#include "CmdArgs.h"
#include "Cpu.h"
#include "Options.h"

const char* CPU_Z80_NAME = "z80";
const char* CPU_Z80N_NAME = "z80n";
const char* CPU_Z180_NAME = "z180";
const char* CPU_R2K_NAME = "r2k";
const char* CPU_R3K_NAME = "r3k";
const char* CPU_8080_NAME = "8080";
const char* CPU_8085_NAME = "8085";
const char* CPU_GBZ80_NAME = "gbz80";

const char* CPU_Z80_DEFINE = "__CPU_Z80__";
const char* CPU_Z80N_DEFINE = "__CPU_Z80N__";
const char* CPU_Z180_DEFINE = "__CPU_Z180__";
const char* CPU_R2K_DEFINE = "__CPU_R2K__";
const char* CPU_R3K_DEFINE = "__CPU_R3K__";
const char* CPU_8080_DEFINE = "__CPU_8080__";
const char* CPU_8085_DEFINE = "__CPU_8085__";
const char* CPU_GBZ80_DEFINE = "__CPU_GBZ80__";

const char* CPU_ZILOG_DEFINE = "__CPU_ZILOG__";
const char* CPU_RABBIT_DEFINE = "__CPU_RABBIT__";
const char* CPU_INTEL_DEFINE = "__CPU_INTEL__";

const char* SWAP_IXIY_DEFINE = "__SWAP_IX_IY__";

int GetCpu() 
{
	return static_cast<int>(theCpu.GetType());
}

const char* GetCpuName()
{
	return theCpu.GetName().c_str();
}

int GetInvokeOpcode()
{
	return theArch.INVOKE();
}

bool SwapIxIy()
{
	return theOptions.swapIxIy;
}

void TraverseDefines(void(*func)(const char *name, int value))
{
	for (auto it = theOptions.defines.cbegin();
		it != theOptions.defines.cend(); ++it)
		func((*it).first.c_str(), (*it).second);
}

bool OptionVerbose()
{
	return theOptions.verbose;
}

bool OptionOptimizeSpeed()
{
	return theOptions.optimizeSpeed;
}

bool OptionDebugInfo()
{
	return theOptions.debugInfo;
}

bool OptionMapFile()
{
	return theOptions.doMapFile;
}

const char* GetEnvPendingOpts()
{
	return theCmdArgs.GetEnvPendingOpts();
}
