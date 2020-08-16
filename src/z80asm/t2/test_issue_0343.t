#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

# z80asm: Rabbit emulation of cpi and cpir is broken

use Modern::Perl;
use Config;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

write_file("$test.asm", <<END);
	extern __z80asm__cpir
	call   __z80asm__cpir
END

unlink "$test.bin";
run_ok("z80asm -b -mz80 $test.asm", '', '');
my $bin_z80 = path("$test.bin")->slurp_raw();

unlink "$test.bin";
run_ok("z80asm -b -mr2k $test.asm", '', '');
my $bin_r2k = path("$test.bin")->slurp_raw();

ok $bin_z80 ne $bin_r2k, "binary different";	# ex (sp),hl is E9 in z80, ED54 in r2k

write_file("$test.c", <<END);
#include <string.h>
int main () { return strncmp("hello world", "hello", 5); }
END
run_ok("zcc +test -mr2k -clib=rabbit $test.c -o $test.bin");
run_ok("z88dk-ticks -mr2k $test.bin", <<END, "");

Ticks: 2550
END

end_test();
