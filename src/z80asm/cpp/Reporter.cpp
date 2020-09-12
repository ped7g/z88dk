//-----------------------------------------------------------------------------
// z80asm restart - error and diagnostics reporter
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "App.h"
#include "Reporter.h"
#include <iostream>
#include <sstream>
#include <cassert>

const char* Reporter::errorMsgs[Reporter::Msg::NUM_MSGS] = {
#	define X(id, msg)	msg,
#	include "Reporter_msgs.def"
};

std::ostream& operator<<(std::ostream& os, const Location& location) {
    if (!location.filename.empty())
        os << location.filename.generic_string() << ":" << location.lineNum << ": ";
    return os;
}

void Reporter::EnterFile(const fs::path& filename) {
    Location location{ filename, 0 };
    locations.push_back(location);
    lastLocation = std::make_shared<Location>(location);
}

void Reporter::LeaveFile() {
    assert(!locations.empty());
    locations.pop_back();
}

bool Reporter::FileInStack(const fs::path& filename) const {
    for (auto& location : locations) {
        if (location.filename == filename)
            return true;
    }
    return false;
}

const fs::path& Reporter::GetFilename() const {
    static fs::path empty;

    if (locations.empty())
        return empty;
    else
        return locations.back().filename;
}

int Reporter::GetLineNum() const {
    if (locations.empty())
        return 0;
    else
        return locations.back().lineNum;
}

std::shared_ptr<Location> Reporter::GetLocation() const {
    return lastLocation;
}

void Reporter::SetLineNum(int lineNum) {
    assert(!locations.empty());
    locations.back().lineNum = lineNum;
    lastLocation = std::make_shared<Location>(locations.back());
}

void Reporter::IncLineNum() {
    SetLineNum(GetLineNum() + 1);
}

void Reporter::SetCLocation(const fs::path& cFilename, int cLineNum) {
    cLocation.filename = cFilename;
    cLocation.lineNum = cLineNum;
}

void Reporter::SetCLocation(int cLineNum) {
    cLocation.lineNum = cLineNum;
}

void Reporter::Error(Msg msg, const std::string& arg) {
    Message("error", msg, arg);
    errorCount++;
}

void Reporter::Warning(Msg msg, const std::string& arg) {
    Message("warning", msg, arg);
}

void Reporter::Info(Msg msg, const std::string& arg) {
    if (app.options.verbose) {
        std::cout << errorMsgs[msg];
        if (!arg.empty())
            std::cout << ": " << arg;
        std::cout << std::endl;
    }
}

void Reporter::Message(const std::string& prefix, Msg msg, const std::string& arg) {
    if (locations.empty())
        Message1(Location(), prefix, msg, arg);
    else {
        for (auto it = locations.rbegin(); it != locations.rend(); ++it) {
            if (it == locations.rbegin())
                Message1(*it, prefix, msg, arg);
            else
                std::cerr << *it << "included from here" << std::endl;
        }
    }

    if (!cLocation.filename.empty())
        Message1(cLocation, prefix, msg, arg);
}

void Reporter::Message1(const Location& location, const std::string& prefix, Msg msg,
                        const std::string& arg) {
    std::cerr << location << prefix << ": " << errorMsgs[msg];
    if (!arg.empty())
        std::cerr << ": " << arg;
    std::cerr << std::endl;
}

