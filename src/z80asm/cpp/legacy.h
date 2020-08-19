//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

#include <stdbool.h>

// interface between legacy C code and new C++ code
#ifdef __cplusplus
extern "C" {
#endif
// common constants
enum {	// TODO: use a class enum when no longer needed by C
    CPU_NOT_DEFINED = 0,
    CPU_Z80 = (1 << 0),
    CPU_Z80N = (1 << 1),
    CPU_Z180 = (1 << 2),
    CPU_R2K = (1 << 3),
    CPU_R3K = (1 << 4),
    CPU_8080 = (1 << 5),
    CPU_8085 = (1 << 6),
    CPU_GBZ80 = (1 << 7),

    CPU_ZILOG = (CPU_Z80 | CPU_Z80N | CPU_Z180),
    CPU_RABBIT = (CPU_R2K | CPU_R3K),
    CPU_INTEL = (CPU_8080 | CPU_8085),
};

extern const char* CPU_Z80_NAME;
extern const char* CPU_Z80N_NAME;
extern const char* CPU_Z180_NAME;
extern const char* CPU_R2K_NAME;
extern const char* CPU_R3K_NAME;
extern const char* CPU_8080_NAME;
extern const char* CPU_8085_NAME;
extern const char* CPU_GBZ80_NAME;

extern const char* CPU_Z80_DEFINE;
extern const char* CPU_Z80N_DEFINE;
extern const char* CPU_Z180_DEFINE;
extern const char* CPU_R2K_DEFINE;
extern const char* CPU_R3K_DEFINE;
extern const char* CPU_8080_DEFINE;
extern const char* CPU_8085_DEFINE;
extern const char* CPU_GBZ80_DEFINE;

extern const char* CPU_ZILOG_DEFINE;
extern const char* CPU_RABBIT_DEFINE;
extern const char* CPU_INTEL_DEFINE;

extern const char* SWAP_IXIY_DEFINE;

// call legacy
int z80asm_main(int argc, char* argv[]);
bool library_file_append(const char* filename);

// called by legacy
void ClearEnvPendingOptions();
void SetEnvPendingOptions(const char* str);
void AppendEnvPendingOptions(const char* str);
const char* GetEnvPendingOptions();

bool OptionVerbose();
bool OptionMapfile();
bool OptionSymtable();
bool OptionListfile();
bool OptionGlobaldef();
bool OptionMakeBinary();
void SetOptionBinary(bool f);
bool OptionSplitBinary();

const char* GetOutputLibrary();
const char* GetOutputDirectory();
const char* GetOutputBinary();
const char* GetOutputObject();

int GetCpu();
int GetInvokeOpcode();
bool SwapIxIy();
void TraverseDefines(void(*func)(const char* name, int value));
bool OptionOptimizeSpeed();
bool OptionDebugInfo();

const char* AddStringPool(const char* str);
const char* ExpandEnvironmentVarsC(const char* str);

void PushSourceDirname(const char* filename);
void PopSourceDirname();
const char* SearchIncludeFile(const char* filename);
const char* SearchLibraryFile(const char* filename);

#ifdef __cplusplus
};
#endif
