//-----------------------------------------------------------------------------
// z80asm restart - implement each of the supported computer architectures
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "Arch.h"
#include "App.h"

struct ArchDefine {
    const char* name;
    int value;
};

struct ArchTable {
    const char* name;
    const ArchDefine* defines;
};

static const ArchDefine general_defines[] = {
    { nullptr, 0 }
};

static const ArchDefine ti83_defines[] = {
    { "__ARCH_TI83__",	1 },
    { nullptr, 0 }
};

static const ArchDefine ti83plus_defines[] = {
    { "__ARCH_TI83PLUS__",	1 },
    { nullptr, 0 }
};

// NOTE: keep in sync with enum Type
static ArchTable archTable[] = {
    { /* GENERAL */		"general",	general_defines		},
    { /* TI83 */		"ti83",		ti83_defines		},
    { /* TI83PLUS */	"ti83plus",	ti83plus_defines	},
};


void Arch::Init(Type type_) {
    type = type_;
    name = archTable[type].name;
    defines.clear();
    for (auto define = archTable[type].defines; define->name != nullptr; ++define)
        defines.push_back({ define->name, define->value });
}

int Arch::INVOKE() const {
    switch (type) {
    case TI83PLUS: return app.options.cpu.RST(0x28);		// Ti83Plus: RST 28H instruction
    default: break;
    }

    return app.options.cpu.CALL();						// all others: CALL
}
