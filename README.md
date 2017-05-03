### c-nice-sigsegv

Small library which captures segmentation faults on native code, outputs the backtrace and the line that caused the fault which can compiled as a static library and used like this:

```bash
$ make
$ LD_PRELOAD=./segfault.so ./experiment # experiment is a small C program that segfaults
```
and it will output something like

```
========== The program crashed =========
==> context
0x4004b6
/home/javierhonduco/c-nice-sigsegv/experiment.c:3

==> offending sigsegv line
*faulty = 314;
==> stacktrace
./segfault.so(print_stacktrace+0x86) [0x7f8e2e286bb6]
./segfault.so(segfault_handler+0x183) [0x7f8e2e286d9b]
/usr/lib/libc.so.6(+0x330b0) [0x7f8e2df1b0b0]
./experiment() [0x4004b6]
/usr/lib/libc.so.6(__libc_start_main+0xf1) [0x7f8e2df08291]
./experiment() [0x4003da]
========================================
```

_**Warning!**_ I just made this for learning purposes, it's probably not safe to use and it's not portable either. Only tested in a Linux 4.9.11 box w/ GCC version 6.3.1 installed.
