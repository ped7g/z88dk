//-----------------------------------------------------------------------------
// z80asm restart - source input
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

#include <fstream>
#include <iostream>

#include "ghc/filesystem.hpp"
namespace fs = ghc::filesystem;		// until we have std::filesystem

class InputFile {
public:
    InputFile(const fs::path& filename);
    ~InputFile();

    bool good() const { return isGood && is.good(); }
    bool eof() const { return is.eof(); }

    bool Getline(std::string& line);

private:
    bool isGood{ false };
    std::ifstream is;
};
