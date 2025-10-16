# endec

A C++ implementation of a RISC-V instruction encoder/decoder that converts assembly instructions into their corresponding machine code representations.

## Supported Instructions

For now, only the base RV32I instructions are supported.

## Assumptions

All instructions are valid (no pseudoinstructions)

Instructions are separated by newline characters

Instructions are in lowercase

Registers are named as x0, x1, etc.

Registers and immediate values are separated by commas only

## Usage

### Input Format

Create an input file `IN.txt` with RISC-V instructions, one per line:


```assembly
addi x0,x0,0
add x3,x3,x3
lw x1,4(x2)
beq x1,x2,label
```

### Compilation

```bash

g++ -std=c++20 -o encoder encoder.cpp
```

### Execution

```bash

./encoder
```

The encoded instructions will be stored in `OUT.txt`


## Some Limitations

There is currently no error handling for malformed instructions; be careful that the input is correct.
The program itself has also not gone through rigorous testing, so there may still be unfound errors in the program itself.

## Some notes

I decided to make my own encoder after finding only a few available online. The one I did find immediiately (https://luplab.gitlab.io/rvcodecjs/) was written in javascript and only usable as a website; it has a nice design, but I wished for a script able to encode/decode multiple instructions at once and on the command line. I'll add on more instructions if I see fit.