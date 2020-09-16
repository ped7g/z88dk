//-----------------------------------------------------------------------------
// z80asm restart - program driver
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "App.h"
#include "InputFile.h"
#include "Utils.h"
#include "config.h"
#include "legacy.h"
#include "z80asm_manual.h"
#include "z80asm_usage.h"
#include "options.yy.h"

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
    optionsLexer = std::make_unique<OptionsLexer>();
}

App::App(const App& rhs) {
    optionsLexer = std::make_unique<OptionsLexer>();
    files = rhs.files;
}

App::App(App&& rhs) {
    optionsLexer = std::move(rhs.optionsLexer);
    files = std::move(rhs.files);
}

App::~App() {
}

App& App::operator=(const App& rhs) {
    optionsLexer = std::make_unique<OptionsLexer>();
    files = rhs.files;
    return *this;
}

App& App::operator=(App&& rhs) {
    optionsLexer = std::move(rhs.optionsLexer);
    files = std::move(rhs.files);
    return *this;
}

bool App::ParseArg(std::string arg) {
    arg = ExpandEnvVars(arg);
    arg = Trim(arg);
    if (arg.empty())
        return true;

    switch (arg[0]) {
    case '-':		// option
    case '+':
        optionsLexer->in(arg);
        if (!optionsLexer->lex()) {
            reporter.Error(Reporter::Msg::ILLEGAL_OPTION, arg);
            return false;
        }
        break;

    case '@':							// file list
        arg.erase(0, 1);				// remove @
        arg = Trim(arg);				// remove blanks
        if (!ExpandListGlob(arg))
            return false;
        break;

    case ';':							// comment
    case '#':
        break;

    default:							// file
        if (!ExpandSourceGlob(arg))
            return false;
        break;
    }

    return true;
}

bool App::ParseArgs(int argc, char* argv[]) {
    // if no arguments, just show usage and exit
    if (argc == 1)
        ExitUsage();

    // parse options
    for (int i = 1; i < argc; ++i) {
        if (!ParseArg(argv[i]))
            return false;
    }

    // error if options but no source file
    if (get_num_errors() == 0 && reporter.GetErrorCount() == 0 && options.files.empty()) {
        reporter.Error(Reporter::Msg::NO_SRC_FILE);
        return false;
    }

    return true;
}

bool App::ParseEnv() {
    using namespace std;

    const char* env = getenv(ENV_Z80ASM);
    if (env) {
        stringstream iss{ string(env) };
        string arg;
        while (iss >> arg) {
            if (!ParseArg(arg))
                return false;
        }
    }
    return true;
}

bool App::ParseListFile(const fs::path& filename) {
    std::string line;

    InputFile in{ filename };
    if (!in.good())
        return false;					// could not open file
    while (in.Getline(line)) {
        if (!ParseArg(line))
            return false;				// parse error
    }
    return true;
}

bool App::ExpandListGlob(const std::string& pattern) {
    if (pattern.find_first_of("*?") != std::string::npos) {		// have a pattern
        auto files = FindGlob(pattern);
        if (files.empty()) {
            reporter.Error(Reporter::Msg::GLOB_NO_FILES, pattern);
            return false;
        }
        else {
            for (auto& file : files) {
                if (!ParseListFile(file))
                    return false;
            }
        }
    }
    else {
        if (!ParseListFile(pattern))
            return false;
    }

    return true;
}

bool App::ExpandSourceGlob(const std::string& pattern) {
    if (pattern.find_first_of("*?") != std::string::npos) {		// have a pattern
        auto files = FindGlob(pattern);
        if (files.empty()) {
            reporter.Error(Reporter::Msg::GLOB_NO_FILES, pattern);
            return false;
        }
        else {
            for (auto& file : files) {
                if (!AppendSource(file))
                    return false;
            }
            return true;
        }
    }
    else
        return AppendSource(pattern);
}

bool App::AddDefines() {
    // add CPU defines
    for (auto define = options.cpu.GetDefinesBegin();
            define != options.cpu.GetDefinesEnd();
            ++define) {
        options.defines.push_back({ *define, 1 });
    }

    if (options.swapIxIy)
        options.defines.push_back({ "__SWAP_IX_IY__", 1 });		// TODO: make constant

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
    fs::path z80asmLibrary = SearchZ80asmLibrary();
    if (!z80asmLibrary.empty())
        if (!library_file_append(z80asmLibrary.generic_string().c_str()))
            return false;

    return true;
}

bool App::MakeOutputDirectory() {
    auto dir = options.outputDirectory;
    if (dir.empty())
        return true;
    else if (fs::is_directory(dir))
        return true;
    else
        return fs::create_directories(dir);
}

