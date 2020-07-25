#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

# Test map file

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

# - BUG_0036 : Map file does not show local symbols with the same name in different modules
# If the same local symbol is defined in multiple modules, only one of
# them appears in the map file.
my $asm = "
	define not_shown
	defc zero=0
	PUBLIC main
main: ld b,10
loop: djnz loop
	PUBLIC last
last:
x31_x31_x31_x31_x31_x31_x31_x31: defb zero
x_32_x32_x32_x32_x32_x32_x32_x32: defb zero
";
my $asm_s = " ld b,10 \n djnz ASMPC \n defw 0 ";

my $asm1 = "
	define not_shown
	
	; show DEFC alias in map file
	PUBLIC alias_main
	EXTERN main
	defc alias_main = main
	
	PUBLIC alias_last
	EXTERN last
	defc alias_last = last

	PUBLIC func
func: ld b,10
loop: djnz loop
	  ret
";
my $asm1_s = " ld b,10 \n djnz ASMPC \n ret ";

my $bin = pack("C*", 0x06, 0x0A, 0x10, 0xFE, 0x00, 0x00, 0x06, 0x0A, 0x10, 0xFE, 0xC9);


# no -m
unlink "${test}.map";
path("${test}.asm")->spew($asm_s);
path("${test}1.asm")->spew($asm1_s);
system_ok("z80asm -b ${test}.asm ${test}1.asm");
ok ! -f "${test}.map", "no ${test}.map";
ok path("${test}.bin")->slurp_raw eq $bin, "${test}.bin ok";

for my $opt ('-m', '--map') {

	# -m, no symbols
	unlink "${test}.map";
	path("${test}.asm")->spew($asm_s);
	path("${test}1.asm")->spew($asm1_s);
	system_ok("z80asm -b $opt ${test}.asm ${test}1.asm");
	ok -f "${test}.map", "got ${test}.map";
	ok path("${test}.bin")->slurp_raw eq $bin, "${test}.bin ok";
	path("${test}.exp")->spew(<<END);
__head                          = \$0000 ; const, public, def, , ,
__tail                          = \$000B ; const, public, def, , ,
__size                          = \$000B ; const, public, def, , ,
END
	system_ok("diff -w ${test}.exp ${test}.map");
	
	# -m, symbols
	unlink "${test}.map";
	path("${test}.asm")->spew($asm);
	path("${test}1.asm")->spew($asm1);
	system_ok("z80asm -b $opt ${test}.asm ${test}1.asm");
	ok -f "${test}.map", "got ${test}.map";
	ok path("${test}.bin")->slurp_raw eq $bin, "${test}.bin ok";
	path("${test}.exp")->spew(<<END);
zero                            = \$0000 ; const, local, , ${test}, , ${test}.asm:3
loop                            = \$0002 ; addr, local, , ${test}, , ${test}.asm:6
x31_x31_x31_x31_x31_x31_x31_x31 = \$0004 ; addr, local, , ${test}, , ${test}.asm:9
x_32_x32_x32_x32_x32_x32_x32_x32 = \$0005 ; addr, local, , ${test}, , ${test}.asm:10
loop                            = \$0008 ; addr, local, , ${test}1, , ${test}1.asm:15
main                            = \$0000 ; addr, public, , ${test}, , ${test}.asm:5
last                            = \$0004 ; addr, public, , ${test}, , ${test}.asm:8
alias_main                      = \$0000 ; addr, public, , ${test}1, , ${test}1.asm:7
alias_last                      = \$0004 ; addr, public, , ${test}1, , ${test}1.asm:11
func                            = \$0006 ; addr, public, , ${test}1, , ${test}1.asm:14
__head                          = \$0000 ; const, public, def, , ,
__tail                          = \$000B ; const, public, def, , ,
__size                          = \$000B ; const, public, def, , ,
END
	system_ok("diff -w ${test}.exp ${test}.map");
}

end_test();
