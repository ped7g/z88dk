#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

path("$test.asm")->spew(<<END);
	nop
END

my $manual = path("doc/z80asm.txt")->slurp;

# make sure help fits in 80 columns
ok open(my $fh, "<:encoding(UTF-8)", "doc/z80asm.txt"), "open doc/z80asm.txt";
while (<$fh>) {
	chomp;
	if (length($_) > 80) {
		ok 0, "line $. longer than 80 chars";
	}
}

for my $help (qw( -? -h )) {
	unlink "$test.bin";
	run_ok("z80asm -b $help $test.asm", $manual, '');
	ok ! -f "$test.bin", "$test.bin not assembled";
}

for my $illegal (qw( -hx --hx -h=1 -?x --?x -?=1 )) {
	run_nok("z80asm -b $illegal $test.asm", '', <<"ERR");
Error: illegal option: $illegal
ERR
}

end_test();
