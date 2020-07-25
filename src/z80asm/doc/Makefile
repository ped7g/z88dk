#------------------------------------------------------------------------------
# z80asm restart
#
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

MV		?= mv -f

all: 	z80asm.md   	z80asm-future.md \
		z80asm.html 	z80asm-future.html \
		z80asm.txt  	z80asm-future.txt \
		z80asm_manual.c z80asm_manual.h \
		z80asm_usage.c  z80asm_usage.h

.SUFFIXES: .md .in .html .txt

.in.md:
	perl make_index.pl < $< > $@~
	$(MV) $@~ $@

.md.html:
	@echo \# use grip from https://github.com/joeyespo/grip
	grip $< --export $@~
	if grep '<title>500' $@~ ; then $(RM) $@~; exit 1; fi
	$(MV) $@~ $@

.html.txt:
	@echo use elinks to render html
	elinks -dump $< > $@~1
	perl -pe 's{file:///.*?/src/z80asm/doc/}{file://}g' $@~1 > $@~2
	$(MV) $@~2 $@
	$(RM) $@~1

z80asm.md z80asm-future.md: Makefile make_index.pl

z80asm.html z80asm-future.html: Makefile

z80asm.txt z80asm-future.txt: Makefile

z80asm_manual.c: Makefile make_c.pl z80asm.txt
	perl make_c.pl -n z80asm_manual z80asm.txt

z80asm_manual.h:
	touch $@

z80asm_usage.c: Makefile make_c.pl usage.txt
	perl make_c.pl -n z80asm_usage usage.txt

z80asm_usage.h:
	touch $@
