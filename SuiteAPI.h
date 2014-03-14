#ifndef SUITEAPI_H
#define SUITEAPI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SIZE 128

typedef struct Program {
   char name[DEFAULT_SIZE];
   char *src[DEFAULT_SIZE];
   int idx;
} Program;

typedef struct Test {
   char inFile[DEFAULT_SIZE];
   char outFile[DEFAULT_SIZE];
   int timeout;
   char *args[DEFAULT_SIZE];
   int idx;
} Test;

Program *ProgramCreate();
void ProgramSetName(Program *p, char *name);
void ProgramAddSource(Program *p, char *src);
void ProgramPrintContents(Program *p);
void ProgramDelete(Program *p);

Test *TestCreate();
void TestSetInFile(Test *t, char *in);
void TestSetOutFile(Test *t, char *out);
void TestSetTimeout(Test *t, int ms);
void TestAddArg(Test *t, char *arg);
void TestPrintContents(Test *t);
void TestDelete(Test *t);

#endif
