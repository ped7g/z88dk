//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "App.h"
#include "legacy.h"

#include <iostream>
#include <sstream>

static bool HasVerbose(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-v")       // found vebose
            return true;
    }
    const char* env = getenv(ENV_Z80ASM);
    if (env) {
        std::stringstream iss{ std::string(env) };
        std::string arg;
        while (iss >> arg) {
            if (arg == "-v")
                return true;
        }
    }
    return false;
}

static void ShowArgv(int argc, char* argv[]) {
    if (HasVerbose(argc, argv)) {
        std::cout << "z80asm command line:";
        for (int i = 0; i < argc; i++)
            std::cout << " " << argv[i];
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    ShowArgv(argc, argv);      // show command line if verbose - before argv parsing

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
