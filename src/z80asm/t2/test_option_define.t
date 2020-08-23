#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

# Test -D

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

my $asm = "ld a,_value23";
my @bin = (0x3E, 1);

write_file("${test}.asm", $asm);

# no -D
run_nok("z80asm ${test}.asm", "", <<END);
Error at file '${test}.asm' line 1: symbol '_value23' not defined
END

# invalid -D
for my $option ('-D23', '-Da-', '-Daaa=', '-Daaa=a', '-Daaa=!', '-Daaa=1x') {
	run_nok("z80asm ${option} ${test}.asm", "", <<END);
Error: illegal option: ${option}
END
}

# -D
for my $equal ("", "=") {
	asm_ok($asm, "-D${equal}_value23", @bin);
}

# -Dvar=value
for my $equal ("", "=") {
	for my $value (255, "0xff", "0XFF", "0ffh", "0FFH", "\$FF") {
		asm_ok($asm, quote_os("-D${equal}_value23=${value}"), $bin[0], 255);
	}
}

# -D with environment variables
$ENV{TEST_ENV} = 'value';
asm_ok($asm, "-D_\${TEST_ENV}23", @bin);

$ENV{TEST_ENV} = '255';
asm_ok($asm, "-D_value23=\${TEST_ENV}", $bin[0], 255);

delete $ENV{TEST_ENV};
asm_ok($asm, "-D_value\${TEST_ENV}23", @bin);

end_test();
