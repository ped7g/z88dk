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
static void define_assembly_defines();
static void option_filler() {}

static void process_options( int* parg, int argc, char* argv[] );
static void process_files( int arg, int argc, char* argv[] );
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
*   lookup-table for all options
*----------------------------------------------------------------------------*/
typedef struct OptsLU {
    enum OptType	 type;		/* type of option */
    void*			arg;		/* option argument */
    char*			short_opt;	/* option text, including starting "-" */
    char*			long_opt;	/* option text, including starting "--" */
}
OptsLU;

#define OPT(type, arg, short_opt, long_opt, help_text, help_arg) \
		  { type, arg, short_opt, long_opt },

static OptsLU opts_lu[] = {
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
*   Parse command line, set options, including opts.files with list of
*	input files, including parsing of '@' lists
*----------------------------------------------------------------------------*/
static void process_env_options() {
    const char* opts = GetEnvPendingOptions();
    if (!opts || *opts == '\0')
        return;

    // add dummy program name option
    argv_t* args = argv_new();
    argv_push(args, "z80asm");

    char* opts_copy = xstrdup(opts);		// strtok needs writeable string
    char* tok = strtok(opts_copy, " ");
    while (tok) {
        argv_push(args, tok);
        tok = strtok(NULL, " ");
    }

    // parse args
    int argc = argv_len(args);
    int arg;
    process_options(&arg, argc, argv_front(args));

    xfree(opts_copy);
    argv_free(args);
}

void parse_argv( int argc, char* argv[] ) {
    int arg = 0;

    init_module();

    if (!get_num_errors())
        process_env_options();				/* process options from Z80ASM environment variable */

    if (!get_num_errors())
        process_options( &arg, argc, argv );/* process all options, set arg to next */

    if (!get_num_errors() && arg >= argc)
        error_no_src_file();				/* no source file */

    if ( ! get_num_errors() )
        process_files( arg, argc, argv );	/* process each source file */

    define_assembly_defines();				/* defined options-dependent constants */
}

/*-----------------------------------------------------------------------------
*   process all options
*----------------------------------------------------------------------------*/
/* check if this option is matched, return char pointer after option, ready
   to retrieve an argument, if any */
static char* check_option( char* arg, char* opt ) {
    size_t len = strlen( opt );

    if ( *opt &&				/* ignore empty option strings */
            strncmp( arg, opt, len ) == 0 ) {
        if ( arg[len] == '=' )
            len++;				/* skip '=' after option, to point at argument */

        return arg + len;		/* point to after argument */
    }
    else
        return NULL;			/* not found */
}

static void process_opt(int* parg, int argc, char* argv[]) {
#define II (*parg)
    int		 j;
    const char* opt_arg_ptr;

    /* search opts_lu[] */
    for (j = 0; j < NUM_ELEMS(opts_lu); j++) {
        if ((opt_arg_ptr = check_option(argv[II], opts_lu[j].long_opt)) != NULL ||
                (opt_arg_ptr = check_option(argv[II], opts_lu[j].short_opt)) != NULL) {
            /* found option, opt_arg_ptr points to after option */
            switch (opts_lu[j].type) {
            case OptSet:
                if (*opt_arg_ptr)
                    error_illegal_option(argv[II]);
                else
                    *((bool*)(opts_lu[j].arg)) = true;

                break;

            case OptCall:
                if (*opt_arg_ptr)
                    error_illegal_option(argv[II]);
                else
                    ((void(*)(void))(opts_lu[j].arg))();

                break;

            case OptCallArg:
                if (*opt_arg_ptr) {
                    opt_arg_ptr = ExpandEnvironmentVarsC(opt_arg_ptr);
                    ((void(*)(const char*))(opts_lu[j].arg))(opt_arg_ptr);
                }
                else
                    error_illegal_option(argv[II]);

                break;

            case OptString:
                if (*opt_arg_ptr) {
                    opt_arg_ptr = ExpandEnvironmentVarsC(opt_arg_ptr);
                    *((const char**)(opts_lu[j].arg)) = opt_arg_ptr;
                }
                else
                    error_illegal_option(argv[II]);

                break;

            case OptStringList:
                if (*opt_arg_ptr) {
                    UT_array** p_path = (UT_array**)opts_lu[j].arg;
                    opt_arg_ptr = ExpandEnvironmentVarsC(opt_arg_ptr);
                    argv_push(*p_path, opt_arg_ptr);
                }
                else
                    error_illegal_option(argv[II]);

                break;

            default:
                xassert(0);
            }

            return;
        }
    }

    /* not found */
    error_illegal_option(argv[II]);

#undef II
}

static void process_options( int* parg, int argc, char* argv[] ) {
#define II (*parg)
    for (II = 1; II < argc; II++) {
        if (argv[II][0] == '\0') {
            // ignore empty args
        }
        else if (strcmp(argv[II], "--") == 0) {
            // end of options
            II++;
            break;
        }
        else if (argv[II][0] != '-' && argv[II][0] != '+') {
            // end of options
            break;
        }
        else
            process_opt(&II, argc, argv);
    }
#undef II
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

static void process_file(char* filename ) {
    strstrip(filename);
    switch (filename[0]) {
    case '\0':		/* no file */
        break;

    case '@':		/* file list */
        filename++;						/* point to after '@' */
        strstrip(filename);
        filename = (char*)ExpandEnvironmentVarsC(filename);
        expand_list_glob(filename);
        break;
    case ';':     /* comment */
    case '#':
        break;
    default:
        filename = (char*)ExpandEnvironmentVarsC(filename);
        expand_source_glob(filename);
    }
}

void expand_source_glob(const char* pattern) {
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

void expand_list_glob(const char* filename) {
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
*   process all files
*----------------------------------------------------------------------------*/
static void process_files( int arg, int argc, char* argv[] ) {
    int i;

    /* Assemble file list */
    for ( i = arg; i < argc; i++ )
        process_file( argv[i] );
}

/*-----------------------------------------------------------------------------
*   Option functions called from Opts table
*----------------------------------------------------------------------------*/
int number_arg(const char* arg) {
    char* end;
    const char* p = arg;
    long lval;
    int radix;
    char suffix = '\0';

    if (p[0] == '\0') 		// empty
        return -1;
    else if (p[0] == '$') {
        p++;
        radix = 16;
    }
    else if (p[0] == '0' && tolower(p[1]) == 'x') {
        p += 2;
        radix = 16;
    }
    else if (isdigit(p[0]) && tolower(p[strlen(p) - 1]) == 'h') {
        suffix = p[strlen(p) - 1];
        radix = 16;
    }
    else
        radix = 10;

    lval = strtol(p, &end, radix);
    if (*end != suffix || errno == ERANGE || lval < 0 || lval > INT_MAX)
        return -1;
    else
        return (int)lval;
}

static void def_sym(const char* name, int value) {
    define_static_def_sym(name, value);
}

static void define_assembly_defines() {
    TraverseDefines(def_sym);
    if (SwapIxIy())
        define_static_def_sym(SWAP_IXIY_DEFINE, 1);
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
