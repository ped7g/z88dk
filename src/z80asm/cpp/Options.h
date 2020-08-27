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

enum class Appmake { NONE, ZX81, ZX };

struct Options {
    // attributes
    bool	verbose{ false };			// true to be verbose
    bool	mapfile{ false };			// generate map file
    bool	symtable{ false };			// generate symbol table file
    bool	listfile{ false };			// generate list file
    bool	globaldef{ false };			// generate table with global defintions
    bool	makeBinary{ false };		// generate binary file
    bool	splitBinary{ false };		// true to split binary file per section
    bool	update{ false };			// true to build only changed files
    bool	relocatable{ false };		// true to produce relocatable code
    bool	relocInfo{ false };			// true to generate a relocation info file

    bool	swapIxIy{ false };			// swap IX and IY
    bool	optimizeSpeed{ false };		// true to optimize for speed
    bool	debugInfo{ false };			// add debug info to map file
    int		filler{ 0 };				// filler value for DEFS

    fs::path outputLibrary;				// name of output library if -x is given
    fs::path outputDirectory;			// path to store output files
    fs::path outputFile;				// name of output binary or object file

    Cpu		cpu;
    Arch	arch;
    Appmake	appmake{ Appmake::NONE };	// which appmake to run after build
    std::string appmakeOptions;			// options to pass to appmake
    std::string appmakeExtension;		// appmake output file extension
    int		appmakeOriginMin{ -1 };		// minimum and ...
    int		appmakeOriginMax{ -1 };		// ... maximum vakues for origin

    std::vector<std::pair<std::string, int>> defines;		// list of -D defines
    std::vector<fs::path>	includePath;	// where to search for source files
    std::vector<fs::path>	libraryPath;	// where to search for library files
    std::vector<fs::path>	libraries;		// list of libraries to link

    // methods
    fs::path GetOutputBinary() const;	// -o argument if -b
    fs::path GetOutputObject() const;	// -o argument if !-b

    void SetDebug();
    void SetAppmake(Appmake appmake);
};
