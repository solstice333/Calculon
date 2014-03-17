#!/bin/bash

vg=false

limit=$(($(ps -A | wc -l) + 10))

./cleanAllTests.sh
make -f mfCalculon all
echo -e "\n\n"

echo -e "\nTESTA - all 3 tests pass, gcc, one source file\n"
cp Tests/aprog/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./a.out SuiteA.suite
else
   ./SafeRun -p$limit ./a.out SuiteA.suite > outputCalculon.out
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTB - all 2 tests pass, gcc, multiple source files\n"
cp Tests/bprog/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./a.out SuiteB.suite
else
   ./SafeRun -p$limit ./a.out SuiteB.suite >> outputCalculon.out
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTC - all 1 tests pass, makefile\n"
cp Tests/cprog/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./a.out SuiteC.suite
else
   ./SafeRun -p$limit ./a.out SuiteC.suite >> outputCalculon.out
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTD - make failure, no target for dependency, bad make\n"
cp Tests/dprog/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./a.out SuiteD.suite
else
   ./SafeRun -p$limit ./a.out SuiteD.suite >> outputCalculon.out
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -ne "\nTESTE - infinite loop, sometimes runtime err, "
echo -e "sometimes runtime error and timeout\n"
cp Tests/eprog/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./a.out SuiteE.suite
else
   ./SafeRun -p$limit ./a.out SuiteE.suite >> outputCalculon.out
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -ne "\nTESTF - Missing source file. Expect memory leaks here due to "
echo -e "the hard exit\n"
cp Tests/fprog/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./a.out SuiteF.suite
else
   ./SafeRun -p$limit ./a.out SuiteF.suite >> outputCalculon.out
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTG - bad diff for test2 and test3\n"
cp Tests/gprog/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./a.out SuiteG.suite
else
   ./SafeRun -p$limit ./a.out SuiteG.suite >> outputCalculon.out
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTESTH - multiple tests. Edge cases fail (A - test1, C - test3)\n"
cp Tests/hprog/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./a.out SuiteH.suite
else
   ./SafeRun -p$limit ./a.out SuiteH.suite >> outputCalculon.out
   echo -e "Return value: $?"
fi
./cleanAllTests.sh

echo -e "\nTEST1\n"
cp Tests/Suite1/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./a.out Suite1.suite
else
   ./SafeRun -p$limit ./a.out Suite1.suite > Suite1.out.k
   echo -e "Return value: $?"
   diff Suite1.out.k Tests/Suite1.out
fi
./cleanAllTests.sh

echo -e "\nTEST2\n"
cp Tests/Suite2/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./a.out Suite2.suite
else
   ./SafeRun -p$limit ./a.out Suite2.suite > Suite2.out.k
   echo -e "Return value: $?"
   diff Suite2.out.k Tests/Suite2.out
fi
./cleanAllTests.sh


echo -e "\n\n"
make -f mfCalculon clean
