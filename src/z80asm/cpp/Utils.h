//-----------------------------------------------------------------------------
// z80asm restart - utilities
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#pragma once

#include <string>
#include <vector>

#include "ghc/filesystem.hpp"
namespace fs = ghc::filesystem;		// until we have std::filesystem

std::string LTrim(const std::string& s);
std::string RTrim(const std::string& s);
std::string Trim(const std::string& s);
std::string ExpandEnvVars(std::string str);
fs::path SearchFile(const fs::path& file, const std::vector<fs::path>& dirs);
std::string CanonPath(std::string pathname);
std::vector<fs::path> FindGlob(const std::string& pattern);
std::istream& SafeGetline(std::istream& is, std::string& t);
