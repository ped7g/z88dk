#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#
# BUG_0002 : CreateLibFile and GetLibFile: buffer overrun
#------------------------------------------------------------------------------

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

unlink_testfiles();
write_file("${test}.asm", <<END);
		PUBLIC one
	one: 
		ld a,1
		ret
END
run_ok("z80asm -x${test}.lib ${test}.asm");
ok -f "${test}.lib", "${test}.lib created";

asm_ok(<<END, "-l${test}.lib", 0xC3, 0x03, 0x00, 0x3E, 0x01, 0xC9);
		EXTERN one
		jp one
END

end_test();
