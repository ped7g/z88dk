#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

for my $cpu ("", "--cpuz80", "--cpu=z80", "-mz80", "-m=z80") {
	asm_ok(<<END, $cpu, 0x76);
		if __CPU_Z80__
		halt
		endif
END
}

# __CPU_xxx_contants___
my $asm = <<END;
		if __CPU_Z80__
		defb 1
		endif
		if __CPU_Z80N__
		defb 2
		endif
		if __CPU_Z180__
		defb 3
		endif
		if __CPU_R2K__
		defb 4
		endif
		if __CPU_R3K__
		defb 5
		endif
		if __CPU_8080__
		defb 6
		endif
		if __CPU_8085__
		defb 7
		endif
		if __CPU_GBZ80__
		defb 8
		endif
		if __CPU_ZILOG__
		defb 9
		endif
		if __CPU_RABBIT__
		defb 10
		endif
		if __CPU_INTEL__
		defb 11
		endif
		if __SWAP_IX_IY__
		defb 12
		endif
END

asm_ok($asm, "-mz80  ", 1, 9);
asm_ok($asm, "-mz80n ", 2, 9);
asm_ok($asm, "-mz180 ", 3, 9);
asm_ok($asm, "-mr2k  ", 4, 10);
asm_ok($asm, "-mr3k  ", 5, 10);
asm_ok($asm, "-m8080 ", 6, 11);
asm_ok($asm, "-m8085 ", 7, 11);
asm_ok($asm, "-mgbz80", 8);

asm_ok($asm, "-mz80   -IXIY", 1, 9, 12);
asm_ok($asm, "-mz80n  -IXIY", 2, 9, 12);
asm_ok($asm, "-mz180  -IXIY", 3, 9, 12);
asm_ok($asm, "-mr2k   -IXIY", 4, 10, 12);
asm_ok($asm, "-mr3k   -IXIY", 5, 10, 12);
asm_ok($asm, "-m8080  -IXIY", 6, 11, 12);
asm_ok($asm, "-m8085  -IXIY", 7, 11, 12);
asm_ok($asm, "-mgbz80 -IXIY", 8, 12);

asm_ok("swapnib", "-mz80n", 0xED, 0x23);

asm_ok("slp", "-mz180", 0xED, 0x76);

asm_ok("ex (sp),hl", "     ", 0xE3);
asm_ok("ex (sp),hl", "-mr2k", 0xED, 0x54);

asm_ok("push su", "-mr3k", 0xED, 0x66);

asm_ok("invoke 0x1234", "          ", 0xCD, 0x34, 0x12);
asm_ok("invoke 0x1234", "-mti83    ", 0xCD, 0x34, 0x12);
asm_ok("invoke 0x1234", "-mti83plus", 0xEF, 0x34, 0x12);

asm_ok("ld ix,0x1234", "      ", 0xDD, 0x21, 0x34, 0x12);
asm_ok("ld ix,0x1234", " -IXIY", 0xFD, 0x21, 0x34, 0x12);
asm_ok("ld ix,0x1234", "--IXIY", 0xFD, 0x21, 0x34, 0x12);

asm_ok("ld iy,0x1234", "      ", 0xFD, 0x21, 0x34, 0x12);
asm_ok("ld iy,0x1234", " -IXIY", 0xDD, 0x21, 0x34, 0x12);
asm_ok("ld iy,0x1234", "--IXIY", 0xDD, 0x21, 0x34, 0x12);

end_test();
