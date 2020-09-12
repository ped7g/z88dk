//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "App.h"
#include "legacy.h"

#include <iostream>

int main(int argc, char* argv[]) {
    z80asm_init();

    // fini must run even if there were errors
    bool ok = true;

    if (!app.ParseEnv())
        ok = false;
    else if (!app.ParseArgs(argc, argv))
        ok = false;
    else if (!app.MakeOutputDirectory())
        ok = false;
    else if (!app.AddDefines())
        ok = false;
    else if (!app.AddLibraries())
        ok = false;
    else if (!app.Assemble())
        ok = false;
    else if (!app.MakeLibrary())
        ok = false;
    else if (z80asm_main() != 0) // --> failed
        ok = false;
    else if (!app.RunAppmake())
        ok = false;

    z80asm_fini();
    if (ok)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}
