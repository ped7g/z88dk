#!/usr/bin/perl

# Z88DK Z80 Macro Assembler
#
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#
# Test options

use strict;
use warnings;
use File::Slurp;
use File::Basename;
use File::Copy;
use Time::HiRes 'sleep';
use Capture::Tiny 'capture_merged';
use Test::Differences; 
use Test::More;
require './t/test_utils.pl';

#------------------------------------------------------------------------------
# asm extension
#------------------------------------------------------------------------------
for my $file ('test', 'test.asm') {
	unlink_testfiles();
	write_file('test.asm', "ret");
	t_z80asm_capture("-b $file", "", "", 0);
	is read_file(bin_file(), binary => ':raw'), "\xC9", "assemble ok";
}

unlink_testfiles();
write_file('test.xxx', "ret");
t_z80asm_capture("-b test.xxx", "", "", 0);
is read_file(bin_file(), binary => ':raw'), "\xC9", "assemble ok";

unlink_testfiles('test.xxx');

#------------------------------------------------------------------------------
# o extension
#------------------------------------------------------------------------------
for my $file ('test', 'test.o') {
	unlink_testfiles();
	write_file('test.asm', "ret");
	t_z80asm_capture("-b $file", "", "", 0);
	is read_file(bin_file(), binary => ':raw'), "\xC9", "assemble ok";
	
	unlink(bin_file(), 'test.asm');
	t_z80asm_capture("-b $file", "", "", 0);
	is read_file(bin_file(), binary => ':raw'), "\xC9", "assemble ok";
}

#------------------------------------------------------------------------------
# -s
#------------------------------------------------------------------------------
unlink_testfiles();

my $asm = "
	PUBLIC main
main:	ld b,10
loop:	djnz loop
	ret
";
my $bin = pack("C*", 
	0x06, 10,
	0x10, -2 & 0xFF,
	0xC9
);

# no symbol table
t_z80asm(
	asm		=> $asm,
	bin		=> $bin,
	options	=> "",
);

# symbol table
t_z80asm(
	asm		=> $asm,
	bin		=> $bin,
	options	=> '-s',
);

#------------------------------------------------------------------------------
# -b
#------------------------------------------------------------------------------

# no -b
unlink_testfiles();
write_file(asm_file(), "nop");

t_z80asm_capture(asm_file(), "", "", 0);
ok -f o_file();
ok ! -f bin_file();

# -b
unlink_testfiles();
write_file(asm_file(), "nop");

t_z80asm_capture("-b ".asm_file(), "", "", 0);
ok -f o_file();
ok -f bin_file();
is read_file(bin_file(), binmode => ':raw'), "\0";

#------------------------------------------------------------------------------
# -o
#------------------------------------------------------------------------------
$bin = bin_file(); $bin =~ s/\.bin$/2.bin/i;

# no -o
unlink_testfiles($bin);
write_file(asm_file(), "nop");

t_z80asm_capture("-b ".asm_file(), "", "", 0);
ok -f bin_file();
ok ! -f $bin;
t_binary(read_file(bin_file(), binmode => ':raw'), "\0");

# -o
unlink_testfiles($bin);
write_file(asm_file(), "nop");

t_z80asm_capture("-b -o$bin ".asm_file(), "", "", 0);
ok ! -f bin_file();
ok -f $bin;
t_binary(read_file($bin, binmode => ':raw'), "\0");

# test -o with environment variables
$ENV{TEST_ENV} = '2.bin';
unlink_testfiles($bin);
write_file(asm_file(), "nop");
t_z80asm_capture('-b "-otest${TEST_ENV}" '.asm_file(), "", "", 0);
ok ! -f bin_file();
ok -f $bin;
t_binary(read_file($bin, binmode => ':raw'), "\0");

delete $ENV{TEST_ENV};
unlink_testfiles($bin);
write_file(asm_file(), "nop");
t_z80asm_capture('-b "-otest${TEST_ENV}2.bin" '.asm_file(), "", "", 0);
ok ! -f bin_file();
ok -f $bin;
t_binary(read_file($bin, binmode => ':raw'), "\0");

unlink_testfiles($bin);

