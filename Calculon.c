#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "SuiteAPI.h"

#define DEFAULT_SIZE 128
#define MAX_WORD "%127s"
#define LINE_SIZE 512
#define MAX_TESTS 128

#define DEBUG 1

/* If pch (pointer to character) is null, bad suite definition file. In this
*  case, exit(1) is called. This is necessary to make sure that the suite
*  definition file contains a test input, test output, and timeout
*/
void checkpch(char *pch);

/* Cleans up a Program object |*p| and all the Test objects within the 
*  container |*tests[]| where the amount of Test objects within the container 
*  is tracked by |*numTests|. The target pointed to by |*numTests| is reset
*  to 0.
*/
void teardownProgramTests(Program *p, Test *tests[], int *numTests);

/* Runs the tests |*tests[]| for Program |*p|. |numTests| is the amount of
*  tests that exist within |*tests[]| 
*/
void runtests(Program *p, Test *tests[], int numTests);


int main(int argc, char **argv) {
   // setup
   if (argc != 2) {
      fprintf(stderr, "Usage: Calculon [Suite file]\n");
      fprintf(stderr, "Where |suite file| is a file" 
       " with a .suite extension\n");
   }

   FILE *suite = fopen(*++argv, "r");
   if (!suite) {
      fprintf(stderr, "Error: Suite file could not be found. Exiting.\n");
      exit(1);
   }

   char line[LINE_SIZE];
   char safecopy[DEFAULT_SIZE];
   Program *p = NULL;
   Test *tests[MAX_TESTS];
   int i, numTests = 0;

   // body
   while (fgets(line, LINE_SIZE, suite)) {
      char *pch = strtok(line, " \t\n");

      if (pch) {
         if (!strcmp(pch, "P")) {   // Create Program object
            if (p) {
               runtests(p, tests, numTests);
               teardownProgramTests(p, tests, &numTests);
            }

            p = ProgramCreate();
            pch = strtok(NULL, " \t\n");
            sscanf(pch, MAX_WORD, safecopy); 
            ProgramSetName(p, safecopy); 

            pch = strtok(NULL, " \t\n");
            while (pch) {
               sscanf(pch, MAX_WORD, safecopy); 

               if (safecopy[strlen(safecopy) - 1] == 'c') 
                  ProgramAddSrc(p, safecopy);
               else if (safecopy[strlen(safecopy) - 1] == 'h')
                  ProgramAddHeader(p, safecopy);

               pch = strtok(NULL, " \t\n");
            }
         }
         else if (!strcmp(pch, "T")) { // Create Test object
            if (!p) {
               fprintf(stderr, "Error: Bad Suite Definition File. Exiting\n");
               exit(1);
            }
            tests[numTests] = TestCreate();

            pch = strtok(NULL, " \t\n");
            checkpch(pch);
            sscanf(pch, MAX_WORD, safecopy); 
            TestSetInFile(tests[numTests], safecopy);

            pch = strtok(NULL, " \t\n");
            checkpch(pch);
            sscanf(pch, MAX_WORD, safecopy); 
            TestSetOutFile(tests[numTests], safecopy);

            pch = strtok(NULL, " \t\n");
            checkpch(pch);
            sscanf(pch, MAX_WORD, safecopy); 
            TestSetTimeout(tests[numTests], atoi(safecopy));

            pch = strtok(NULL, " \t\n");
            while (pch) {
               sscanf(pch, MAX_WORD, safecopy); 
               TestAddArg(tests[numTests], safecopy);
               pch = strtok(NULL, " \t\n");
            }

#if DEBUG
            ProgramPrintContents(p);
            TestPrintContents(tests[numTests]);
#endif
            numTests++;
         }
         else {
            fprintf(stderr, "Error: Bad Suite Definition File. Exiting\n");
            exit(1);
         }

#if DEBUG
         printf("\n");
#endif
      }
   }

   // teardown
   runtests(p, tests, numTests);
   teardownProgramTests(p, tests, &numTests);
   fclose(suite);

   return 0;
}

void checkpch(char *pch) {
   if (!pch) {
      fprintf(stderr, "Error: Bad Suite Definition File. Exiting\n");
      exit(1);
   }
}

void teardownProgramTests(Program *p, Test *tests[], int *numTests) {
   ProgramDelete(p);

   int i;
   for (i = 0; i < *numTests; i++) 
      TestDelete(tests[i]);
   *numTests = 0;
}

void runtests(Program *p, Test *tests[], int numTests) {
   int i;
   char *path;

   path = mkDirMvTests(p, tests, numTests);

   chdir(path);
   runGccMake(p);

   for (i = 0; i < numTests; i++) {
      int inFd = open(tests[i]->inFile, O_RDONLY);
      char outFileK[DEFAULT_SIZE + 2];
      sprintf(outFileK, "%s.k", tests[i]->outFile);
      int outFd = creat(outFileK, 0644);

      pid_t cpid = fork();
      if (cpid < 0)
         fprintf(stderr, "Error: Something forked up\n");
      else if (cpid > 0) {
         close(inFd);
         close(outFd);
         wait(NULL);
      }
      else {
         dup2(inFd, 0);
         close(inFd);
         dup2(outFd, 1);
         close(outFd);

         execv(SAFERUN, buildSrArgs(p, tests, i));
      }
   }

   chdir("..");
   // rmDirRmTests(path);
}
