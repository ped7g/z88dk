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
    return path_prepend_output_dir(path_replace_ext(filename, FILEEXT_LIST));
}

const char* get_def_filename(const char* filename ) {
    return path_prepend_output_dir(path_replace_ext(filename, FILEEXT_DEF));
}

const char* get_err_filename(const char* filename ) {
    return path_prepend_output_dir(path_replace_ext(filename, FILEEXT_ERR));
}

const char* get_bin_filename(const char* filename ) {
    return path_prepend_output_dir(path_replace_ext(filename, FILEEXT_BIN));
}

const char* get_lib_filename(const char* filename ) {
    return path_replace_ext(filename, FILEEXT_LIB);
}

const char* get_sym_filename(const char* filename ) {
    return path_prepend_output_dir(path_replace_ext(filename, FILEEXT_SYM));
}

const char* get_map_filename(const char* filename) {
    return path_prepend_output_dir(path_replace_ext(filename, FILEEXT_MAP));
}

const char* get_reloc_filename(const char* filename) {
    return path_prepend_output_dir(path_replace_ext(filename, FILEEXT_RELOC));
}

const char* get_asm_filename(const char* filename) {
    return path_replace_ext(filename, FILEEXT_ASM);
}

const char* get_obj_filename(const char* filename ) {
    return path_prepend_output_dir(path_replace_ext(filename, FILEEXT_OBJ));
}
