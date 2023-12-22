# Complete Compiler for MICRO Language

Built a complete compiler from a lexer, parser, AST construction to code generation for a sun architecture simulator.
Performed optimizations such as CSE, data flow analysis and register allocation. Performed code generation
for loops, functions, if else statements, for statements. 

## Optimisations involved

  1. CSE
  2. Data flow analysis
  3. Liveness
  4. Register Allocation

# Building
A Makefile with the following targets:
  1. compiler: this target will build your compiler. (-1 for warnings)
  2. clean: this target will remove any intermediate files that were created to build the compiler.

# Tiny

Tiny is a very simple assembly code interpreter. 

Tiny simulates an architecture that has 4 data registers, a stack pointer (sp),
a frame pointer (fp) and both integer an floatingpoin arithmetic. All data
elements have size 1.

to build tiny: `g++ tiny4regs.C -o tiny'

# Steps to Run
  1. make compiler
  2. Compile tiny
  3. ./compiler input.micro -o out.a
  4. ./tiny out.a
