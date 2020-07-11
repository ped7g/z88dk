#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

# z80asm: Ti83 and Ti83 apps can't be compiled

use Modern::Perl;
use Config;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();
my $cmd;

asm_ok(<<END, "", 1, 2);
		defc    Lo      = 1
		defc    Lz      = 2
		defb 	Lo, Lz
END

for my $cpu (qw( r2k r3k )) {
	asm_ok(<<END, "-m$cpu", 0xE2, 0,0, 0xEA, 0,0);
		jp Lz, 0
		jp Lo, 0
END
}

end_test();
