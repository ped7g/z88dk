#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

# Test -L

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

path("${test}_dir")->remove_tree;
mkdir("${test}_dir");

# create library
for my $equal ("", "=") {
    unlink "${test}_dir/${test}.lib";
    write_file("${test}.asm", "PUBLIC main \n main: \n nop \n ret ");
    run_ok("z80asm -x${equal}${test}_dir/${test}.lib ${test}.asm");
    ok -f "${test}_dir/${test}.lib", "library created";
}

my $asm = "EXTERN main \n call main \n ret";
my @bin = (0xCD, 0x04, 0x00, 0xC9, 0x00, 0xC9);

# no -L, full path : OK
asm_ok($asm, "-l${test}_dir/${test}.lib", @bin);
asm_ok($asm, "-l${test}_dir/${test}    ", @bin);

# no -L, only file name : error
write_file("${test}.asm", $asm);
run_nok("z80asm -l${test}.lib ${test}.asm", "", <<END);
Error: cannot read file '${test}.lib'
END

run_nok("z80asm -l${test}     ${test}.asm", "", <<END);
Error: cannot read file '${test}.lib'
END

# -L : OK
for my $equal ("", "=") {
    for my $ext ("", ".lib") {
        asm_ok($asm, "-L${equal}${test}_dir -l${equal}${test}${ext}", @bin);
    }
}

# use environment variable in -L
$ENV{TEST_ENV} = 'dir';
asm_ok($asm, "-L${test}_\${TEST_ENV} -l${test}.lib", @bin);
asm_ok($asm, "-L${test}_\${TEST_ENV} -l${test}    ", @bin);

delete $ENV{TEST_ENV};
asm_ok($asm, "-L${test}_\${TEST_ENV}dir -l${test}.lib", @bin);
asm_ok($asm, "-L${test}_\${TEST_ENV}dir -l${test}    ", @bin);

path("${test}_dir")->remove_tree;

# test -l, -x

# create a library
my $lib = "${test}_lib.lib";

write_file("${test}.asm", <<END);
	PUBLIC one
one: 
	ld a,1
	ret
END

unlink $lib, "${test}.o";
run_ok("z80asm -x$lib ${test}.asm");
ok -f "${test}.o", "${test}.o created";
ok -f $lib, "$lib created";

# link with the library
asm_ok(<<END, "-l$lib", 0xC3, 0x03, 0x00, 0x3E, 0x01, 0xC9);
	EXTERN one
	jp one
END

unlink $lib;

# test BUG_0038: library modules not loaded in sequence
# obj1 requires libobj7 and obj3;
# obj2 requires libobj6 and obj3;
# obj3 requires libobj5 and obj1;
# libobj5 requires libobj6;
# libobj6 requires libobj7;
# libobj7 requires libobj5;

write_file("${test}.asm", <<END);
        PUBLIC A1
        EXTERN A3
        EXTERN A7
    A1: defb 1,A7,A3    ; A1 at addr 0, len 3
END

write_file("${test}2.asm", <<END);
        PUBLIC A2
        EXTERN A3
        EXTERN A6
    A2: defb 2,A6,A3    ; A2 at addr 3, len 3
END

write_file("${test}3.asm", <<END);
        PUBLIC A3
        EXTERN A1
        EXTERN A5
    A3: defb 3,A5,A1    ; A3 at addr 6, len 3
END

write_file("${test}5.asm", <<END);
        PUBLIC A5
        EXTERN A6
    A5: defb 5,A6       ; A5 at addr 9, len 2
END

write_file("${test}6.asm", <<END);
        PUBLIC A6 
        EXTERN A7 
    A6: defb 6,A7       ; A6 at addr 11, len 2
END

write_file("${test}7.asm", <<END);
        PUBLIC A7
        EXTERN A5
    A7: defb 7,A5       ; A7 at addr 13, len 2
END

unlink "${test}5.lib";
run_ok("z80asm -x${test}5.lib ${test}5.asm");
ok -f "${test}5.lib", "${test}5.lib created";

unlink "${test}6.lib";
run_ok("z80asm -x${test}6.lib ${test}6.asm");
ok -f "${test}6.lib", "${test}6.lib created";

unlink "${test}7.lib";
run_ok("z80asm -x${test}7.lib ${test}7.asm");
ok -f "${test}7.lib", "${test}7.lib created";


unlink "${test}.bin";
run_ok("z80asm -l -m -b -l${test}5.lib -l${test}6.lib -l${test}7.lib ".
        "${test}.asm ${test}2.asm ${test}3.asm");
check_bin_file("${test}.bin",
				1, 13, 6,
				2, 11, 6,
				3,  9, 0,
				5, 11, 
				6, 13,
				7,  9);


