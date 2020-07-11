#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#
# BUG_0016: RCM2000 and RCM3000 emulation routines not assembled when LIST is ON (-l)
#------------------------------------------------------------------------------

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

for my $list ("", "-l") {
	asm_ok("cpi", "-mr2k $list", 
			0xCD, 0x03, 0x00,
			0x38, 0x14, 0xBE, 0x23, 0x0B, 0xF5, 0xED, 0x54, 0xCB, 0x85, 0xCB, 0xD5, 
			0x78, 0xB1, 0x20, 0x02, 0xCB, 0x95, 0xED, 0x54, 0xF1, 0xC9, 0xBE, 0x23, 
			0x0B, 0xF5, 0xED, 0x54, 0xCB, 0xC5, 0x18, 0xEA);
}

end_test();
