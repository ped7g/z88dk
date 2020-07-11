#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

package testlib;

use strict;
use warnings;
use File::Basename;
use Test::More;
use Path::Tiny;
use Config;

require Exporter;
our @ISA = qw(Exporter);
our @EXPORT = qw( ok nok diag note test_name run_ok run_nok asm_ok system_ok end_test path );

my $exe = $Config{osname} eq 'MSWin32' ? '.exe' : '';
my $test = basename($0, '.t');
$ENV{PATH} = '.'.$Config{path_sep}.
			 '../z80nm'.$Config{path_sep}.
			 $ENV{PATH};

sub test_name { return $test; }

sub run {
	my($ok, $test, $cmd, $expout, $experr) = @_;

	ok open(my $out, ">", "$test.expout"), "write $test.expout";
	print $out $expout;
	close $out;

	ok open(my $err, ">", "$test.experr"), "write $test.experr";
	print $err $experr;
	close $err;

	$cmd .= " > $test.gotout 2> $test.goterr";
	if ($ok) {
		ok 0 == system($cmd), $cmd;
	}
	else {
		ok 0 != system($cmd), $cmd;
	}

	$cmd = "diff -w $test.gotout $test.expout";
	ok 0 == system($cmd), $cmd;

	$cmd = "diff -w $test.goterr $test.experr";
	ok 0 == system($cmd), $cmd;
}

sub run_ok  { run(1, $test, @_); }
sub run_nok { run(0, $test, @_); }

sub asm_ok {
	my($asm, $options, @bin) = @_;
	path("$test.asm")->spew($asm);
	unlink("$test.bin");
	run_ok("z80asm -b $options $test.asm", "", "");
	ok path("$test.bin")->slurp_raw eq pack("C*", @bin), "bin ok";
}

sub system_ok {
	my($cmd) = @_;
	ok 0==system($cmd), $cmd;
}

sub end_test {
	if (Test::More->builder->is_passing) {
		unlink("$test.gotout", "$test.goterr", "$test.expout", "$test.experr",
				"$test.asm", "$test.d", "$test.o", "$test.bin", 
				"$test.lis", "$test.err", "$test.map", 
				"$test.c");
	}
	done_testing();
}

1;
