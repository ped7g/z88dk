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

OPT_VAR(argv_t*,	files,	  NULL)			/* list of input files */

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

OPT_TITLE("Binary Output:")
OPT(OptCallArg, option_filler, "-f", "", "Default value to fill in DEFS (decimal or hex)", "BYTE")


/*-----------------------------------------------------------------------------
*   clear macros
*----------------------------------------------------------------------------*/
#undef OPT_VAR
#undef OPT_TITLE
#undef OPT
