//-----------------------------------------------------------------------------
// z80asm restart - program driver
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

#include "Options.h"
#include "Reporter.h"
#include <memory>
#include <string>
#include <vector>

#include "ghc/filesystem.hpp"
namespace fs = ghc::filesystem;		// until we have std::filesystem

extern const char* Z80ASM;

class OptionsLexer;					// cannot include generated options.yy.h

class App {
public:
    App();
    App(const App& rhs);			// rule of 5
    App(App&& rhs);
    virtual ~App();
    App& operator=(const App& rhs);
    App& operator=(App&& rhs);

    bool ParseArg(std::string arg);
    bool ParseArgs(int argc, char* argv[]);
    bool ParseEnv();
    bool ParseListFile(const fs::path& filename);
    bool ExpandListGlob(const std::string& pattern);
    bool ExpandSourceGlob(const std::string& pattern);
    bool AddDefines();
    bool AddLibraries();
    bool MakeOutputDirectory();
    bool Assemble();
    bool MakeLibrary();
    bool RunAppmake();

    static void ExitUsage();
    static void ExitManual();

    // public access attributes
    Options options;
    Reporter reporter;

private:
    std::unique_ptr<OptionsLexer>	optionsLexer;
    std::vector<std::string> files;

    fs::path SearchZ80asmLibrary();
    bool CheckLibraryExists(const fs::path& filename);
    fs::path SearchSource(const fs::path& filename);
    bool AppendSource(const fs::path& filename);
    bool AssembleFile(const fs::path& filename);
};

// sigleton
extern App app;
