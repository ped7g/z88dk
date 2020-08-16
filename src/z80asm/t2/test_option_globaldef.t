#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

# Test -g

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

write_file("${test}.asm", <<END);
	PUBLIC main, x31_x31_x31_x31_x31_x31_x31_x31, x_32_x32_x32_x32_x32_x32_x32_x32
main: ld b,10
loop: djnz loop
	PUBLIC last
last:
x31_x31_x31_x31_x31_x31_x31_x31: defb 0
x_32_x32_x32_x32_x32_x32_x32_x32: defb 0
END

write_file("${test}2.asm", <<END);
	; show DEFC alias in map file
	PUBLIC alias_main
	EXTERN main
	defc alias_main = main
	
	PUBLIC alias_last
	EXTERN last
	defc alias_last = last

	PUBLIC func
func: ret
END

my @bin = (0x06, 0x0A, 0x10, 0xFE, 0x00, 0x00, 0xC9);

# no -g
unlink "${test}.bin", "${test}.def";
run_ok("z80asm -b ${test}.asm ${test}2.asm", "", "");
check_bin_file("${test}.bin", @bin);
ok ! -f "${test}.def", "no ${test}.def file";

# -g
unlink "${test}.bin", "${test}.def";
run_ok("z80asm -b -g ${test}.asm ${test}2.asm", "", "");
check_bin_file("${test}.bin", @bin);

check_text_file("${test}.def", <<END);
DEFC main                            = \$0000
DEFC x31_x31_x31_x31_x31_x31_x31_x31 = \$0004
DEFC x_32_x32_x32_x32_x32_x32_x32_x32 = \$0005
DEFC last                            = \$0004
DEFC alias_main                      = \$0000
DEFC alias_last                      = \$0004
DEFC func                            = \$0006
END

end_test();