write_file("${test}.asm", <<END);
        EXTERN abs 
        call abs
END

write_file("${test}2.asm", <<END);
        PUBLIC abs
    abs: 
        ld a,1
        ret
END

write_file("${test}5.asm", <<END);
        PUBLIC abs
    abs: 
        ld a,2
        ret
END

unlink "${test}5.lib";
run_ok("z80asm -x${test}5.lib ${test}5.asm");
ok -f "${test}5.lib", "${test}5.lib created";


unlink "${test}.bin";
run_ok("z80asm -l -m -b -l${test}5.lib ${test}.asm ${test}2.asm");
check_bin_file("${test}.bin",
				0xCD, 3, 0,
				0x3E, 1,
				0xC9);

unlink "${test}.bin";
run_ok("z80asm -l -m -b -l${test}5.lib ${test}.asm");
check_bin_file("${test}.bin",
				0xCD, 3, 0,
				0x3E, 2,
				0xC9);


# test BUG_0039: library not pulled in if PUBLIC symbol not referenced in expression
write_file("${test}.asm", <<END);
        EXTERN A51 
        defb A51
END

write_file("${test}5.asm", <<END);
        PUBLIC A5, A51
    A5: 
        defc A51 = 51
END


unlink "${test}5.lib";
run_ok("z80asm -x${test}5.lib ${test}5.asm");
ok -f "${test}5.lib", "${test}5.lib created";


unlink "${test}.bin";
run_ok("z80asm -l -m -b -l${test}5.lib ${test}.asm");
check_bin_file("${test}.bin", 51);


# link objects and libs
# library modules are loaded in alpha-sequence of labels, starting at 10

write_file("${test}1.asm", <<END);
        PUBLIC A1
    A1: defb 1
END

write_file("${test}2.asm", <<END);
        PUBLIC A2
    A2: defb 2
END

write_file("${test}3.asm", <<END);
        PUBLIC A3
    A3: defb 3
END

unlink "${test}1.lib";
run_ok("z80asm -x${test}1.lib ${test}1.asm ${test}2.asm ${test}3.asm");
ok -f "${test}1.lib", "${test}1.lib created";

write_file("${test}4.asm", <<END);
        PUBLIC A4
    A4: defb 4
END

write_file("${test}5.asm", <<END);
        PUBLIC A5
    A5: defb 5
END

write_file("${test}6.asm", <<END);
        PUBLIC A6
    A6: defb 6
END

unlink "${test}2.lib";
run_ok("z80asm -x${test}2.lib ${test}4.asm ${test}5.asm ${test}6.asm");
ok -f "${test}2.lib", "${test}2.lib created";

write_file("${test}.asm", <<END);
    A0:
		EXTERN A1,A2,A3,A4,A5,A6
        EXTERN A7,A8,A9
        defb A1,A2,A3,A4,A5,A6,A7,A8,A9
        defb 0
END

write_file("${test}7.asm", <<END);
        PUBLIC A7
    A7: defb 7
END

write_file("${test}8.asm", <<END);
        PUBLIC A8
    A8: defb 8
END

write_file("${test}9.asm", <<END);
        PUBLIC A9
    A9: defb 9
END


unlink "${test}.bin";
run_ok("z80asm -l -m -b -l${test}1.lib -l${test}2.lib ".
            "${test}.asm ${test}7.asm ${test}8.asm ${test}9.asm");
check_bin_file("${test}.bin",
				13, 14, 15, 16, 17, 18, 10, 11, 12,
                0,
                7, 8, 9, 1, 2, 3, 4, 5, 6);


# PUBLIC and EXTERN
write_file("${test}1.asm", <<END);
	PUBLIC func_1
	PUBLIC func_2
func_1:
	ld a,1
func_2:
	ld a,2
	ret
END

write_file("${test}2.asm", <<END);
	EXTERN  func_2
	call func_2
	ret
END

# link object files
unlink "${test}2.bin";
run_ok("z80asm -l -m -b ${test}2.asm ${test}1.asm");
check_bin_file("${test}2.bin",
				0xCD, 0x06, 0x00, 0xC9, 
                0x3E, 0x01, 
                0x3E, 0x02, 0xC9);

# link library files
unlink "${test}1.lib";
run_ok("z80asm -x${test}1.lib ${test}1.asm");
ok -f "${test}1.lib", "${test}1.lib created";

unlink "${test}2.bin";
run_ok("z80asm -l -m -b -l${test}1.lib ${test}2.asm");
check_bin_file("${test}2.bin",
				0xCD, 0x06, 0x00, 0xC9, 
                0x3E, 0x01, 
                0x3E, 0x02, 0xC9);

end_test();
