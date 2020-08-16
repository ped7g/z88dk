#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

package testlib;

use Modern::Perl;
use File::Basename;
use Test::More;
use Path::Tiny;
use Config;

require Exporter;
our @ISA = qw(Exporter);
our @EXPORT = qw( 
			ok nok diag note 
			test_name 
			run_ok run_nok 
			asm_ok 
			system_ok 
			path 
			write_file check_text_file check_bin_file
			unlink_testfiles end_test 
);

my $exe = $Config{osname} eq 'MSWin32' ? '.exe' : '';
my $test = basename($0, '.t');
$ENV{PATH} = '.'.$Config{path_sep}.
			 '../z80nm'.$Config{path_sep}.
			 $ENV{PATH};

sub test_name { return $test; }

sub run {
	my($ok, $test, $cmd, $expout, $experr) = @_;
	$expout //= "";
	$experr //= "";

	$cmd .= " > $test.gotout 2> $test.goterr";
	if ($ok) {
		ok 0 == system($cmd), $cmd;
	}
	else {
		ok 0 != system($cmd), $cmd;
	}

	check_text_file("$test.gotout", $expout);
	check_text_file("$test.goterr", $experr);
}

sub run_ok  { run(1, $test, @_); }
sub run_nok { run(0, $test, @_); }

sub asm_ok {
	my($asm, $options, @bin) = @_;
	write_file("$test.asm", $asm);
	unlink("$test.bin");
	run_ok("z80asm -b $options $test.asm", "", "");
	check_bin_file("$test.bin", @bin), "bin ok";
}

sub system_ok {
	my($cmd) = @_;
	ok 0==system($cmd), $cmd;
}

sub unlink_testfiles {
	unlink <${test}*> if Test::More->builder->is_passing;
}

sub end_test {
	unlink_testfiles();
	done_testing();
}

sub write_file {
	my($file, $contents) = @_;
	path($file)->spew_raw($contents);
}

sub check_text_file {
	my($file, $expected) = @_;
	write_file("$file.expected", $expected);
	system_ok("diff -w $file $file.expected");
	unlink "$file.expected" if Test::More->builder->is_passing;
} 

sub check_bin_file {
	my($file, @data) = @_;
	ok path($file)->slurp_raw eq pack("C*", @data), "$file ok";
}

1;
