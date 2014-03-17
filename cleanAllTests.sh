#!/bin/bash

rm -rf aprog.c bprog.c cprog.c dprog.c eprog.c fprog.c gprog.c SmartAlloc.* Makefile header.h test*.in test*.out

for i in `seq 1 8`; do
   for f in $(ls Tests/Suite$i); do
      rm -rf $f
   done
done
