#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#
# BUG_0014: -x./zx_clib should create ./zx_clib.lib but actually creates .lib
#------------------------------------------------------------------------------

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

for my $lib (      "${test}",    "${test}.lib",
				 "./${test}",  "./${test}.lib",
				".\\${test}", ".\\${test}.lib" ) {
	next if ($lib =~ /\\/ && $^O !~ /MSWin32/);

    unlink("${test}.lib");
    ok ! -f "${test}.lib", "${test}.lib deleted, building $lib";
	
	write_file("${test}.asm", <<END);
			PUBLIC main
		main: 
			ret
END
	run_ok("z80asm -x${test}.lib ${test}.asm");
    ok -f "${test}.lib", "${test}.lib exists, built $lib";

	asm_ok(<<END, "-l${test}.lib", 0xC3, 0x03, 0x00, 0xC9);
		EXTERN main
		jp main
END
}

end_test();
