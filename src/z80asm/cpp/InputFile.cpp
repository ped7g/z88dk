//-----------------------------------------------------------------------------
// z80asm restart - source input
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "InputFile.h"
#include "App.h"
#include "Reporter.h"
#include "Utils.h"
#include <cassert>

InputFile::InputFile(const fs::path& filename) {
    // search file in path
    fs::path foundFilename = SearchFile(filename, app.options.includePath);
    if (foundFilename.empty()) {
        app.reporter.Error(Reporter::Msg::FILE_NOT_FOUND, filename.generic_string());
        foundFilename = filename;
    }
    else if (app.reporter.FileInStack(foundFilename))
        app.reporter.Error(Reporter::Msg::RECURSIVE_INCLUDE, foundFilename.generic_string());
    else {
        is.open(foundFilename, std::ios::binary);	// binary for cross-platform newlines
        if (is.is_open())
            isGood = true;
    }

    // push a file to the input stack
    app.reporter.EnterFile(foundFilename);

    // push file directory to include path to allow relative paths in includes
    fs::path dirname = fs::path(foundFilename).parent_path();
    if (dirname.empty())
        app.options.includePath.push_back(fs::path("."));
    else
        app.options.includePath.push_back(dirname);
}

InputFile::~InputFile() {
    // pop file from input stack
    app.reporter.LeaveFile();

    // pop directory from include path
    assert(!app.options.includePath.empty());
    app.options.includePath.pop_back();
}

bool InputFile::Getline(std::string& line) {
    line.clear();
    if (!is.is_open())
        return false;
    else {
        if (!SafeGetline(is, line).eof()) {
            app.reporter.IncLineNum();
            return true;
        }
        else
            return false;
    }
}
