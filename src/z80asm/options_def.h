/*
Z88DK Z80 Macro Assembler

Copyright (C) Gunther Strube, InterLogic 1993-99
Copyright (C) Paulo Custodio, 2011-2020
License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0

Define command line options
*/

/*-----------------------------------------------------------------------------
*   define option variables
*----------------------------------------------------------------------------*/
#ifndef OPT_VAR
#define OPT_VAR(type, name, default)
#endif

OPT_VAR( bool,		symtable,	false	)
OPT_VAR( bool,		list,		false	)	/* -l flag */
OPT_VAR( bool,		cur_list,	false	)	/* current LSTON/LSTOFF status */
OPT_VAR( bool,		globaldef,	false	)
OPT_VAR( bool,		make_bin,	false	)
OPT_VAR( bool,		split_bin,	false   )	/* true to split binary file per section */
OPT_VAR( bool,		date_stamp,	false	)
OPT_VAR( bool,		relocatable, false	)
OPT_VAR( bool,      reloc_info, false   )	/* generate .reloc file */

OPT_VAR(appmake_t, appmake, APPMAKE_NONE)
OPT_VAR(const char *, appmake_opts, "")
OPT_VAR(const char *, appmake_ext, "")
OPT_VAR(int, appmake_origin_min, -1)
OPT_VAR(int, appmake_origin_max, -1)

OPT_VAR(const char *,	bin_file,	NULL	)	/* set by -o */
OPT_VAR(const char *,	lib_file,	NULL	)	/* set by -x */
OPT_VAR(const char *,    consol_obj_file, NULL)	/* set by -o and no -b */
OPT_VAR(const char *,    output_directory, NULL)	/* set by -O */

OPT_VAR(argv_t *,	files,	  NULL)			/* list of input files */

OPT_VAR(int,		filler,		0)			/* filler byte for defs */

/*-----------------------------------------------------------------------------
*   define options
*----------------------------------------------------------------------------*/
#ifndef OPT_TITLE
#define OPT_TITLE(text)
#endif

#ifndef OPT
#define OPT(type, arg, short_opt, long_opt, help_text, help_arg)
#endif

OPT_TITLE("Libraries:")
OPT(OptCallArg, option_make_lib, "-x", "", "Create a library file" FILEEXT_LIB, "FILE")
OPT(OptCallArg, option_use_lib, "-l", "", "Use library file" FILEEXT_LIB, "FILE")

OPT_TITLE("Binary Output:")
OPT(OptString, (void*)&opts.output_directory, "-O", "", "Output directory", "DIR")
OPT(OptString, (void*)&opts.bin_file, "-o", "", "Output binary file", "FILE")
OPT(OptSet, &opts.make_bin, "-b", "", "Assemble and link/relocate to file" FILEEXT_BIN, "")
OPT(OptSet, &opts.split_bin, "-split-bin", "", "Create one binary file per section", "")
OPT(OptSet, &opts.date_stamp, "-d", "", "Assemble only updated files", "")
OPT(OptCallArg, option_origin, "-r", "", "Relocate binary file to given address (decimal or hex)", "ADDR")
OPT(OptSet, &opts.relocatable, "-R", "", "Create relocatable code", "")
OPT(OptSet, &opts.reloc_info, "-reloc-info", "", "Geneate binary file relocation information", "")
OPT(OptCallArg, option_filler, "-f", "", "Default value to fill in DEFS (decimal or hex)", "BYTE")

OPT_TITLE("Output File Options:")
OPT(OptSet, &opts.symtable, "-s", "", "Create symbol table file" FILEEXT_SYM, "")
OPT(OptSet, &opts.list, "-l", "", "Create listing file" FILEEXT_LIST, "")
OPT(OptSet, &opts.globaldef, "-g", "", "Create global definition file" FILEEXT_DEF, "")

OPT_TITLE("Appmake Options:")
OPT(OptCall, option_appmake_zx81, "", "+zx81", "Generate ZX81 .P file, origin at " ZX81_ORIGIN_S, "")
OPT(OptCall, option_appmake_zx, "", "+zx", "Generate ZX Spectrum .tap file, origin defaults to\n"
	"                         " ZX_ORIGIN_S " (in a REM), but can be set with -rORG >= 24000\n"
	"                         for above RAMTOP", "")

/*-----------------------------------------------------------------------------
*   clear macros
*----------------------------------------------------------------------------*/
#undef OPT_VAR
#undef OPT_TITLE
#undef OPT
