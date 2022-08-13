# f28hs-2021-22-cwk-C-resit

Resit Coursework on a programming exercise (1 piece of CW, covering C and ARM Assembler programming).

The [CW specification is here](https://www.macs.hw.ac.uk/~hwloidl/Courses/F28HS/ResitCoursework.pdf)

This folder contains the CW specification template files for the source code and for the report:
- crack_rot.c ... top level C file as a template for the C programming part of the CW spec
- rotN.s ... template for the ARM programming part; the fct rotN should be callable from C
- test.sh ... a test script for testing the main CW spec (see below)

**Fork** and **Clone** this gitlab repo to get started on the coursework.

**Push** to the repo and ask questions in the comments box to get help.

You should implement the CW spec (above) by completing the code in `crack_rot.c` which contains a template structure.

You can build the C program, starting from the template in `crack_rot.c`, by typing
> make

and run the program with a hard-coded input by typing
> make run

and do unit testing on the matching function
> make test

**NB**: A separate gitlab repo contains the specification of the Assembler part of the Resit Coursework.

The final version of the code should be pushed to this repo, and also submitted through Vision, together with the report.

A test script is available to do unit-testing using the -f option of the application. Run it like this from the command line
> sh ./test.sh

To test whether all tests have been successful you can do
> echo $?

which should print `0`.

If you picked up the `.gitlab-ci.yml` file in this repo, this test will be done automatically when uploading the file and you will get either a Pass or Fail in the CI section of the gitlab-student server.


