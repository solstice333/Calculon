#!/bin/bash

vg=false

./cleanAllTests.sh
make -f mfCalculon rebuild
echo -e "\n\n"

echo -e "\nTESTA - all 3 tests pass, gcc, one source file\n"
cp Tests/aprog/* .
if $vg; then
   valgrind ./a.out SuiteA.suite
else
   ./a.out SuiteA.suite
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTB - all 2 tests pass, gcc, multiple source files\n"
cp Tests/bprog/* .
if $vg; then
   valgrind ./a.out SuiteB.suite
else
   ./a.out SuiteB.suite
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTC - all 1 tests pass, makefile\n"
cp Tests/cprog/* .
if $vg; then
   valgrind ./a.out SuiteC.suite
else
   ./a.out SuiteC.suite
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTD - make failure, no target for dependency, bad make\n"
cp Tests/dprog/* .
if $vg; then
   valgrind ./a.out SuiteD.suite
else
   ./a.out SuiteD.suite
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -ne "\nTESTE - infinite loop, sometimes runtime err, "
echo -e "sometimes runtime error and timeout\n"
cp Tests/eprog/* .
if $vg; then
   valgrind ./a.out SuiteE.suite
else
   ./a.out SuiteE.suite
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -ne "\nTESTF - Missing source file. Expect memory leaks here due to "
echo -e "the hard exit\n"
cp Tests/fprog/* .
if $vg; then
   valgrind ./a.out SuiteF.suite
else
   ./a.out SuiteF.suite
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTG - bad diff for test2 and test3\n"
cp Tests/gprog/* .
if $vg; then
   valgrind ./a.out SuiteG.suite
else
   ./a.out SuiteG.suite
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTH - multiple tests. Edge cases fail (A - test1, C - test3)\n"
cp Tests/hprog/* .
if $vg; then
   valgrind ./a.out SuiteH.suite
else
   ./a.out SuiteH.suite
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\n\n"
make -f mfCalculon clean
