#!/bin/bash

if [ $# -eq 0 ]; then
   echo "Usage: addRules.sh [ file1 file2 ... ]"
   echo "where |file1| is the name of the source file without the .c extension"
   exit 1
fi

echo "CC=gcc" > Makefile
echo "FLAGS=-g" >> Makefile

echo -ne "FILES=" >> Makefile
for i in $*; do
   echo -ne "$i.c " >> Makefile
done
echo >> Makefile

echo -ne "OBJ=" >> Makefile
for i in $*; do
   echo -ne "$i.o " >> Makefile
done
echo >> Makefile

echo "OUT=a.out" >> Makefile
echo >> Makefile

echo "all: build" >> Makefile
echo >> Makefile

echo 'build: $(OBJ)' >> Makefile 
echo '	$(CC) $(FLAGS) $(OBJ)' >> Makefile
echo >> Makefile

for i in $*; do
   gcc -MM $i.c >> Makefile
   echo '	$(CC) $(FLAGS) -c ' $i.c >> Makefile
   echo >> Makefile
done

echo "clean:" >> Makefile 
echo '	rm -rf $(OBJ) $(OUT)' >> Makefile	
echo >> Makefile

echo "rebuild: clean build" >> Makefile
