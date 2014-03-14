#ifndef SUITEAPI_H
#define SUITEAPI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define DEFAULT_SIZE 128
#define MULT 10
#define P30 "-p30"
#define SAFERUN "./SafeRun"
#define CP "/bin/cp"

/* Program holds the data on the line prefixed with a 'P' in the 
*  Suite Definition File  
*/
typedef struct Program {
   char name[DEFAULT_SIZE];
   char *src[DEFAULT_SIZE];
   char *header[DEFAULT_SIZE];
   int idxSrc;
   int idxHeader;
} Program;

/* Test holds the data on the line prefixed with a 'T' in the Suite
*  Definition File
*/
typedef struct Test {
   char inFile[DEFAULT_SIZE];
   char outFile[DEFAULT_SIZE];
   int timeout;
   char *args[DEFAULT_SIZE];
   int idx;
} Test;

/* Initializes a Program object */
Program *ProgramCreate();

/* Sets the name of the executable to be ran for Program |*p| */
void ProgramSetName(Program *p, char *name);

/* Adds source files to the list contained within Program |*p| 
*  to use with gcc 
*/
void ProgramAddSource(Program *p, char *src);

/* Adds header files to the list contained within Program |*p|
*  to use with gcc
*/
void ProgramAddHeader(Program *p, char *header);

/* Prints the contents of Program |*p| */
void ProgramPrintContents(Program *p);

/* Destroys Program |*p| */
void ProgramDelete(Program *p);

/* Initializes a Test object */
Test *TestCreate();

/* Sets the name of the test file |*in| in Test |*t| used with input 
* redirection for the executable that's to be tested
*/
void TestSetInFile(Test *t, char *in);

/* Sets the name of the test file |*out| in Test |*t| used with output 
*  redirection for the executable that's to be tested
*/
void TestSetOutFile(Test *t, char *out);

/* Sets the maximum time in milliseconds |ms| before a timeout error is
*  to occurr for Test |*t|
*/
void TestSetTimeout(Test *t, int ms);

/* Adds arguments |*arg| to the argument list associated to Test |*t| 
*  that is to be sent to the executable to be tested
*/
void TestAddArg(Test *t, char *arg);

/* Prints the contents of the Test object |*t| */
void TestPrintContents(Test *t);

/* Destroys Test |*t| */
void TestDelete(Test *t);

/* Build an argument list for the SafeRun executable */
char **buildSrArgs(Program *p, Test *tests[], int testNum);

/* Prints argument list |**args| */
void printSrArgs(char **args); 

/* Makes new directory with name being the parent process id of Calculon
*  and moves all necessary content to that directory for testing
*/
void makeDirMoveTests(Program *p, Test *tests[], int numTests);

#endif
