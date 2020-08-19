//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "Options.h"

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
