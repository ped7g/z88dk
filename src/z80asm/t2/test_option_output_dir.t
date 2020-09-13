#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

# Test -O option
# Test https://github.com/z88dk/z88dk/issues/441
# z80asm: add command line option for specifying the output directory

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

my @output = ("${test}.o", "${test}.bin", "${test}.sym", "${test}.lis", "${test}.map", "${test}.def");

write_file("${test}.asm", "ret");

# first run without -O
unlink_output();
run_ok("z80asm -s -l -m -g -b ${test}.asm");
for (@output) {
	ok -f $_, "$_ created";
}
check_bin_file("${test}.bin", 0xC9);
ok ! -d "${test}_dir", "${test}_dir not created";

# second run with -O
for my $equal ("", "=") {
    for my $dir ("${test}_dir", "${test}_dir//sub/dir/") {
        unlink_output();
        run_ok("z80asm -s -l -m -g -b -O${equal}${dir} ${test}.asm");
        ok -d $dir, "$dir created";
        for (@output) {
            ok -f "$dir/$_", "$dir/$_ created";
        }
        check_bin_file("$dir/${test}.bin", 0xC9);
    }
}

# make trees under output dir
for my $abs_path (0..1) {
    unlink_output();
    unlink_testfiles();

    my $src_file = "${test}_src/s1/s2/${test}.asm";
    path($src_file)->parent->mkpath;
    write_file($src_file, "ret");

    if ($abs_path) {
        $src_file = path($src_file)->absolute;
    }
    run_ok("z80asm -s -l -m -g -b -O${test}_dir $src_file");

    my $out_dir = path("${test}_dir/$src_file")->parent;

    # <kludge> detect home directory of msys
    if ($^O eq 'msys' && !-d $out_dir && $src_file =~ /^\/(\w+)\//) {
        # test_dir/home/... -> test_dir/C/msys64/home/...
        my $root = $1;
        $out_dir = path("${test}_dir");
        while (1) {
            my @children = $out_dir->children;
            die if @children != 1;
            last if $children[0]->basename eq $root;
            $out_dir = $children[0];
        }
        $out_dir = path($out_dir, $src_file)->parent;
    }
    # </kludge>
    
    ok -d $out_dir, "$out_dir created";
    for (@output) {
        ok -f "$out_dir/$_", "$out_dir/$_ created";
    }
    check_bin_file("$out_dir/${test}.bin", 0xC9);
}

unlink_output();
unlink_testfiles();
end_test();


sub unlink_output {
    unlink @output;
    path("${test}_dir")->remove_tree;
    path("${test}_src")->remove_tree;
}
