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
sub x7,x8,x9
srl x3,x4,x5
sb x10,20(x11)
slli x20,x21,24
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

### Example Output

```
01000000100101000000001110110011 // subtract
00000000010100100101000110110011 // shift right logical
00000000101001011000101000100011 // store byte
00000001100010101001101000010011 // shift left logical immediate
```

## Some Limitations

There is currently no error handling for malformed instructions; be careful that the input is correct.
The program itself has also not gone through rigorous testing, so there may still be unfound errors in the program itself.

## Some notes

I decided to make my own encoder after finding only a few available online. The one I did find immediately (https://luplab.gitlab.io/rvcodecjs/) was written in javascript and only usable as a website; it has a nice design, but I wished for a script able to encode/decode multiple instructions at once and on the command line. I'll add on more instructions if I see fit.