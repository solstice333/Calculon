#!/bin/bash

vg=false
ignore=true

limit=$(($(ps -A | wc -l) + 10))
#limit=30

./cleanAllTests.sh
make -f mfCalculon rebuild
echo -e "\n\n"

if ! $ignore; then
   echo -e "\nTESTA - all 3 tests pass, gcc, one source file\n"
   cp Tests/aprog/* .
   if $vg; then
      valgrind ./SafeRun -p$limit ./a.out SuiteA.suite
   else
      ./SafeRun -p$limit ./a.out SuiteA.suite &> aprog.out.k
      echo -e "Return value: $?"
      diff aprog.out.k Tests/aprog.out
   fi
   ./cleanAllTests.sh

   echo -e "\nTESTB - all 2 tests pass, gcc, multiple source files\n"
   cp Tests/bprog/* .
   if $vg; then
      valgrind ./SafeRun -p$limit ./a.out SuiteB.suite
   else
      ./SafeRun -p$limit ./a.out SuiteB.suite &> bprog.out.k
      echo -e "Return value: $?"
      diff bprog.out.k Tests/bprog.out
   fi
   ./cleanAllTests.sh

   echo -e "\nTESTC - all 1 tests pass, makefile\n"
   cp Tests/cprog/* .
   if $vg; then
      valgrind ./SafeRun -p$limit ./a.out SuiteC.suite
   else
      ./SafeRun -p$limit ./a.out SuiteC.suite &> cprog.out.k
      echo -e "Return value: $?"
      diff cprog.out.k Tests/cprog.out
   fi
   ./cleanAllTests.sh

   echo -e "\nTESTD - make failure, no target for dependency, bad make\n"
   cp Tests/dprog/* .
   if $vg; then
      valgrind ./SafeRun -p$limit ./a.out SuiteD.suite
   else
      ./SafeRun -p$limit ./a.out SuiteD.suite &> dprog.out.k
      echo -e "Return value: $?"
      diff dprog.out.k Tests/dprog.out
   fi
   ./cleanAllTests.sh

   echo -ne "\nTESTE - infinite loop, sometimes runtime err, "
   echo -e "sometimes runtime error and timeout\n"
   cp Tests/eprog/* .
   if $vg; then
      valgrind ./SafeRun -p$limit ./a.out SuiteE.suite
   else
      ./SafeRun -p$limit ./a.out SuiteE.suite &> eprog.out.k
      echo -e "Return value: $?"
      diff eprog.out.k Tests/eprog.out
   fi
   ./cleanAllTests.sh

   echo -ne "\nTESTF - Missing source file. Expect memory leaks here due to "
   echo -e "the hard exit\n"
   cp Tests/fprog/* .
   if $vg; then
      valgrind ./SafeRun -p$limit ./a.out SuiteF.suite
   else
      ./SafeRun -p$limit ./a.out SuiteF.suite &> fprog.out.k
      echo -e "Return value: $?"
      diff fprog.out.k Tests/fprog.out
   fi
   ./cleanAllTests.sh

   echo -e "\nTESTG - bad diff for test2 and test3\n"
   cp Tests/gprog/* .
   if $vg; then
      valgrind ./SafeRun -p$limit ./a.out SuiteG.suite
   else
      ./SafeRun -p$limit ./a.out SuiteG.suite &> gprog.out.k
      echo -e "Return value: $?"
      diff gprog.out.k Tests/gprog.out
   fi
   ./cleanAllTests.sh

   echo -e "\nTESTH - multiple tests. Edge cases fail (A - test1, C - test3)\n"
   cp Tests/hprog/* .
   if $vg; then
      valgrind ./SafeRun -p$limit ./a.out SuiteH.suite
   else
      ./SafeRun -p$limit ./a.out SuiteH.suite &> hprog.out.k
      echo -e "Return value: $?"
      diff hprog.out.k Tests/hprog.out
   fi
   ./cleanAllTests.sh
fi

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

echo -e "\nTEST3\n"
cp Tests/Suite3/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./a.out Suite3.suite
else
   ./SafeRun -p$limit ./a.out Suite3.suite > Suite3.out.k
   echo -e "Return value: $?"
   diff Suite3.out.k Tests/Suite3.out
fi
./cleanAllTests.sh

echo -e "\nTEST4\n"
cp Tests/Suite4/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./a.out Suite4.suite
else
   ./SafeRun -p$limit ./a.out Suite4.suite > Suite4.out.k
   echo -e "Return value: $?"
   diff Suite4.out.k Tests/Suite4.out
fi
./cleanAllTests.sh

echo -ne "\nTEST5 - Forcing redirection of stderr and stdout to file" 
echo -e " in bash due to inability of piping outer-most SafeRun\n"
cp Tests/Suite5/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./a.out Suite5.suite
else
   # Note the redirection of stderr and stdout to file
   ./SafeRun -p$limit ./a.out Suite5.suite &> Suite5.out.k
   echo -e "Return value: $?"
   diff Suite5.out.k Tests/Suite5.out
fi
./cleanAllTests.sh

echo -ne "\nTEST6 - Compilation differences due to different gcc versions? "
echo -ne "Calculon output is the same however. Forcing redirection of "
echo -ne "stderr and stdout to file in bash. I don't want to pipe gcc "
echo -e "stderr to the Calculon parent. That's retarded.\n"
cp Tests/Suite6/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./a.out Suite6.suite
else
   ./SafeRun -p$limit ./a.out Suite6.suite &> Suite6.out.k
   echo -e "Return value: $?"
   diff Suite6.out.k Tests/Suite6.out
fi
./cleanAllTests.sh

echo -ne "\nTEST7 - Compilation differences due to different gcc versions? "
echo -ne "Calculon output is the same however. Forcing redirection of "
echo -ne "stderr and stdout to file in bash. I don't want to pipe gcc "
echo -e "stderr to the Calculon parent. That's retarded.\n"
cp Tests/Suite7/* .
if $vg; then
   valgrind ./SafeRun -p$limit ./a.out Suite7.suite
else
   ./SafeRun -p$limit ./a.out Suite7.suite &> Suite7.out.k
   echo -e "Return value: $?"
   diff Suite7.out.k Tests/Suite7.out
fi
./cleanAllTests.sh

echo -ne "\nTEST8 - Compilation differences due to different gcc versions? "
echo -ne "Calculon output is the same however. Forcing redirection of "
echo -ne "stderr and stdout to file in bash. I don't want to pipe gcc "
echo -e "stderr to the Calculon parent. That's retarded.\n"
cp Tests/Suite8/* .
if $vg; then
valgrind ./SafeRun -p$limit ./a.out Suite8.suite
else
./SafeRun -p$limit ./a.out Suite8.suite &> Suite8.out.k
echo -e "Return value: $?"
diff Suite8.out.k Tests/Suite8.out
fi
./cleanAllTests.sh

echo -e "\n\n"
make -f mfCalculon clean
