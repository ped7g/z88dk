#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

# Test -d option

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

# first compiles; second skips
unlink_testfiles();
write_file("$test.asm", "nop");
test_compile("", "$test.asm", "$test.o");

my $date_obj = -M "$test.o";

# now skips compile
sleep 1;            # make sure our obj is older
test_compile("", "$test.asm", "$test.o");

ok abs((-M "$test.o") - $date_obj) < 0.001, "$test.o is the same";

# touch source
sleep 1;		    # make sure our obj is older
write_file("$test.asm", "nop");
test_compile("", "$test.asm", "$test.o");

ok abs((-M "$test.o") - $date_obj) > 0, "$test.o is newer";

# remove source, give -d -> uses existing object 
# ... - with extensiom
unlink "$test.asm";
$date_obj = -M "$test.o";
test_compile("", "$test.asm", "$test.o");
ok abs((-M "$test.o") - $date_obj) < 0.001, "$test.o is the same";

# ... - without extensiom
test_compile("", "$test", "$test.o");
ok abs((-M "$test.o") - $date_obj) < 0.001, "$test.o is the same";

# test -d with -O
unlink_testfiles();
write_file("$test.asm", "nop");
path("${test}_dir")->remove_tree;
test_compile("-O${test}_dir/obj", "$test", "${test}_dir/obj/$test.o");

$date_obj = -M "${test}_dir/obj/$test.o";

# now skips compile
sleep 1;            # make sure our obj is older
test_compile("-O${test}_dir/obj", "$test", "${test}_dir/obj/$test.o");
ok abs((-M "$test.o") - $date_obj) < 0.001, "$test.o is the same";

# touch source
sleep 1;		    # make sure our obj is older
write_file("$test.asm", "nop");
test_compile("-O${test}_dir/obj", "$test", "${test}_dir/obj/$test.o");

ok abs((-M "$test.o") - $date_obj) > 0, "$test.o is newer";

path("${test}_dir")->remove_tree;
end_test();

sub test_compile {
    my($options, $asm, $obj) = @_;
    run_ok("z80asm -d $options $asm");
    ok -f "$obj", "$obj created";
    run_ok("z80nm -a $obj | grep ' C '", <<'END', "");
    C $0000: 00
END
}
