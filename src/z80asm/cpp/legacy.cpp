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

#include "filesystem/path.h"

#include <unordered_set>

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

const char * AddStringPool(const char * str)
{
	using namespace std;

	if (!str)
		return nullptr;

	static unordered_set<string> pool;
	const char* pooledString = pool.insert(str).first->c_str();
	return pooledString;
}

const char * ExpandEnvironmentVarsC(const char * str_)
{
	using namespace std;

	string str{ str_ };
	str = ExpandEnvironmentVars(str);
	return AddStringPool(str.c_str());
}

void PushSourceDirname(const char * filename)
{
	using namespace filesystem;

	auto dirname = path(filename).parent_path();
	theOptions.includePath.push_back(dirname.str(path::posix_path));
}

void PopSourceDirname()
{
	if (!theOptions.includePath.empty())
		theOptions.includePath.pop_back();
}

static filesystem::path SearchFile(const filesystem::path& file,
	const std::vector<std::string>& dirs)
{
	using namespace filesystem;

	// if no directory list or file exists, return filename
	if (dirs.empty() || file.is_file())
		return file;

	// search in dir list
	for (const auto& dir : dirs) {
		path testFile = path(dir) / file;
		if (testFile.is_file())
			return testFile;
	}

	// not found, return original file name
	return file;
}

const char * SearchIncludeFile(const char * filename)
{
	using namespace filesystem;

	path file = SearchFile(path(filename), theOptions.includePath);
	return AddStringPool(file.str(path::posix_path).c_str());
}

const char * SearchLibraryFile(const char * filename)
{
	using namespace filesystem;

	path file = SearchFile(path(filename), theOptions.libraryPath);
	return AddStringPool(file.str(path::posix_path).c_str());
}

