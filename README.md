# Shogun

Welcome to the Shogun project! Shogun is a virtual machine
with plans to incorporate a simple language and compiler
as well. The project is currently only tested on windows,
but it should build on any platform and there are plans to
increase cross-platform compatibility.

# Projects

libsvm: The Shogun virtual machine

tests: Unit tests using gtest

# Architecture

SVM's architecture is made to be fairly simple. It is a stack machine with
three main places that data can be stored: The stack, memory (the heap),
and registers.

## The Stack

The stack is just as it sounds: a stack data structure. Three operations can
be executed on it: push, pop, and peek. Pushing places data on the top of the
stack. Popping removes data from the stack. Peeking shows data on the stack
without removing it. Generally, all opcodes take their arguments from the stack,
with a few exceptions.

## The heap

The heap (known just as "memory" internally) is where the bulk of your data is
stored, including the program itself. The heap is a continuous block of memory that
is split up into segments and can be resized at will.

The first segment of memory is always made up of a single slot and should always contain
a nil value. This segment, appropriately, is always at address 0.

The second segment of memory contains your program, and is generally bounded by a HALT
opcode to prevent SVM from trying to read unrelated data as part of the program itself.

The third segment of memory is extra data that comes bundled with your program. Often
it will be used as the constant table. Generally a constant table is used to store
strings, since they can be fairly long and if they are repeated in the code they can
increase the file size. SVM doesn't enforce any specific way to use this segment,
so it is up to the programmer on how to use it.

The fourth and final segment of memory is data created at runtime.

There is no real segmentation to the heap, but generally it can be thought of being split
into four parts. Sometimes you might want to be able to rewrite the program at runtime,
so the program might spill over from the second into the fourth segment, and sometimes
you might write back into the second segment. SVM doesn't put any restrictions on doing
such things.

## Registers

SVM contains no general purpose registers, but does contain a few registers that have
specific purposes.

### MMX

The MMX register is used to store the current local heap offset. When the program first
starts, MMX is set to the address directly after the program. From there, MMX can be
used however the programmer desires, but generally it exists to denote where the current
local scope is.

### PRI

The PRI register is the program index. It stores the address of the current instruction.