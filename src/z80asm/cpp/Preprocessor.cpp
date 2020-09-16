//-----------------------------------------------------------------------------
// z80asm restart - preprocessor
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "Preprocessor.h"
#include "App.h"

Preprocessor::Preprocessor(const fs::path& inFilename, const fs::path& outFilename)
    : inFile(inFilename), outFile(outFilename)
    , initialErrorCount(app.reporter.GetErrorCount()) {
    // load defines from command line arguments
    for (auto& define : app.options.defines)
        defines.Set(define.first, std::to_string(define.second));
}

Preprocessor::~Preprocessor() {
    if (initialErrorCount != app.reporter.GetErrorCount())		// got errors
        outFile.Remove();
}

bool Preprocessor::Run() {
    if (!good())
        return false;

    std::string line;
    while (Getline(line))
        Putline(line);

    if (initialErrorCount == app.reporter.GetErrorCount())
        return true;
    else
        return false;
}

bool Preprocessor::Getline(std::string& line) {
    if (!inFile.Getline(line))
        return false;

    // send to list file

    return true;
}

void Preprocessor::Putline(const std::string& line) {
    if (outLineNum == 0 || outLineNum != app.reporter.GetLineNum()) {
        outFile.Out() << std::endl
                      << "\t" << "LINE" << "\t"
                      << app.reporter.GetLineNum()
                      << ", \"" << app.reporter.GetFilename().generic_string() << "\""
                      << std::endl;
        outLineNum = app.reporter.GetLineNum();
    }

    outFile.Out() << line << std::endl;
    outLineNum++;
}
