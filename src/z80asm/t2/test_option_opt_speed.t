#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

# Test https://github.com/z88dk/z88dk/issues/1077
# z80asm: optimize for speed - replace JR by JP

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

asm_ok('djnz 0',	'          ', 0x10, 0xFE);
asm_ok('djnz 0',	'-opt-speed', 0x10, 0xFE);

asm_ok('jr 0',		'          ', 0x18, 0xFE);
asm_ok('jr 0',		'-opt-speed', 0xC3, 0x00, 0x00);
asm_ok('jp 0',		'          ', 0xC3, 0x00, 0x00);
asm_ok('jp 0',		'-opt-speed', 0xC3, 0x00, 0x00);

asm_ok('jr nz, 0',	'          ', 0x20, 0xFE);
asm_ok('jr nz, 0',	'-opt-speed', 0xC2, 0x00, 0x00);
asm_ok('jp nz, 0',	'          ', 0xC2, 0x00, 0x00);
asm_ok('jp nz, 0',	'-opt-speed', 0xC2, 0x00, 0x00);

asm_ok('jr z, 0',	'          ', 0x28, 0xFE);
asm_ok('jr z, 0',	'-opt-speed', 0xCA, 0x00, 0x00);
asm_ok('jp z, 0',	'          ', 0xCA, 0x00, 0x00);
asm_ok('jp z, 0',	'-opt-speed', 0xCA, 0x00, 0x00);

asm_ok('jr nc, 0',	'          ', 0x30, 0xFE);
asm_ok('jr nc, 0',	'-opt-speed', 0xD2, 0x00, 0x00);
asm_ok('jp nc, 0',	'          ', 0xD2, 0x00, 0x00);
asm_ok('jp nc, 0',	'-opt-speed', 0xD2, 0x00, 0x00);

asm_ok('jr c, 0',	'          ', 0x38, 0xFE);
asm_ok('jr c, 0',	'-opt-speed', 0xDA, 0x00, 0x00);
asm_ok('jp c, 0',	'          ', 0xDA, 0x00, 0x00);
asm_ok('jp c, 0',	'-opt-speed', 0xDA, 0x00, 0x00);

end_test();