bool App::Assemble() {
    // create defines
    for (auto& define : options.defines)
        define_static_def_sym_c(define.first.c_str(), define.second);

    // assemble all files
    bool allOk = true;
    for (auto& file : options.files) {
        if (!AssembleFile(file))
            allOk = false;
    }

    return allOk;
}

bool App::MakeLibrary() {
    if (options.outputLibrary.empty())
        return true;		// no -x option
    else {
        // create a C-list to pass to make_library
        const char** files = new const char* [options.files.size()];
        for (size_t i = 0; i < options.files.size(); i++)
            files[i] = options.files[i].c_str();

        // call C
        make_library(options.outputLibrary.generic_string().c_str(),
                     options.files.size(), files);

        // free array
        delete[] files;

        // check for errors
        if (get_num_errors() == 0 && reporter.GetErrorCount() == 0)
            return true;
        else
            return false;
    }
}

bool App::RunAppmake() {
    using namespace std;

    if (options.appmake == Appmake::NONE)
        return true;

    int origin = get_first_section_origin();
    if (origin < options.appmakeOriginMin ||
            origin > options.appmakeOriginMax) {
        error_invalid_org(origin);
        return false;
    }

    fs::path binFilename = fs::path(get_first_module_filename())
                           .replace_extension(_BIN);
    fs::path outFilename = fs::path(get_first_module_filename())
                           .replace_extension(options.appmakeExtension);

    stringstream cmd;
    cmd << "appmake " << options.appmakeOptions
        << " -b \"" << binFilename.generic_string() << "\""
        << " -o \"" << outFilename.generic_string() << "\""
        << " --org " << origin;

    reporter.Info(Reporter::Msg::RUN, cmd.str());

    int rv = system(cmd.str().c_str());
    if (rv != 0) {
        reporter.Error(Reporter::Msg::RUN_FAILED, cmd.str());
        return false;
    }

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

/*	z80asm standard library
	search in current die, then in exe path, then in exe path/../lib, then in ZCCCFG/..
	Ignore if not found, probably benign - user will see undefined symbols
	__z80asm__xxx if the library routines are called
*/
fs::path App::SearchZ80asmLibrary() {
    using namespace std;

    // build library name: z80asm-CPU-[ixiy].lib
    std::string libName = "z80asm-" + options.cpu.GetName() + "-";
    if (options.swapIxIy)
        libName += "ixiy";
    libName += _LIB;

    // check current directory
    fs::path libPath = libName;
    if (CheckLibraryExists(libPath))
        return libPath;

    // check PREFIX/lib
    libPath = fs::path(PREFIX) / "lib" / libName;
    if (CheckLibraryExists(libPath))
        return libPath;

    // check -L path
    libPath = SearchFile(libName, options.libraryPath);
    if (!libPath.empty() && CheckLibraryExists(libPath))
        return libPath;

    // check environment ${ZCCCFG}
    const char* zcccfg = getenv(ENV_ZCCCFG);
    if (zcccfg) {
        libPath = fs::path(zcccfg) / ".." / libName;
        if (CheckLibraryExists(libPath))
            return libPath;
    }

    // not found, return empty path
    return fs::path();
}

bool App::CheckLibraryExists(const fs::path& filename) {
    using namespace std;

    if (fs::is_regular_file(filename))
        return true;
    else {
        reporter.Info(Reporter::Msg::LIBRARY_NOT_FOUND, filename.generic_string());
        return false;
    }
}

// search for the first file in path, with the given extension,
// with .asm extension and with .o extension
// if not found, return original file
// so that user can type "test" instead of "test.asm"
fs::path App::SearchSource(const fs::path& filename) {
    fs::path f = SearchFile(filename, options.includePath);
    if (!f.empty())
        return f;

    f = fs::path(filename).replace_extension(_ASM);
    f = SearchFile(f, options.includePath);
    if (!f.empty())
        return f;

    f = fs::path(filename).replace_extension(_O);
    f = SearchFile(f, options.includePath);
    if (!f.empty())
        return f;

    reporter.Error(Reporter::Msg::FILE_NOT_FOUND, filename.generic_string());
    return fs::path();
}

bool App::AppendSource(const fs::path& filename) {
    auto f = SearchSource(filename);
    if (f.empty())
        return false;
    else {
        options.files.push_back(f);
        return true;
    }
}

bool App::AssembleFile(const fs::path& filename) {
    assemble_file(filename.generic_string().c_str());

    // check for errors
    if (get_num_errors() == 0 && reporter.GetErrorCount() == 0)
        return true;
    else
        return false;
}
