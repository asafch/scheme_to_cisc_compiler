# Scheme to CISC compiler

This is part of a 3-part project to create a Scheme-to-CISC compiler, in a compiler principles course taken in BGU. See [part 1](https://github.com/asafch/scheme_to_cisc_part1) and [part 2](https://github.com/asafch/scheme_to_cisc_part2).

The objective of the final project is to create a fully functional compiler, that compiles Scheme code into a mock-up assembly code. This mock-up assembly code is just `C`-macros inside a `.c` file, which is then compiled with `gcc`. The output of the compiled `*.o` file should be the same as the output received in `Chez Scheme` when evaluating the original Scheme code input to the compiler.
Compiling the output with GCC is mandatory, since the macros rely on GCC's extension that enables jumping to lables in the `C` code.

See [assignment description](http://www.cs.bgu.ac.il/~comp161/wiki.files/final-project.html).

The compiler is implemented in [OCaml](https://ocaml.org).

Parsing Combinators are used in the implementation, and the base PC functionality
resides in `pc.ml`, which was written by Prof. Mayer Goldberg.

The definition and basic files for using the mock-up assembly are also the work of Dr. Goldberg.

## Usage
1. Fire up OCaml, then: `#use compiler.ml;;`
2. Have your input in some file, say `input.scm`
3. Compile the input with `Code_Gen.compile_scheme_file “input.scm” “out.c”;;`
    * NOTE: if you wish to name the output file differently, make sure to update the new name in `makefile`.
4. Exit OCaml and compile `out.c` with `make`.
5. Run the compiled `C` file: `./out` and have fun!

### License

The MIT License (MIT)

Copyright (c) 2015 Asaf Chelouche, Avi Gurfinkel

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
