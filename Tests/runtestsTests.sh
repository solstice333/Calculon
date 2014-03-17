#!/bin/bash

vg=false

limit=$(($(ps -A | wc -l) + 10))

./cleanAllTests.sh
echo -e "\n\n"

echo -e "\nTESTA - all 3 tests pass, gcc, one source file\n"
cp aprog/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./Calculon SuiteA.suite
else
   ./SafeRun -p$limit ./Calculon SuiteA.suite > outputTests.out
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTB - all 2 tests pass, gcc, multiple source files\n"
cp bprog/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./Calculon SuiteB.suite
else
   ./SafeRun -p$limit ./Calculon SuiteB.suite >> outputTests.out 
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTC - all 1 tests pass, makefile\n"
cp cprog/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./Calculon SuiteC.suite
else
   ./SafeRun -p$limit ./Calculon SuiteC.suite >> outputTests.out
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTD - make failure, no target for dependency, bad make\n"
cp dprog/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./Calculon SuiteD.suite
else
   ./SafeRun -p$limit ./Calculon SuiteD.suite >> outputTests.out
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -ne "\nTESTE - infinite loop, sometimes runtime err, "
echo -e "sometimes runtime error and timeout\n"
cp eprog/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./Calculon SuiteE.suite
else
   ./SafeRun -p$limit ./Calculon SuiteE.suite >> outputTests.out
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -ne "\nTESTF - Missing source file. Expect memory leaks here due to "
echo -e "the hard exit\n"
cp fprog/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./Calculon SuiteF.suite
else
   ./SafeRun -p$limit ./Calculon SuiteF.suite >> outputTests.out
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTG - bad diff for test2 and test3\n"
cp gprog/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./Calculon SuiteG.suite
else
   ./SafeRun -p$limit ./Calculon SuiteG.suite >> outputTests.out
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTH - multiple tests. Edge cases fail (A - test1, C - test3)\n"
cp hprog/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./Calculon SuiteH.suite
else
   ./SafeRun -p$limit ./Calculon SuiteH.suite >> outputTests.out
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\n\n"
