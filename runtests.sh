#!/bin/bash

vg=true

./cleanAllTests.sh
make -f mfCalculon rebuild

echo -e "TESTA\n"
cp Tests/aprog/* .
if $valgrind; then
   valgrind ./a.out SuiteA.suite
else
   ./a.out SuiteA.suite
fi
./cleanAllTests.sh

echo -e "TESTB\n"
cp Tests/bprog/* .
if $valgrind; then
   valgrind ./a.out SuiteB.suite
else
   ./a.out SuiteB.suite
fi
./cleanAllTests.sh

echo -e "TESTC\n"
cp Tests/cprog/* .
if $valgrind; then
   valgrind ./a.out SuiteC.suite
else
   ./a.out SuiteC.suite
fi
./cleanAllTests.sh

echo -e "TESTD\n"
cp Tests/dprog/* .
if $valgrind; then
   valgrind ./a.out SuiteD.suite
else
   ./a.out SuiteD.suite
fi
./cleanAllTests.sh

echo -e "TESTE\n"
cp Tests/eprog/* .
if $valgrind; then
   valgrind ./a.out SuiteE.suite
else
   ./a.out SuiteE.suite
fi
./cleanAllTests.sh

make -f mfCalculon clean
