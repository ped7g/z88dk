#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

# Test -I option

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

path("${test}_dir")->remove_tree;

# syntax
write_file("${test}.asm", 'include');
run_nok("z80asm ${test}.asm", "", <<END);
Error at file '${test}.asm' line 1: syntax error
END

# no -I, multiple levels
write_file("${test}0.inc", 'defb 0');
for (1..9) { 
	write_file("${test}$_.inc", "include \"${test}".($_-1).".inc\"\ndefb $_"); 
}
asm_ok(<<END, "", 0..10);
	include "${test}9.inc"
	defb 10
END

# -I
unlink "${test}.inc";
mkdir("${test}_dir");
write_file("${test}_dir/${test}.inc", 'defb 11');

# no -I, full path : OK
asm_ok("include \"${test}_dir/${test}.inc\"", "", 11);

# no -I, only file name : error
write_file("${test}.asm", "include \"${test}.inc\"");
run_nok("z80asm -b ${test}.asm", "", <<END);
Error at file '${test}.asm' line 1: cannot read file '${test}.inc'
END

# -I : OK
for my $equal ("", "=") {
	asm_ok("include \"${test}.inc\"", "-I${equal}${test}_dir", 11);
}

# test -I using environment variables
$ENV{TEST_ENV} = $test;
asm_ok("include \"${test}.inc\"", "-I\${TEST_ENV}_dir", 11);

delete $ENV{TEST_ENV};
asm_ok("include \"${test}.inc\"", "-I${test}\${TEST_ENV}_dir", 11);

# -I, full path : OK
asm_ok("include \"${test}_dir/${test}.inc\"", "-b -I${test}_dir", 11);

# directory of source file is added to include path
unlink <${test}_dir/*>;
write_file("${test}_dir/${test}.inc", "defb 11");
write_file("${test}_dir/${test}.asm", "include \"${test}.inc\"");
run_ok("z80asm -b ${test}_dir/${test}.asm", "", "");
check_bin_file("${test}_dir/${test}.bin", 11);

# error_read_file
# BUG_0034 : If assembly process fails with fatal error, invalid library is kept
unlink "${test}.lib";
write_file("${test}.asm", "include \"${test}.inc\"");
run_nok("z80asm -x${test}.lib ${test}.asm}", "", <<END);
Error at file '${test}.asm' line 1: cannot read file '${test}.inc'
END
ok ! -f "${test}.lib", "${test}.lib does not exist";

# error_include_recursion
write_file("${test}.asm", "include \"${test}1.inc\"");
write_file("${test}1.inc", "include \"${test}2.inc\"");
write_file("${test}2.inc", "include \"${test}1.inc\"");
run_nok("z80asm ${test}.asm}", "", <<END);
Error at file '${test}2.inc' line 1: cannot include file '${test}1.inc' recursively
END

path("${test}_dir")->remove_tree;
end_test();
