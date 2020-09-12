//-----------------------------------------------------------------------------
// Z88DK Z80 Module Assembler
// Parse command line options
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#pragma once

#include "legacy.h"

#include "strutil.h"
#include "types.h"
#include "utarray.h"
#include "utstring.h"

/*-----------------------------------------------------------------------------
*   Change extension of given file name, return pointer to file name in
*	strpool
*	Extensions may be changed by options.
*----------------------------------------------------------------------------*/
extern const char* get_asm_filename(const char* filename );
extern const char* get_list_filename(const char* filename );
extern const char* get_obj_filename(const char* filename );
extern const char* get_def_filename(const char* filename );
extern const char* get_err_filename(const char* filename );
extern const char* get_bin_filename(const char* filename );
extern const char* get_lib_filename(const char* filename );
extern const char* get_sym_filename(const char* filename );
extern const char* get_map_filename(const char* filename);
extern const char* get_reloc_filename(const char* filename);
