//-----------------------------------------------------------------------------
// Z88DK Z80 Module Assembler
// Parse command line options
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "legacy.h"

#include "config.h"
#include "portability.h"
#include "die.h"
#include "errors.h"
#include "fileutil.h"
#include "init.h"
#include "model.h"
#include "modlink.h"
#include "options_c.h"
#include "srcfile.h"
#include "str.h"
#include "strutil.h"
#include "symtab.h"
#include "utarray.h"
#include "utstring.h"
#include "z80asm.h"
#include "zutils.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

/* default file name extensions */
#define FILEEXT_ASM     ".asm"
#define FILEEXT_LIST    ".lis"
#define FILEEXT_OBJ     ".o"
#define FILEEXT_DEF     ".def"
#define FILEEXT_ERR     ".err"
#define FILEEXT_BIN     ".bin"
#define FILEEXT_LIB     ".lib"
#define FILEEXT_SYM     ".sym"
#define FILEEXT_MAP     ".map"
#define FILEEXT_RELOC   ".reloc"

/* types */
enum OptType {
    OptSet,
    OptCall, OptCallArg,
    OptString, OptStringList,
};

/* declare functions */
static void expand_source_glob(const char* pattern);
static void expand_list_glob(const char* filename);

/*-----------------------------------------------------------------------------
*   singleton opts
*----------------------------------------------------------------------------*/
#define OPT_VAR(type, name, default)	default,
Opts opts = {
#include "options_def.h"
};

/*-----------------------------------------------------------------------------
*   Initialize module
*----------------------------------------------------------------------------*/
DEFINE_init_module() {
    opts.files = argv_new();
}

DEFINE_dtor_module() {
    argv_free(opts.files);
}

/*-----------------------------------------------------------------------------
*   process a file
*----------------------------------------------------------------------------*/

/* search for the first file in path, with the given extension,
* with .asm extension and with .o extension
* if not found, return original file */
static const char* search_source(const char* filename) {
    const char* f;

    if (file_exists(filename))
        return filename;

    f = SearchIncludeFile(filename);
    if (*f != '\0' && file_exists(f))
        return f;

    f = get_asm_filename(filename);
    if (file_exists(f))
        return f;

    f = SearchIncludeFile(f);
    if (*f != '\0' && file_exists(f))
        return f;

    f = get_obj_filename(filename);
    if (file_exists(f))
        return f;

    f = SearchIncludeFile(f);
    if (*f != '\0' && file_exists(f))
        return f;

    error_read_file(filename);
    return filename;
}

static void process_file_1(char* filename) {
    strstrip(filename);
    switch (filename[0]) {
    case '\0':		/* no file */
        break;

    case '@':		/* file list */
        filename++;						/* point to after '@' */
        strstrip(filename);
        expand_list_glob(ExpandEnvironmentVarsC(filename));
        break;
    case ';':     /* comment */
    case '#':
        break;
    default:
        expand_source_glob(ExpandEnvironmentVarsC(filename));
    }
}

void process_file(const char* filename) {
    init_module();
    char* buffer = xstrdup(filename);	// calleee need to modify string
    process_file_1(buffer);
    xfree(buffer);
}

static void expand_source_glob(const char* pattern) {
    if (strpbrk(pattern, "*?") != NULL) {		// is a pattern
        argv_t* files = path_find_glob(pattern);

        if (argv_len(files) == 0)
            error_glob_no_files(pattern);		// error if pattern matched no file

        for (char** p = argv_front(files); *p; p++)
            argv_push(opts.files, search_source(*p));

        argv_free(files);
    }
    else
        argv_push(opts.files, search_source(pattern));
}

static void expand_list_glob(const char* filename) {
    if (strpbrk(filename, "*?") != NULL) {		// is a pattern
        argv_t* files = path_find_glob(filename);

        if (argv_len(files) == 0)
            error_glob_no_files(filename);		// error if pattern matched no file

        for (char** p = argv_front(files); *p; p++) {
            char* filename = *p;
            src_push();
            {
                char* line;

                // append the directoy of the list file to the include path	and remove it at the end
                PushSourceDirname(filename);

                if (src_open(filename, false)) {
                    while ((line = src_getline()) != NULL)
                        process_file(line);
                }

                // finished assembly, remove dirname from include path
                PopSourceDirname();
            }
            src_pop();
        }
        argv_free(files);
    }
    else {
        src_push();
        {
            char* line;

            // append the directoy of the list file to the include path	and remove it at the end
            PushSourceDirname(filename);

            if (src_open(filename, false)) {
                while ((line = src_getline()) != NULL)
                    process_file(line);
            }

            // finished assembly, remove dirname from include path
            PopSourceDirname();
        }
        src_pop();
    }
}

/*-----------------------------------------------------------------------------
*
*----------------------------------------------------------------------------*/
static void def_sym(const char* name, int value) {
    define_static_def_sym(name, value);
}

void define_assembly_defines(void) {
    init_module();
    TraverseDefines(def_sym);
    if (SwapIxIy())
        define_static_def_sym(SWAP_IXIY_DEFINE, 1);
}

bool files_empty(void) {
    init_module();
    return argv_len(opts.files) == 0;
}

/*-----------------------------------------------------------------------------
*   Change extension of given file name, return pointer to file name in
*	strpool
*	Extensions may be changed by options.
*----------------------------------------------------------------------------*/
static const char* path_prepend_output_dir(const char* filename) {
    char path[FILENAME_MAX];
    if (GetOutputDirectory()) {
        if (isalpha(filename[0]) && filename[1] == ':')	// it's a win32 absolute path
            snprintf(path, sizeof(path), "%s/%c/%s",
                     GetOutputDirectory(), filename[0], filename + 2);
        else
            snprintf(path, sizeof(path), "%s/%s",
                     GetOutputDirectory(), filename);
        return spool_add(path);
    }
    else
        return filename;
}

const char* get_list_filename(const char* filename ) {
    init_module();
    return path_prepend_output_dir(path_replace_ext(filename, FILEEXT_LIST));
}

const char* get_def_filename(const char* filename ) {
    init_module();
    return path_prepend_output_dir(path_replace_ext(filename, FILEEXT_DEF));
}

const char* get_err_filename(const char* filename ) {
    init_module();
    return path_prepend_output_dir(path_replace_ext(filename, FILEEXT_ERR));
}

const char* get_bin_filename(const char* filename ) {
    init_module();
    return path_prepend_output_dir(path_replace_ext(filename, FILEEXT_BIN));
}

const char* get_lib_filename(const char* filename ) {
    init_module();
    return path_replace_ext(filename, FILEEXT_LIB);
}

const char* get_sym_filename(const char* filename ) {
    init_module();
    return path_prepend_output_dir(path_replace_ext(filename, FILEEXT_SYM));
}

const char* get_map_filename(const char* filename) {
    init_module();
    return path_prepend_output_dir(path_replace_ext(filename, FILEEXT_MAP));
}

const char* get_reloc_filename(const char* filename) {
    init_module();
    return path_prepend_output_dir(path_replace_ext(filename, FILEEXT_RELOC));
}

const char* get_asm_filename(const char* filename) {
    return path_replace_ext(filename, FILEEXT_ASM);
}

const char* get_obj_filename(const char* filename ) {
    return path_prepend_output_dir(path_replace_ext(filename, FILEEXT_OBJ));
}
