# Shogun

Welcome to the Shogun project! Shogun is a virtual machine
with plans to incorporate a simple language and compiler
as well. The project is currently only tested on windows,
but it should build on any platform and there are plans to
increase cross-platform compatibility.

# Projects

libsvm: The Shogun virtual machine

tests: Unit tests using gtest

shogun: ShogunVM executable

shasm: ShogunVM assembler

sholan: Compiler for sholan->shasm

# WARNING

The current state of sholan is that many things are, in fact, broken. Memory is used as a single giant stack, which makes heap operations impossible, and generated code is littered with bugs. Shogun and the VM itself is generally fine, but it isn't recommended to use Sholan at this time.
