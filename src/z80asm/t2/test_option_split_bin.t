#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

# Test -split-bin option

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

# no -split-bin
write_file("${test}.asm", <<END);
	defw ASMPC
	
	section code
	defw ASMPC
	
	section data
	defw ASMPC
	
	section bss		; split file here
	org 0x4000
	defw ASMPC
END

unlink "${test}.bin", "${test}_code.bin", "${test}_data.bin", "${test}_bss.bin";
run_ok("z80asm -b ${test}.asm");

check_bin_file("${test}.bin", unpack("C*", pack("v*", 0, 2, 4)));
ok ! -f "${test}_code.bin", "no ${test}_code.bin";
ok ! -f "${test}_data.bin", "no ${test}_data.bin";
check_bin_file("${test}_bss.bin", unpack("C*", pack("v*", 0x4000)));


# -split-bin
write_file("${test}.asm", <<END);
	defw ASMPC		; split file here
	
	section code	; split file here
	defw ASMPC
	
	section data	; split file here
	defw ASMPC
	
	section bss		; split file here
	org 0x4000
	defw ASMPC
END

unlink "${test}.bin", "${test}_code.bin", "${test}_data.bin", "${test}_bss.bin";
run_ok("z80asm -b -split-bin ${test}.asm");

check_bin_file("${test}.bin", unpack("C*", pack("v*", 0)));
check_bin_file("${test}_code.bin", unpack("C*", pack("v*", 2)));
check_bin_file("${test}_data.bin", unpack("C*", pack("v*", 4)));
check_bin_file("${test}_bss.bin", unpack("C*", pack("v*", 0x4000)));


# ORG -1 to split
write_file("${test}.asm", <<END);
	defw ASMPC
	
	section code
	defw ASMPC
	
	section data	; split file here
	org 0x4000
	defw ASMPC
	
	section bss		; split file here
	org -1
	defw ASMPC
END

unlink "${test}.bin", "${test}_code.bin", "${test}_data.bin", "${test}_bss.bin";
run_ok("z80asm -b ${test}.asm");

check_bin_file("${test}.bin", unpack("C*", pack("v*", 0, 2)));
ok ! -f "${test}_code.bin", "no ${test}_code.bin";
check_bin_file("${test}_data.bin", unpack("C*", pack("v*", 0x4000)));
check_bin_file("${test}_bss.bin", unpack("C*", pack("v*", 0x4002)));

end_test();
