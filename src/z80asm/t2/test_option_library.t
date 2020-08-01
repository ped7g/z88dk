#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

# Test -L, --lib-path

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

path("${test}_dir")->remove_tree;
mkdir("${test}_dir");

# create library
ok ! -f "${test}_dir/${test}.lib", "no library";
path("${test}.asm")->spew("PUBLIC main \n main: \n nop \n ret ");
run_ok("z80asm -x${test}_dir/${test}.lib ${test}.asm", "", "");
ok -f "${test}_dir/${test}.lib", "library created";

my $asm = "EXTERN main \n call main \n ret";
my @bin = (0xCD, 0x04, 0x00, 0xC9, 0x00, 0xC9);

# no -L, full path : OK
asm_ok($asm, "-i${test}_dir/${test}.lib", @bin);
asm_ok($asm, "-i${test}_dir/${test}    ", @bin);

# no -L, only file name : error
path("${test}.asm")->spew($asm);
run_nok("z80asm -i${test}.lib ${test}.asm", "", <<END);
Error: cannot read file '${test}.lib'
END

run_nok("z80asm -i${test}     ${test}.asm", "", <<END);
Error: cannot read file '${test}.lib'
END

# -L : OK
for my $option ('-L', '-L=', '--lib-path', '--lib-path=') {
	asm_ok($asm, "${option}${test}_dir -i${test}.lib", @bin);
	asm_ok($asm, "${option}${test}_dir -i${test}    ", @bin);
}

# use environment variable in -L
$ENV{TEST_ENV} = 'dir';
asm_ok($asm, "-L${test}_\${TEST_ENV} -i${test}.lib", @bin);
asm_ok($asm, "-L${test}_\${TEST_ENV} -i${test}    ", @bin);

delete $ENV{TEST_ENV};
asm_ok($asm, "-L${test}_\${TEST_ENV}dir -i${test}.lib", @bin);
asm_ok($asm, "-L${test}_\${TEST_ENV}dir -i${test}    ", @bin);

path("${test}_dir")->remove_tree;
end_test();
