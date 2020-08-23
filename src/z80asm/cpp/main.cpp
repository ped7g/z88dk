//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "App.h"
#include "legacy.h"

#include <iostream>

int main(int argc, char* argv[]) {

    if (!app.ParseEnv())
        return EXIT_FAILURE;
    if (!app.ParseArgs(argc, argv))
        return EXIT_FAILURE;
    if (!app.MakeOutputDirectory())
        return EXIT_FAILURE;
    if (!app.AddDefines())
        return EXIT_FAILURE;
    if (!app.AddLibraries())
        return EXIT_FAILURE;

    // init and fini must run even if there were errors
    bool ok = true;
    z80asm_init();
    if (z80asm_main(argc, argv) != 0) // --> failed
        ok = false;
    if (ok)
        if (!app.RunAppmake())
            ok = false;
    z80asm_fini();

    if (ok)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}
