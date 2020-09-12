//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "Options.h"
#include "legacy.h"
#include <cassert>

fs::path Options::GetOutputBinary() const {
    if (outputFile.empty())
        return fs::path();
    else if (makeBinary)
        return outputFile;
    else
        return fs::path();
}

fs::path Options::GetOutputObject() const {
    if (outputFile.empty())
        return fs::path();
    else if (!makeBinary) {
        fs::path obj = outputFile;
        obj.replace_extension(".o");		// TODO: make constant
        return obj;
    }
    else
        return fs::path();
}

void Options::SetDebug() {
    debugInfo = true;
    mapfile = true;
}

void Options::SetAppmake(Appmake appmake) {
    switch (appmake) {
    case Appmake::ZX:
        appmakeOptions = "+zx";
        appmakeExtension = ".tap";
        appmakeOriginMin = 23760;
        appmakeOriginMax = 0xFFFF;
        break;

    case Appmake::ZX81:
        appmakeOptions = "+zx81";
        appmakeExtension = ".P";
        appmakeOriginMin = 16514;
        appmakeOriginMax = 16514;
        break;

    default: assert(0);
    }

    this->appmake = appmake;
    set_origin_option(appmakeOriginMin);
    makeBinary = true;
    cpu.Init(Cpu::Type::Z80);
}