#------------------------------------------------------------------------------
# -mz180
t_z80asm_ok(0, "
	slp
	
	mlt bc
	mlt de
	mlt hl
	mlt sp
	
	in0 b,(10)
	in0 c,(11)
	in0 d,(12)
	in0 e,(13)
	in0 h,(14)
	in0 l,(15)
	in0 f,(16)
	in0 a,(17)
	
	out0 (10),b
	out0 (11),c
	out0 (12),d
	out0 (13),e
	out0 (14),h
	out0 (15),l
	out0 (17),a
	
	otim
	otimr
	otdm
	otdmr
	
	tstio 23
	
	tst a,b
	tst a,c
	tst a,d
	tst a,e
	tst a,h
	tst a,l
	tst a,(hl)
	tst a,a
	tst a,23
	
	tst b
	tst c
	tst d
	tst e
	tst h
	tst l
	tst (hl)
	tst a
	tst 23
	
	test a,b
	test a,c
	test a,d
	test a,e
	test a,h
	test a,l
	test a,(hl)
	test a,a
	test a,23
	
	test b
	test c
	test d
	test e
	test h
	test l
	test (hl)
	test a
	test 23
	
", pack("C*", 
	0xED, 0x76,		# slp
	
	0xED, 0x4C, 	# mlt
	0xED, 0x5C, 
	0xED, 0x6C, 
	0xED, 0x7C, 
	
	0xED, 0x00, 10,	# in0
	0xED, 0x08, 11,	
	0xED, 0x10, 12,	
	0xED, 0x18, 13,	
	0xED, 0x20, 14,	
	0xED, 0x28, 15,	
	0xED, 0x30, 16,	
	0xED, 0x38, 17,	
	
	0xED, 0x01, 10,	# out0
	0xED, 0x09, 11,	
	0xED, 0x11, 12,	
	0xED, 0x19, 13,	
	0xED, 0x21, 14,	
	0xED, 0x29, 15,	
	0xED, 0x39, 17,	
	
	0xED, 0x83,		# otxx
	0xED, 0x93,		
	0xED, 0x8B,		
	0xED, 0x9B,		
	
	0xED, 0x74, 23,	# tstio
	
	0xED, 0x04,		# tst
	0xED, 0x0C,
	0xED, 0x14,
	0xED, 0x1C,
	0xED, 0x24,
	0xED, 0x2C,
	0xED, 0x34,
	0xED, 0x3C,
	0xED, 0x64, 23, 
	
	0xED, 0x04,		# tst
	0xED, 0x0C,
	0xED, 0x14,
	0xED, 0x1C,
	0xED, 0x24,
	0xED, 0x2C,
	0xED, 0x34,
	0xED, 0x3C,
	0xED, 0x64, 23, 
	
	0xED, 0x04,		# test
	0xED, 0x0C,
	0xED, 0x14,
	0xED, 0x1C,
	0xED, 0x24,
	0xED, 0x2C,
	0xED, 0x34,
	0xED, 0x3C,
	0xED, 0x64, 23, 
	
	0xED, 0x04,		# test
	0xED, 0x0C,
	0xED, 0x14,
	0xED, 0x1C,
	0xED, 0x24,
	0xED, 0x2C,
	0xED, 0x34,
	0xED, 0x3C,
	0xED, 0x64, 23, 
	
), "-mz180");

t_z80asm_error("slp			", "Error at file 'test.asm' line 1: illegal identifier");
t_z80asm_error("mlt bc		", "Error at file 'test.asm' line 1: illegal identifier");
t_z80asm_error("in0 b,(10)	", "Error at file 'test.asm' line 1: illegal identifier");
t_z80asm_error("out0 (10),b	", "Error at file 'test.asm' line 1: illegal identifier");
t_z80asm_error("otim		", "Error at file 'test.asm' line 1: illegal identifier");
t_z80asm_error("otimr		", "Error at file 'test.asm' line 1: illegal identifier");
t_z80asm_error("otdm		", "Error at file 'test.asm' line 1: illegal identifier");
t_z80asm_error("otdmr		", "Error at file 'test.asm' line 1: illegal identifier");
t_z80asm_error("tstio 23	", "Error at file 'test.asm' line 1: illegal identifier");
t_z80asm_error("tst b		", "Error at file 'test.asm' line 1: illegal identifier");

#------------------------------------------------------------------------------
# -mr2k
t_z80asm_ok(0, "
	altd ld a,31
	ioi ld a,(0xFE)
	ioe ld a,(0xFE)
	
", pack("C*", 
	0x76, 0x3E, 0x1F,
	0xD3, 0x3A, 0xFE, 0x00,
	0xDB, 0x3A, 0xFE, 0x00,
	
), "-mr2k");
	
unlink_testfiles();
done_testing();

sub quote_os {
	my($txt) = @_;
	if ($^O eq 'MSWin32') {
		return '"'.$txt.'"';
	}
	else {
		return "'".$txt."'";
	}
}
