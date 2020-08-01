#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

# Test -D, --define

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

my $asm = "ld a,_value23";
my @bin = (0x3E, 1);

path("${test}.asm")->spew($asm);

# no -D
run_nok("z80asm ${test}.asm", "", <<END);
Error at file '${test}.asm' line 1: symbol '_value23' not defined
END

# invalid -D
for my $option ('-D23', '-Da-') {
	run_nok("z80asm ${option} ${test}.asm", "", <<END);
Error: illegal option: ${option}
END
}

# -D
for my $option ('-D', '-D=', '--define', '--define=') {
	asm_ok($asm, "${option}_value23", @bin);
}

# -D with environment variables
$ENV{TEST_ENV} = 'value';
asm_ok($asm, "-D=_\${TEST_ENV}23", @bin);

delete $ENV{TEST_ENV};
asm_ok($asm, "-D=_value\${TEST_ENV}23", @bin);

end_test();
