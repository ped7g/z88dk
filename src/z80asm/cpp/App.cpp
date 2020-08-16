//-----------------------------------------------------------------------------
// z80asm restart - program driver
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "App.h"
#include "config.h"
#include "legacy.h"
#include "z80asm_manual.h"
#include "z80asm_usage.h"
#include "options.yy.h"
#include "ghc/filesystem.hpp"
namespace fs = ghc::filesystem;		// until we have std::filesystem

#include <cstring>
#include <iostream>
#include <sstream>

#ifndef PREFIX
#define PREFIX "/usr/local/share/z88dk"
#endif

#ifndef Z88DK_VERSION
#define Z88DK_VERSION "build " __DATE__
#endif

static const char copyrightMsg[] {
    "Z80 Module Assembler " Z88DK_VERSION "\n"
    "(c) InterLogic 1993-2009, Paulo Custodio 2011-2020"
};


// sigleton
App app;

App::App() {
    optionsLexer = new OptionsLexer;
}

App::App(const App& rhs) {
    optionsLexer = new OptionsLexer;
    files = rhs.files;
}

App::~App() {
    delete optionsLexer;
}

App& App::operator=(const App& rhs) {
    optionsLexer = new OptionsLexer;
    files = rhs.files;
    return *this;
}

bool App::ParseEnv(const std::string& envVariable) {
    using namespace std;

    ClearEnvPendingOptions();
    const char* env = getenv(envVariable.c_str());
    if (env) {
        stringstream iss{ string(env) };
        string arg;

        while (iss >> arg) {
            arg = App::ExpandEnvironmentVars(arg);
            optionsLexer->in(arg);
            if (!optionsLexer->lex()) {					// TODO: error handling
                AppendEnvPendingOptions(arg.c_str());	// pass options not parsed to the C code
            }
        }
    }
    return true;
}

bool App::ParseArgs(int argc, char* argv[]) {
    using namespace std;

    // if no arguments, just show usage and exit
    if (argc == 1)
        ExitUsage();

    // parse options
    for (int i = 1; i < argc; ++i) {
        string arg = App::ExpandEnvironmentVars(argv[i]);
        optionsLexer->in(arg);
        if (optionsLexer->lex())
            argv[i][0] = '\0';		// cancel this argument for next pass
        else {						// TODO: error handling
        }
    }

    return true;
}

bool App::AddDefines() {
    // add CPU defines
    for (auto define = options.cpu.GetDefinesBegin();
            define != options.cpu.GetDefinesEnd();
            ++define) {
        options.defines.push_back({ *define, 1 });
    }

    // add ARCH defines
    for (auto define = options.arch.GetDefinesBegin();
            define != options.arch.GetDefinesEnd();
            ++define) {
        options.defines.push_back({ define->first, define->second });
    }

    return true;
}

bool App::AddLibraries() {
    // add -lxxx libraries
    for (auto& library : options.libraries)
        if (!library_file_append(library.c_str()))
            return false;

    // add z80asm-CPU-IXIY.lib
    auto z80asmLibrary = SearchZ80asmLibrary();
    if (!z80asmLibrary.empty())
        if (!library_file_append(z80asmLibrary.c_str()))
            return false;

    return true;
}

void App::ExitUsage() {
    using namespace std;

    cout << copyrightMsg << endl
         << endl << z80asm_usage;
    exit(EXIT_SUCCESS);
}

void App::ExitManual() {
    using namespace std;

    cout << z80asm_manual;
    exit(EXIT_SUCCESS);
}

std::string App::ExpandEnvironmentVars(std::string str) {
    using namespace std;

    size_t pos1 = str.find("${");
    while (pos1 != string::npos) {
        size_t pos2 = str.find("}", pos1 + 2);
        if (pos2 != string::npos) {		// found ${ENVVAR}
            string var = str.substr(pos1 + 2, pos2 - pos1 - 2);
            const char* value = getenv(var.c_str());
            if (value == nullptr)
                value = "";
            str = str.substr(0, pos1) + value + str.substr(pos2 + 1);
            pos1 += strlen(value);
        }
        else
            pos1 += 2;

        // search next
        pos1 = str.find("${", pos1);
    }

    return str;
}

std::string App::SearchFile(const std::string& file,
                            const std::vector<std::string>& dirs) {
    using namespace std;

    // if no directory list or file exists, return filename
    if (dirs.empty() || fs::is_regular_file(fs::path(file)))
        return file;

    // search in dir list
    for (const auto& dir : dirs) {
        string testFile = dir + "/" + file;
        if (fs::is_regular_file(fs::path(testFile)))
            return testFile;
    }

    // not found, return original file name
    return file;
}

/*	z80asm standard library
	search in current die, then in exe path, then in exe path/../lib, then in ZCCCFG/..
	Ignore if not found, probably benign - user will see undefined symbols
	__z80asm__xxx if the library routines are called
*/
std::string App::SearchZ80asmLibrary() {
    using namespace std;

    // build library name: z80asm-CPU-[ixiy].lib
    string libName = "z80asm-" + options.cpu.GetName() + "-";
    if (options.swapIxIy)
        libName += "ixiy";
    libName += ".lib";

    // check current directory
    if (CheckLibraryExists(libName))
        return libName;

    // check PREFIX/lib
    string libPath = string(PREFIX) + "/lib/" + libName;
    if (CheckLibraryExists(libPath))
        return libPath;

    // check -L path
    libPath = SearchFile(libName, options.libraryPath);
    if (libPath != libName && CheckLibraryExists(libPath))
        return libPath;

    // check environment ${ZCCCFG}
    libPath = "${ZCCCFG}/../" + libName;
    libPath = ExpandEnvironmentVars(libPath);
    if (CheckLibraryExists(libPath))
        return libPath;

    return string();		// empty string if not found
}

bool App::CheckLibraryExists(const std::string& filename) {
    using namespace std;

    if (fs::is_regular_file(fs::path(filename)))
        return true;
    else {
        if (options.verbose)
            cout << "Library '" << filename << "' not found" << endl;
        return false;
    }
}
