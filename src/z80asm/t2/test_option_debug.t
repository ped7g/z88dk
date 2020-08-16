#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

# issue #341: Produce a debugger-friendly filename/bank/memory address file

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

# only ASM code
write_file("${test}1.asm", <<END);
		public func

		c_line 1, "${test}1.c"
	func:
		ld a, 1
		ld b, 3
		
		c_line 2
		add a, b
		ret
END

write_file("${test}.asm", <<END);
		extern func

	main:
		call func
		ret
END

run_ok("z80asm -b -m ${test}.asm ${test}1.asm");
check_text_file("${test}.map", <<END);
main                            = \$0000 ; addr, local, , ${test}, , ${test}.asm:3
func                            = \$0004 ; addr, public, , ${test}1, , ${test}1.c:1
__head                          = \$0000 ; const, public, def, , ,
__tail                          = \$000A ; const, public, def, , ,
__size                          = \$000A ; const, public, def, , ,
END

run_ok("z80asm -b -debug ${test}.asm ${test}1.asm");
check_text_file("${test}.map", <<END);
main                            = \$0000 ; addr, local, , ${test}, , ${test}.asm:3
__ASM_LINE_3                    = \$0000 ; addr, local, , ${test}, , ${test}.asm:3
__ASM_LINE_4                    = \$0000 ; addr, local, , ${test}, , ${test}.asm:4
__ASM_LINE_5                    = \$0003 ; addr, local, , ${test}, , ${test}.asm:5
__C_LINE_1                      = \$0004 ; addr, local, , ${test}1, , ${test}1.c:1
__C_LINE_2                      = \$0008 ; addr, local, , ${test}1, , ${test}1.c:2
func                            = \$0004 ; addr, public, , ${test}1, , ${test}1.c:1
__head                          = \$0000 ; const, public, def, , ,
__tail                          = \$000A ; const, public, def, , ,
__size                          = \$000A ; const, public, def, , ,
END


# ASM and C code
write_file("${test}1.asm", <<END);
		public _one
	_one:
		ld hl, 1
		ret
END

write_file("${test}.c", <<END);
	extern int one();

	int main() {
		return one()+one();
	}
END

run_ok("zcc +z80 -m -clib=new -Cc-gcline -Ca-debug ${test}.c ${test}1.asm -o${test}.bin");
system_ok("grep ${test} ${test}.map | grep -v '\\.i:[0-9]' > ${test}.map1");
check_text_file("${test}.map1", <<END);
__C_LINE_0                      = \$0000 ; addr, local, , ${test}_c, , ${test}.c:0
__C_LINE_3                      = \$0000 ; addr, local, , ${test}_c, , ${test}.c:3
__C_LINE_4                      = \$016C ; addr, local, , ${test}_c, code_compiler, ${test}.c::main:4
__C_LINE_5                      = \$0176 ; addr, local, , ${test}_c, code_compiler, ${test}.c::main:5
__ASM_LINE_2                    = \$0000 ; addr, local, , ${test}1_asm, , ${test}1.asm:2
__ASM_LINE_3                    = \$0000 ; addr, local, , ${test}1_asm, , ${test}1.asm:3
__ASM_LINE_4                    = \$0003 ; addr, local, , ${test}1_asm, , ${test}1.asm:4
_main                           = \$016C ; addr, public, , ${test}_c, code_compiler, ${test}.c::main:3
_one                            = \$0000 ; addr, public, , ${test}1_asm, , ${test}1.asm:2
END

end_test();
