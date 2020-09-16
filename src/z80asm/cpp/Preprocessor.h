//-----------------------------------------------------------------------------
// z80asm restart - preprocessor
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

#include "InputFile.h"
#include "OutputFile.h"
#include "SymbolTable.h"

#include "ghc/filesystem.hpp"
namespace fs = ghc::filesystem;		// until we have std::filesystem

class Preprocessor {
public:
    Preprocessor(const fs::path& inFilename, const fs::path& outFilename);
    ~Preprocessor();

    bool good() const { return inFile.good() && outFile.good(); }
    bool Run();

    bool Getline(std::string& line);
    void Putline(const std::string& line);

private:
    InputFile		inFile;
    OutputFile		outFile;
    int				initialErrorCount;
    int				outLineNum{ 0 };
    SymbolTable<std::string> defines;
};
