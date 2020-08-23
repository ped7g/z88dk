//-----------------------------------------------------------------------------
// z80asm restart - program driver
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

#include "Options.h"

#include <vector>
#include <string>

#include "ghc/filesystem.hpp"
namespace fs = ghc::filesystem;		// until we have std::filesystem

class OptionsLexer;					// cannot include generated options.yy.h

class App {
public:
    App();
    App(const App& rhs);			// rule of 5
    App(App&& rhs);
    virtual ~App();
    App& operator=(const App& rhs);
    App& operator=(App&& rhs);

    bool ParseEnv(const std::string& envVariable = "Z80ASM");
    bool ParseArgs(int argc, char* argv[]);
    bool AddDefines();
    bool AddLibraries();
    bool MakeOutputDirectory();
    bool RunAppmake();

    static void ExitUsage();
    static void ExitManual();
    static std::string ExpandEnvironmentVars(std::string str);
    static fs::path SearchFile(const fs::path& file,
                               const std::vector<fs::path>& dirs);

    // public access to options
    Options options;

private:
    OptionsLexer*	optionsLexer{ nullptr };
    std::vector<std::string> files;

    fs::path SearchZ80asmLibrary();
    bool CheckLibraryExists(const fs::path& filename);
};

// sigleton
extern App app;
