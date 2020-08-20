//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

#include "Cpu.h"
#include "Arch.h"

#include <string>
#include <utility>
#include <vector>

#include "ghc/filesystem.hpp"
namespace fs = ghc::filesystem;		// until we have std::filesystem

struct Options {
    bool	verbose{ false };			// true to be verbose
    bool	mapfile{ false };			// generate map file
    bool	symtable{ false };			// generate symbol table file
    bool	listfile{ false };			// generate list file
    bool	globaldef{ false };			// generate table with global defintions
    bool	makeBinary{ false };		// generate binary file
    bool	splitBinary{ false };		// true to split binary file per section
    bool	update{ false };			// true to build only changed files

    bool	swapIxIy{ false };			// swap IX and IY
    bool	optimizeSpeed{ false };		// true to optimize for speed
    bool	debugInfo{ false };			// add debug info to map file

    fs::path outputLibrary;				// name of output library if -x is given
    fs::path outputDirectory;			// path to store output files
    fs::path outputFile;				// name of output binary or object file
    fs::path GetOutputBinary() const;	// -o argument if -b
    fs::path GetOutputObject() const;	// -o argument if !-b

    Cpu		cpu;
    Arch	arch;

    std::vector<std::pair<std::string, int>> defines;		// list of -D defines
    std::vector<fs::path>	includePath;	// where to search for source files
    std::vector<fs::path>	libraryPath;	// where to search for library files
    std::vector<fs::path>	libraries;		// list of libraries to link
};
