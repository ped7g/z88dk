#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#
# CH_0012 : wrappers on OS calls to raise fatal error
#------------------------------------------------------------------------------

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

unlink_testfiles();
run_nok("z80asm ${test}.asm", "", <<END);
Error: cannot read file '${test}.asm'
END

unlink_testfiles();
write_file("${test}.asm", <<END);
	INCLUDE "${test}.inc"
END
run_nok("z80asm ${test}.asm", "", <<END);
Error at file '${test}.asm' line 1: cannot read file '${test}.inc'
END

write_file("${test}.asm", <<END);
	BINARY "${test}.inc"
END
run_nok("z80asm ${test}.asm", "", <<END);
Error at file '${test}.asm' line 1: cannot read file '${test}.inc'
END

unlink_testfiles();
write_file("${test}.asm", <<END);
	nop
END
run_nok("z80asm -l${test}.lib ${test}.asm", "", <<END);
Error: cannot read file '${test}.lib'
END

end_test();
