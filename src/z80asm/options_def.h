/*
Z88DK Z80 Macro Assembler

Copyright (C) Gunther Strube, InterLogic 1993-99
Copyright (C) Paulo Custodio, 2011-2020
License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0

Define command line options
*/

#ifndef OPT_VAR
#define OPT_VAR(type, name, default)
#endif

OPT_VAR(argv_t*,	files,	  NULL)			/* list of input files */

#undef OPT_VAR
