#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

my $asm = <<END;
	ld ix,0x1234
	ld iy,0x1234
	if __SWAP_IX_IY__
	ret
	endif
END

asm_ok($asm, "      ", 0xDD, 0x21, 0x34, 0x12, 0xFD, 0x21, 0x34, 0x12);
asm_ok($asm, " -IXIY", 0xFD, 0x21, 0x34, 0x12, 0xDD, 0x21, 0x34, 0x12, 0xC9);

end_test();
