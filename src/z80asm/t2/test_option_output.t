#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

# Test -o option

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

write_file("$test.asm", "ret");

# no -o
unlink "$test.bin";
run_ok("z80asm -b $test.asm");
check_bin_file("$test.bin", 0xC9);

# -o
unlink "$test.2.bin";
run_ok("z80asm -b -o$test.2.bin $test.asm");
check_bin_file("$test.2.bin", 0xC9);

# test -o with environment variables
$ENV{TEST_ENV} = "2.bin";
unlink "$test.2.bin";
run_ok("z80asm -b -o$test.\${TEST_ENV} $test.asm");
check_bin_file("$test.2.bin", 0xC9);

delete $ENV{TEST_ENV};
unlink "$test.2.bin";
run_ok("z80asm -b -o$test.\${TEST_ENV}2.bin $test.asm");
check_bin_file("$test.2.bin", 0xC9);

end_test();
