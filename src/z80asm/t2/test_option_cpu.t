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

# __CPU_xxx_contants___ and __ARCH_xxx_constants__
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
		if __ARCH_TI83__
		defb 12
		endif
		if __ARCH_TI83PLUS__
		defb 13
		endif
END

for my $spurious ("", "-mr2k -m8080") {	# check that only last -m has effect
	asm_ok($asm, "$spurious -mz80     ", 1, 9);
	asm_ok($asm, "$spurious -mti83    ", 1, 9, 12);
	asm_ok($asm, "$spurious -mti83plus", 1, 9, 13);
	asm_ok($asm, "$spurious -mz80n    ", 2, 9);
	asm_ok($asm, "$spurious -mz180    ", 3, 9);
	asm_ok($asm, "$spurious -mr2k     ", 4, 10);
	asm_ok($asm, "$spurious -mr3k     ", 5, 10);
	asm_ok($asm, "$spurious -m8080    ", 6, 11);
	asm_ok($asm, "$spurious -m8085    ", 7, 11);
	asm_ok($asm, "$spurious -mgbz80   ", 8);
}

asm_ok("swapnib", "-mz80n", 0xED, 0x23);

asm_ok("slp", "-mz180", 0xED, 0x76);

asm_ok("ex (sp),hl", "     ", 0xE3);
asm_ok("ex (sp),hl", "-mr2k", 0xED, 0x54);

asm_ok("push su", "-mr3k", 0xED, 0x66);

asm_ok("invoke 0x1234", "          ", 0xCD, 0x34, 0x12);
asm_ok("invoke 0x1234", "-mti83    ", 0xCD, 0x34, 0x12);
asm_ok("invoke 0x1234", "-mti83plus", 0xEF, 0x34, 0x12);

end_test();
