//-----------------------------------------------------------------------------
// z80asm restart - utilities
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "Utils.h"
#include <iostream>
#include <algorithm>

// include C-library for file globbing
extern "C" {
#ifdef _WIN32
#  include <unixem/glob.h>
#endif
#include <glob.h>
#include <dirent.h>
};

static const std::string WHITESPACE = " \n\r\t\f\v";

std::string LTrim(const std::string& s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string RTrim(const std::string& s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string Trim(const std::string& s) {
    return RTrim(LTrim(s));
}

std::string ExpandEnvVars(std::string str) {
    size_t pos1 = str.find("${");
    while (pos1 != std::string::npos) {
        size_t pos2 = str.find("}", pos1 + 2);
        if (pos2 != std::string::npos) {		// found ${ENVVAR}
            std::string var = str.substr(pos1 + 2, pos2 - pos1 - 2);
            const char* value = getenv(var.c_str());
            if (value == nullptr)
                value = "";
            str = str.substr(0, pos1) + value + str.substr(pos2 + 1);
            pos1 += strlen(value);
        }
        else
            pos1 += 2;

        // search next
        pos1 = str.find("${", pos1);
    }
    return str;
}

fs::path SearchFile(const fs::path& file, const std::vector<fs::path>& dirs) {
    // if no directory list or file exists, return filename
    if (fs::is_regular_file(file))
        return file;

    // search in dir list
    for (const auto& dir : dirs) {
        fs::path testFile = dir / file;
        if (fs::is_regular_file(testFile))
            return testFile;
    }

    // not found, return empty path
    return fs::path();
}

std::string CanonPath(std::string pathname) {
    // replace bakslashes by foward slashes
    auto pos = pathname.find('\\');
    while (pos != std::string::npos) {
        pathname[pos] = '/';
        pos = pathname.find('\\', pos);
    }

    // remove double slashes
    pos = pathname.find("//");
    while (pos != std::string::npos) {
        pathname.erase(pos, 1);			// remove one '/'
        pos = pathname.find("//", pos);
    }

    return pathname;
}

static int xglob(const std::string& pattern, int flags,
#ifdef _WIN32	// ugly!
                 const
#endif
                 int(*errfunc)(const char* epath, int eerrno), glob_t* pglob) {
    int ret = glob(pattern.c_str(), flags, errfunc, pglob);
    if (ret == GLOB_NOMATCH || ret == 0)
        return ret;
    else {
        std::cerr << "glob pattern: " << pattern << ": ";
        switch (ret) {
        case GLOB_ABORTED: std::cerr << "filesystem problem" << std::endl; break;
        case GLOB_NOSPACE: std::cerr << "no dynamic memory" << std::endl; break;
        default: std::cerr << "unknown problem" << std::endl; break;
        }
        exit(EXIT_FAILURE);
    }
    return ret;	// not reached
}

// recursion workhorse
static void find_glob_1(std::vector<fs::path>& files, std::string pattern) {
    pattern = CanonPath(pattern);

    auto p_wc = pattern.find_first_of("*?");
    if (p_wc == std::string::npos) {					// no wildcards
        fs::path file{ pattern };
        if (fs::is_regular_file(file))
            files.push_back(file);						// only collect regular files
    }
    else if (pattern.substr(p_wc, 2) == "**") {			// '**' - recursively find all subdirs
        std::string sub_pattern = pattern.substr(0, p_wc) + pattern.substr(p_wc + 2);
        find_glob_1(files, sub_pattern);				// recurse without '**'

        sub_pattern = pattern.substr(0, p_wc + 1);		// up to first '*'
        auto p_child = pattern.find('/', p_wc);			// point to first '/' after '**'
        if (p_child == std::string::npos) 				// no child
            sub_pattern += pattern.substr(p_wc + 2);	// only one '*' up to end
        else											// has child
            sub_pattern += pattern.substr(p_wc + 2, p_child - (p_wc + 2));	// only one '*', up to '/'

        // expand glob at this directory level
        glob_t glob_files;
        int ret = xglob(sub_pattern, GLOB_NOESCAPE, NULL, &glob_files);
        if (ret == 0) {									// matched files
            for (int i = 0; i < static_cast<int>(glob_files.gl_pathc); i++) {
                std::string found = glob_files.gl_pathv[i];
                if (fs::is_directory(found)) {
                    sub_pattern = found + "/**";
                    if (p_child != std::string::npos)
                        sub_pattern += pattern.substr(p_child);
                    find_glob_1(files, sub_pattern);	// recurse
                }
                else {
                    sub_pattern = found;
                    if (p_child != std::string::npos)
                        sub_pattern += pattern.substr(p_child);
                    find_glob_1(files, sub_pattern);	// recurse
                }
            }
        }
        globfree(&glob_files);
    }
    else {												// find one level of subdirs
        auto p_child = pattern.find('/', p_wc);			// point to first '/' after wildcard
        std::string sub_pattern;

        if (p_child == std::string::npos) 				// no child
            sub_pattern = pattern;
        else
            sub_pattern = pattern.substr(0, p_child);	// up to '/'

        // expand glob at this directory level
        glob_t glob_files;
        int ret = xglob(sub_pattern, GLOB_NOESCAPE, NULL, &glob_files);
        if (ret == 0) {									// matched files
            for (int i = 0; i < static_cast<int>(glob_files.gl_pathc); i++) {
                sub_pattern = glob_files.gl_pathv[i];
                if (p_child != std::string::npos)
                    sub_pattern += pattern.substr(p_child);
                find_glob_1(files, sub_pattern);	// recurse
            }
        }
        globfree(&glob_files);
    }
}

std::vector<fs::path> FindGlob(const std::string& pattern) {
    std::vector<fs::path> files;
    find_glob_1(files, pattern);
    return files;
}

std::istream& SafeGetline(std::istream& is, std::string& t) {
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for (;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if (sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if (t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}