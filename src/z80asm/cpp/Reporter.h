//-----------------------------------------------------------------------------
// z80asm restart - error and diagnostics reporter
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "ghc/filesystem.hpp"
namespace fs = ghc::filesystem;		// until we have std::filesystem

struct Location {
    fs::path	filename;
    int			lineNum;

    Location(const Location& location)
        : filename(location.filename), lineNum(location.lineNum) {}
    Location(const fs::path filename = fs::path(), int lineNum = 0)
        : filename(filename), lineNum(lineNum) {}
};

std::ostream& operator<<(std::ostream& os, const Location& location);

class Reporter {
public:
    enum Msg {
#		define X(id, msg)	id,
#		include "Reporter_msgs.def"
        NUM_MSGS
    };

    void EnterFile(const fs::path& filename);
    void LeaveFile();

    bool FileInStack(const fs::path& filename) const;		// checks if already in stack

    const fs::path& GetFilename() const;
    int GetLineNum() const;
    std::shared_ptr<Location> GetLocation() const;

    void SetLineNum(int lineNum);
    void IncLineNum();

    void SetCLocation(const fs::path& cFilename, int cLineNum);
    void SetCLocation(int cLineNum);

    int GetErrorCount()const { return errorCount; }

    void Error(Msg msg, const std::string& arg = "");
    void Warning(Msg msg, const std::string& arg = "");
    void Info(Msg msg, const std::string& arg = "");	// only if verbose


private:
    std::vector<Location>		locations;		// current input stack of locations
    std::shared_ptr<Location>	lastLocation;	// shared pointer of each location returned by GetLocation()
    Location					cLocation;		// last location set by C_LINE
    int							errorCount{ 0 };// total number of errors

    static const char* errorMsgs[NUM_MSGS];

    void Message(const std::string& prefix, Msg msg, const std::string& arg = "");
    void Message1(const Location& location, const std::string& prefix, Msg msg,
                  const std::string& arg = "");
};
