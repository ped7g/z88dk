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
unlink "${test}.bin";
path("${test}.asm")->spew('include');
run_nok("z80asm ${test}.asm", "", <<END);
Error at file '${test}.asm' line 1: syntax error
END

# no -I, multiple levels
path("${test}0.inc")->spew('defb 0');
for (1..9) { 
	path("${test}$_.inc")->spew("include \"${test}".($_-1).".inc\"\ndefb $_"); 
}
unlink "${test}.bin";
asm_ok(<<END, "", 0..10);
	include "${test}9.inc"
	defb 10
END

# -I
mkdir("${test}_dir");
path("${test}_dir/${test}.inc")->spew('defb 11');

# no -I, full path : OK
unlink "${test}.bin";
asm_ok("include \"${test}_dir/${test}.inc\"", "", 11);

# no -I, only file name : error
unlink "${test}.inc";
path("${test}.asm")->spew("include \"${test}.inc\"");
run_nok("z80asm -b ${test}.asm", "", <<END);
Error at file '${test}.asm' line 1: cannot read file '${test}.inc'
END

# -I : OK
unlink "${test}.bin";
asm_ok("include \"${test}.inc\"", "-I${test}_dir", 11);

# test -I using environment variables
$ENV{TEST_ENV} = $test;
unlink "${test}.bin";
asm_ok("include \"${test}.inc\"", "-I\${TEST_ENV}_dir", 11);

delete $ENV{TEST_ENV};
unlink "${test}.bin";
asm_ok("include \"${test}.inc\"", "-I${test}\${TEST_ENV}_dir", 11);

# -I, full path : OK
unlink "${test}.bin";
asm_ok("include \"${test}_dir/${test}.inc\"", "-b -I${test}_dir", 11);

# directory of source file is added to include path
unlink <${test}_dir/*>;
path("${test}_dir/${test}.inc")->spew("defb 11");
path("${test}_dir/${test}.asm")->spew("include \"${test}.inc\"");
run_ok("z80asm -b ${test}_dir/${test}.asm", "", "");
ok path("${test}_dir/${test}.bin")->slurp_raw eq pack("C*", 11), "bin ok";

# error_read_file
# BUG_0034 : If assembly process fails with fatal error, invalid library is kept
unlink "${test}.bin", "${test}.lib", "${test}.inc";
path("${test}.asm")->spew("include \"${test}.inc\"");
run_nok("z80asm -x${test}.lib ${test}.asm}", "", <<END);
Error at file '${test}.asm' line 1: cannot read file '${test}.inc'
END
ok ! -f "${test}.lib", "${test}.lib does not exist";

# error_include_recursion
path("${test}.asm")->spew("include \"${test}.inc\"");
path("${test}.inc")->spew("include \"${test}.asm\"");
run_nok("z80asm ${test}.asm}", "", <<END);
Error at file '${test}.inc' line 1: cannot include file '${test}.asm' recursively
END

path("${test}_dir")->remove_tree;
end_test();
