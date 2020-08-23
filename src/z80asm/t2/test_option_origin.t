#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

# Test -rORIGIN

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

# -rORIGIN
for my $equal ("", "=") {
	for my $origin (0, 0x1234) {
		my $origin_hex = sprintf("%x", $origin);
		for my $origin_text ($origin, 
							 "0x${origin_hex}", "0X${origin_hex}", 
							 "0${origin_hex}h", "0${origin_hex}H", 
							 "\$${origin_hex}") {
			asm_ok("start: jp start", "-b ".quote_os("-r${equal}${origin_text}"), 
					0xC3, $origin & 0xFF, ($origin >> 8) & 0xFF);
		}
	}
}

# out of range
write_file("$test.asm", "start: jp start");
run_nok("z80asm -b -r0x10000 $test.asm", "", <<END);
Error: integer '65536' out of range
END

end_test();
