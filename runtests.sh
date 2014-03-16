#!/bin/bash

make -f mfCalculon rebuild

echo -e "TESTA\n"
cp Tests/aprog/* .
./a.out SuiteA.suite
./cleanAllTests.sh

echo -e "TESTB\n"
cp Tests/bprog/* .
./a.out SuiteB.suite
./cleanAllTests.sh

echo -e "TESTC\n"
cp Tests/cprog/* .
./a.out SuiteC.suite
./cleanAllTests.sh

echo -e "TESTD\n"
cp Tests/dprog/* .
./a.out SuiteD.suite
./cleanAllTests.sh

echo -e "TESTE\n"
cp Tests/eprog/* .
./a.out SuiteE.suite
./cleanAllTests.sh

make -f mfCalculon clean
