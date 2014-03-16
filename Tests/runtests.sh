#!/bin/bash

vg=false

./cleanAllTests.sh
echo -e "\n\n"

echo -e "\nTESTA - all 3 tests pass, gcc, one source file\n"
cp aprog/* .
if $vg; then
   valgrind ./Calculon SuiteA.suite
else
   ./Calculon SuiteA.suite
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTB - all 2 tests pass, gcc, multiple source files\n"
cp bprog/* .
if $vg; then
   valgrind ./Calculon SuiteB.suite
else
   ./Calculon SuiteB.suite
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTC - all 1 tests pass, makefile\n"
cp cprog/* .
if $vg; then
   valgrind ./Calculon SuiteC.suite
else
   ./Calculon SuiteC.suite
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTD - make failure, no target for dependency, bad make\n"
cp dprog/* .
if $vg; then
   valgrind ./Calculon SuiteD.suite
else
   ./Calculon SuiteD.suite
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -ne "\nTESTE - infinite loop, sometimes runtime err, "
echo -e "sometimes runtime error and timeout\n"
cp eprog/* .
if $vg; then
   valgrind ./Calculon SuiteE.suite
else
   ./Calculon SuiteE.suite
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -ne "\nTESTF - Missing source file. Expect memory leaks here due to "
echo -e "the hard exit\n"
cp fprog/* .
if $vg; then
   valgrind ./Calculon SuiteF.suite
else
   ./Calculon SuiteF.suite
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTG - bad diff for test2 and test3\n"
cp gprog/* .
if $vg; then
   valgrind ./Calculon SuiteG.suite
else
   ./Calculon SuiteG.suite
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTH - multiple tests. Edge cases fail (A - test1, C - test3)\n"
cp hprog/* .
if $vg; then
   valgrind ./Calculon SuiteH.suite
else
   ./Calculon SuiteH.suite
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\n\n"
