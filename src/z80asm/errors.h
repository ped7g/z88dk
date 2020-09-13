/*
Z88DK Z80 Macro Assembler

Copyright (C) Gunther Strube, InterLogic 1993-99
Copyright (C) Paulo Custodio, 2011-2020
License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0

Error handling.
*/

#pragma once

#include "error_func.h"
#include <stdio.h>

enum ErrType { ErrInfo, ErrWarn, ErrError };

/*-----------------------------------------------------------------------------
*	initialize error module
*----------------------------------------------------------------------------*/
extern void errors_init( void );

/*-----------------------------------------------------------------------------
*	define the next FILE, LINENO, MODULE to use in error messages
*	error_xxx(), warn_xxx()
*----------------------------------------------------------------------------*/
extern void set_error_null( void );             /* clear all locations */
extern void set_error_file(const char* filename );
extern void set_error_module(const char* modulename );
extern void set_error_line( int lineno );

extern const char* get_error_file(void);
extern int         get_error_line(void);

/*-----------------------------------------------------------------------------
*	reset count of errors and return current count
*----------------------------------------------------------------------------*/
extern void reset_error_count( void );
extern int  get_num_errors( void );

/*-----------------------------------------------------------------------------
*   Execute an error
*----------------------------------------------------------------------------*/
extern void do_error( enum ErrType err_type, char* message );
