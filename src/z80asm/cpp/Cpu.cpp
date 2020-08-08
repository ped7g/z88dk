//-----------------------------------------------------------------------------
// z80asm restart - implement each of the supported CPUs
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "Cpu.h"

#include <cassert>

struct CpuLookup {
	Cpu::Type type;
	const char* name;		// name used in standard library search
	const char** defines;	// defines during assembly
};

static const char* z80_defines[] =	{ "__CPU_Z80__",	"__CPU_ZILOG__",	nullptr };
static const char* z80n_defines[] =	{ "__CPU_Z80N__",	"__CPU_ZILOG__",	nullptr };
static const char* z180_defines[] =	{ "__CPU_Z180__",	"__CPU_ZILOG__",	nullptr };
static const char* r2k_defines[] =	{ "__CPU_R2K__",	"__CPU_RABBIT__",	nullptr };
static const char* r3k_defines[] =	{ "__CPU_R3K__",	"__CPU_RABBIT__",	nullptr };
static const char* i8080_defines[] ={ "__CPU_8080__",	"__CPU_INTEL__",	nullptr };
static const char* i8085_defines[] ={ "__CPU_8085__",	"__CPU_INTEL__",	nullptr };
static const char* gbz80_defines[] ={ "__CPU_GBZ80__",						nullptr };

static CpuLookup cpuLookup[] = {
	{ Cpu::Type::Z80,	"z80",	z80_defines },
	{ Cpu::Type::Z80N,	"z80n",	z80n_defines },
	{ Cpu::Type::Z180,	"z180",	z180_defines },
	{ Cpu::Type::R2K,	"r2k",	r2k_defines },
	{ Cpu::Type::R3K,	"r3k",	r3k_defines },
	{ Cpu::Type::I8080,	"8080",	i8080_defines },
	{ Cpu::Type::I8085,	"8085",	i8085_defines },
	{ Cpu::Type::GBZ80,	"gbz80",gbz80_defines },
};

void Cpu::Init(Type type)
{
	for (const auto& lu : cpuLookup) {
		if (type == lu.type) {
			this->type = type;
			this->name = lu.name;
			this->defines.clear();
			for (auto define = lu.defines; *define != nullptr; ++define)
				this->defines.push_back(*define);
			return;
		}
	}
	assert(0);	// CPU not found
}

int Cpu::RST(int n) const
{
	// accept 0..7
	if (n > 0 && n < 8) 
		n *= 8;

	// check range
	if (n < 0 || n > 0x38 || (n & 7) != 0)
		return -1;

	// check specific CPUs
	switch (type) {
	case R2K: case R3K:
		switch (n) {
		case 0x00: case 0x08: case 0x30: return -1;
		default: break;
		}
	default: break;
	}

	// all ok
	return 0xC7 + n;
}
