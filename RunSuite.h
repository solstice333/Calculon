#ifndef SUITEAPI_H
#define SUITEAPI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#define DEFAULT_SIZE 128
#define MULT 10

// TODO change this when running on vogon
//#define P30 "-p30"
#define P30 "-p200"
//#define SAFERUN "/home/grade_cstaley/bin/SafeRun"
#define SAFERUN "../SafeRun"

#define CP "/bin/cp"
#define GCC "/usr/bin/gcc"
#define MAKE "/usr/bin/make"
#define DIFF "/usr/bin/diff"
#define OUTPUT_OPT "-o"
#define W 1
#define R 0
#define BUFSIZE 1024
#define TARGET 9
#define MAKE_FAIL 2
#define GCC_FAIL 1

/* Typedef "struct dirent" with "dirent" */
typedef struct dirent dirent;

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

/* Struct for holding data related to cases involving failure output or 
*  failure exit status' for SafeRun and/or diff (excluding build failures 
*  associating to gcc or Make)
*/
typedef struct Failure {
   int fail;
   int diff, runtime, timeout;
} Failure;

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

/* Build an argument list for the SafeRun utility, where Program |*p|
*  contains the program you want SafeRun to execute, Test |*tests[]|
*  contains all the Test objects associated to Program |*p|, and |testNum|
*  is the current index of interest in |*tests[]| */
char **buildSrArgs(Program *p, Test *tests[], int testNum);

/* Makes new directory with name being the parent process id of Calculon
*  and moves all necessary content to that directory for testing
*/
char *mkDirMvTests(Program *p, Test *tests[], int numTests);

/* Removes the directory specified by |*path| and all of its contents.
*  The contents must be files, not directories
*/
void rmDirRmTests(char *path);

/* Runs Make for Program |*p| to be tested. If no makefile exists, runs
*  gcc for |*p| instead. Checks for GNUmakefile, makefile, and Makefile
*  in that order. Returns a nonzero value if Make has failed and an error
*  message to stderr. Returns 0 if successful.
*/
int runGccMake(Program *p);

/* Prints failure messages for program |*name| that's being tested at test
*  iteration |tnum| with the given Failure object |*fail| that contains
*  failure data as defined in the Failure struct at the top of this header 
*  file
*/
void printFailure(char *name, int tnum, Failure *fail);

/* Prints success message for program |*name| that's being tested if
*  the total number of failures throughout the tests |totalFailures| is 0. 
*  |totalTests| contain the maximum number of tests that was done for 
*  program |*name|.
*/
void printSuccess(char *name, int totalFailures, int totalTests);

/* Silence messages written to |fd| from programs called using exec where
*  |fd| is usually stdout (1) or stderr (2) - whichever stream you want to
*  silence
*/
void silence(int fd);

/* This function will check for open file descriptors up to fd |n|, and then
*  exit immediately. For debugging purposes only.
*/
void checkOpenFds(int n);

/* Build an argument list for the diff utility. The program being tested 
*  redirects its stdout to the actual output file |*act|. Likewise, |*exp| is 
*  the name of the expected output file to be diff'ed against. 
*/
char **buildDiffArgs(char *act, char *exp);

#endif
