//-----------------------------------------------------------------------------
// z80asm restart - output file
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

#include <fstream>
#include <iostream>

#include "ghc/filesystem.hpp"
namespace fs = ghc::filesystem;		// until we have std::filesystem

class OutputFile {
public:
    OutputFile(const fs::path& filename, std::ios::openmode mode = std::ios::out);

    bool good() const { return os.good(); }
    void Putline(std::string& line);	// adds endl
    void Put(std::string& text);
    void Put(const char* data, size_t size);
    std::ofstream& Out() { return os; }

    void Remove();						// close and remove file

private:
    fs::path filename;
    std::ofstream os;
};
