//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "legacy.h"
#include "CmdArgs.h"

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
	return theCmdArgs.GetCpu();
}

const char* GetCpuName()
{
	return theCmdArgs.GetCpuName().c_str();
}

bool IsTi83Plus()
{
	return theCmdArgs.IsTi83Plus();
}

bool SwapIxIy()
{
	return theCmdArgs.SwapIxIy();
}

void TraverseDefines(void(*func)(const char *name))
{
	for (auto it = theCmdArgs.cbeginDefines();
		it != theCmdArgs.cendDefines(); ++it)
		func((*it).c_str());
}

bool OptionVerbose()
{
	return theCmdArgs.IsVerbose();
}

bool OptionOptimizeSpeed()
{
	return theCmdArgs.IsOptimizeSpeed();
}

bool OptionDebugInfo()
{
	return theCmdArgs.IsDebugInfo();
}

bool OptionMapFile()
{
	return theCmdArgs.DoMapFile();
}

const char* GetEnvPendingOpts()
{
	return theCmdArgs.GetEnvPendingOpts();
}
