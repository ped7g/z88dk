#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

# z80asm: ld hl, sp+ -6

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

for my $n (-4, 0, 4) {
	my $offset = $n > 0 ? "+$n" : $n < 0 ? "$n" : "";

	asm_ok("ld hl, sp$offset", "-mgbz80", 0xF8, $n & 0xFF);
	
	asm_ok(<<END, "-m8085", (0x28, $n & 0xFF) x 3, (0x38, $n & 0xFF) x 3);
		ldhi $n
		adi hl,$n
		ld de, hl$offset
		ldsi $n
		adi sp,$n
		ld de, sp$offset
END
	
	asm_ok(<<END, "-mr2k", 0xE4, $n & 0xFF, 0xF4, $n & 0xFF, 0xC4, $n & 0xFF, 0xD4, $n & 0xFF);
		ld hl, (ix$offset)
		ld (ix$offset), hl
		ld hl, (sp$offset)
		ld (sp$offset), hl
END
}

# check warnings
for my $n (-129, -128, 0, 255, 256) {
	my $offset = $n > 0 ? "+$n" : $n < 0 ? "$n" : "";
	my $warning = ($n >= -128 && $n < 256) ? 
		"" : 
		"Warning at file '$test.asm' line 1: integer '$n' out of range\n";

	ok 1, "n=$n";

	write_file("$test.asm", <<END);
		ld de, hl$offset
END
	unlink "$test.bin";
	run_ok("z80asm -b -m8085 $test.asm", '', $warning);
	ok path("$test.bin")->slurp_raw() eq pack("C*", 0x28, $n & 0xFF), "bin ok";
	
	write_file("$test.asm", <<END);
		ld de, sp$offset
END
	unlink "$test.bin";
	run_ok("z80asm -b -m8085 $test.asm", '', $warning);
	ok path("$test.bin")->slurp_raw() eq pack("C*", 0x38, $n & 0xFF), "bin ok";

	write_file("$test.asm", <<END);
		ld hl, (sp$offset)
END
	unlink "$test.bin";
	run_ok("z80asm -b -mr2k $test.asm", '', $warning);
	ok path("$test.bin")->slurp_raw() eq pack("C*", 0xC4, $n & 0xFF), "bin ok";
	
	write_file("$test.asm", <<END);
		ld (sp$offset), hl
END
	unlink "$test.bin";
	run_ok("z80asm -b -mr2k $test.asm", '', $warning);
	ok path("$test.bin")->slurp_raw() eq pack("C*", 0xD4, $n & 0xFF), "bin ok";
}

for my $n (-129, -128, 0, 127, 128) {
	my $offset = $n > 0 ? "+$n" : $n < 0 ? "$n" : "";
	my $warning = ($n >= -128 && $n < 128) ? 
		"" : 
		"Warning at file '$test.asm' line 1: integer '$n' out of range\n";

	ok 1, "n=$n";

	write_file("$test.asm", <<END);
		ld hl, (ix$offset)
END
	unlink "$test.bin";
	run_ok("z80asm -b -mr2k $test.asm", '', $warning);
	ok path("$test.bin")->slurp_raw() eq pack("C*", 0xE4, $n & 0xFF), "bin ok";
	
	write_file("$test.asm", <<END);
		ld (ix$offset), hl
END
	unlink "$test.bin";
	run_ok("z80asm -b -mr2k $test.asm", '', $warning);
	ok path("$test.bin")->slurp_raw() eq pack("C*", 0xF4, $n & 0xFF), "bin ok";
	
}

end_test();
