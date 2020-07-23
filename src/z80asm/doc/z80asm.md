<a id=top></a>

# z80asm - Z80 module assembler, linker, librarian

**z80asm** is part of the [z88dk](http://www.z88dk.org/) project and is used as the back-end of the z88dk C compilers. It is not to be confused with other non-**z88dk** related projects with the same name.

**z80asm** is a relocatable assembler, linker and librarian that can assemble Intel 8080/8085 and Z80-family assembly files into a relocatable object format, can manage sets of object files in libraries and can build binary images by linking these object files together. The binary images can be defined in different sections, to match the target architecture.

**NOTE**: This document is a work-in-progress. It describes the functionality already working in the assembler in the **z80asm2** branch. This document and the `z80asm-future.md` will converge as **z80asm2** gets more features and converges to the current capability of **z80asm** in the **master** branch.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=1></a>

## 1. Usage ...


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=1_1></a>

### 1.1. ... as assembler

    z80asm [options] file...

By default, i.e. without any options, assemble each of the listed files into relocatable object files with a `.o` extension. 


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=1_2></a>

### 1.2. ... as linker

    z80asm -b [options] [-ilibrary.lib...] file...

Link the object files together and with any requested libraries into a set of binary files.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=1_3></a>

### 1.3. ... as librarian

    z80asm -xlibrary.lib [options] file...

Build a library containing all the object files passed as argument. That library can then be used during linking by specifying it with the `-i` option.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2></a>

## 2. Options


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_1></a>

### 2.1. Help Options


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_1_1></a>

#### 2.1.1. no arguments (show usage)

Show a help screen with the available options. 


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_1_2></a>

#### 2.1.2. -h, -?, --help (show manual)

Show this document. The output can be piped to `more` for pagination.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_1_3></a>

#### 2.1.3. -v, --verbose (show progress)

Show progress messages on `stdout`.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_2></a>

### 2.2. Code Generation Options


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_2_1></a>

#### 2.2.1. -mCPU, --cpu=CPU (select CPU)

Assemble for the given CPU. The following CPU's are supported:

| CPU      | Name                          |
| -------- | ----------------------------- |
| z80      | Zilog Z80                     |
| z180     | Zilog Z180                    |
| z80n     | ZX Next variant of the Z80    |
| gbz80    | GameBoy variant of the Z80    |
| 8080     | Intel 8080 (1)                |
| 8085     | Intel 8085 (1)                |
| r2k      | Rabbit RCM2000                |
| r3k      | Rabbit RCM3000                |
| ti83     | Texas Instruments TI83 (2)    |
| ti83plus | Texas Instruments TI83Plus (2)|

**Notes:**

**(1)** The Intel 8080 and 8085 are with Zilog or Intel mnemonics, except for the mnemonics that have different meanings, i.e.

| Intel | Zilog      | Comment                                     |
| ----- | ---------- | ------------------------------------------- |
| JP nn | JP P, nn   | Must use Zilog mnemonic, as JP is ambiguous |
| CP nn | CALL P, nn | Must use Zilog mnemonic, as CP is ambiguous |

**(2)** The Texas Instruments CPU's are standard Z80, but the `INVOKE` statement is assembled differently, i.e.

| CPU      | Statement | Assembled as       |
| ----     | --------- | ------------------ |
| ti83     | INVOKE nn | CALL nn            |
| ti83plus | INVOKE nn | RST 0x28 \ DEFW nn |


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_2_2></a>

#### 2.2.2. -IXIY, --IXIY (swap IX and IY)

Swap all occurrences of registers `IX` and `IY`, and also their 8-bit halves (`IXH`, `IXL`, `IYH` and `IYL`).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_2_3></a>

#### 2.2.3. --opt=speed (optimise for speed)

Replace all occurrences of `JR` by `JP`, as the later are faster. `DJNZ` is not replaced by `DEC B \ JP` as the later is slower.



----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3></a>

## 3. Copyright

The original z80asm module assembler was written by Gunther Strube. 
It was converted from QL SuperBASIC version 0.956, initially ported to Lattice C,
and then to C68 on QDOS.

It has been maintained since 2011 by Paulo Custodio.

Copyright (C) Gunther Strube, InterLogic 1993-99  
Copyright (C) Paulo Custodio, 2011-2020


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=4></a>

## 4. License

Artistic License 2.0 (http://www.perlfoundation.org/artisticlicense2_0)

<a id=keywords></a>


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5></a>

## 5. Keywords
 [-IXIY](#2_2_2) 
 [-h](#2_1_2) 
 [-mCPU](#2_2_1) 
 [-v](#2_1_3) 
<a id=index></a>


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6></a>

## 6. Index
- [1.](#1) Usage ...
  - [1.1.](#1_1) ... as assembler
  - [1.2.](#1_2) ... as linker
  - [1.3.](#1_3) ... as librarian
- [2.](#2) Options
  - [2.1.](#2_1) Help Options
    - [2.1.1.](#2_1_1) no arguments (show usage)
    - [2.1.2.](#2_1_2)  [-h](#2_1_2) , -?, --help (show manual)
    - [2.1.3.](#2_1_3)  [-v](#2_1_3) , --verbose (show progress)
  - [2.2.](#2_2) Code Generation Options
    - [2.2.1.](#2_2_1)  [-mCPU](#2_2_1) , --cpu=CPU (select CPU)
    - [2.2.2.](#2_2_2)  [-IXIY](#2_2_2) , - [-IXIY](#2_2_2)  (swap IX and IY)
    - [2.2.3.](#2_2_3) --opt=speed (optimise for speed)
- [3.](#3) Copyright
- [4.](#4) License
- [5.](#5) Keywords
- [6.](#6) Index
