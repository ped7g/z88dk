//-----------------------------------------------------------------------------
// z80asm restart - output file
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "OutputFile.h"

OutputFile::OutputFile(const fs::path& filename, std::ios::openmode mode)
    : filename(filename), os(filename, mode | std::ios::out) {
}

void OutputFile::Putline(std::string& line) {
    os << line << std::endl;
}

void OutputFile::Put(std::string& text) {
    os << text;
}

void OutputFile::Put(const char* data, size_t size) {
    os.write(data, size);
}

void OutputFile::Remove() {
    if (os.is_open())
        os.close();
    remove(filename.generic_string().c_str());
}
