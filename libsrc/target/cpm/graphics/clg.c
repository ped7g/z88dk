/*
 *	CP/M GSX based graphics libraries
 *
 *	clg()
 *
 *	Stefano Bodrato - 2021
 *
 *	$Id: clg.c $
 */

#include <cpm.h>
#include <graphics.h>


/* Clear Graphics */

void clg()
{
	gios_open(1);
	gios_clg();
	gios_l_color(7);
}
