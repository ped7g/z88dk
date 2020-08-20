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

OPT_VAR(appmake_t, appmake, APPMAKE_NONE)
OPT_VAR(const char*, appmake_opts, "")
OPT_VAR(const char*, appmake_ext, "")
OPT_VAR(int, appmake_origin_min, -1)
OPT_VAR(int, appmake_origin_max, -1)


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
OPT(OptCallArg, option_origin, "-r", "", "Relocate binary file to given address (decimal or hex)",
    "ADDR")
OPT(OptCallArg, option_filler, "-f", "", "Default value to fill in DEFS (decimal or hex)", "BYTE")

OPT_TITLE("Appmake Options:")
OPT(OptCall, option_appmake_zx81, "", "+zx81", "Generate ZX81 .P file, origin at " ZX81_ORIGIN_S,
    "")
OPT(OptCall, option_appmake_zx, "", "+zx", "Generate ZX Spectrum .tap file, origin defaults to\n"
    "                         " ZX_ORIGIN_S " (in a REM), but can be set with -rORG >= 24000\n"
    "                         for above RAMTOP", "")

/*-----------------------------------------------------------------------------
*   clear macros
*----------------------------------------------------------------------------*/
#undef OPT_VAR
#undef OPT_TITLE
#undef OPT
