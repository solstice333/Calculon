#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "SuiteAPI.h"

#define WORD_SIZE 128
#define LINE_SIZE 512
#define MAX_TESTS 128
#define SAFERUN "./SafeRun"

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
      fprintf(stderr, "Suite file could not be found. Exiting.\n");
      exit(1);
   }

   char line[LINE_SIZE];
   Program *p = NULL;
   Test *tests[MAX_TESTS];
   int i, numTests = 0;

   // body
   while (fgets(line, LINE_SIZE, suite)) {
      char *pch = strtok(line, " \t\n");

      if (pch) {
         if (*pch == 'P') {   // Create Program object
            if (p) {
               runtests(p, tests, numTests);
               teardownProgramTests(p, tests, &numTests);
            }

            p = ProgramCreate();
            pch = strtok(NULL, " \t\n");
            ProgramSetName(p, pch); 

            pch = strtok(NULL, " \t\n");
            while (pch) {
               if (pch[strlen(pch) - 1] == 'c') 
                  ProgramAddSrc(p, pch);

               pch = strtok(NULL, " \t\n");
            }
         }
         else if (*pch == 'T') { // Create Test object
            if (!p) {
               printf("Bad suite file. Exiting\n");
               exit(1);
            }
            tests[numTests] = TestCreate();
            pch = strtok(NULL, " \t\n");
            TestSetInFile(tests[numTests], pch);
            pch = strtok(NULL, " \t\n");
            TestSetOutFile(tests[numTests], pch);
            pch = strtok(NULL, " \t\n");
            TestSetTimeout(tests[numTests], atoi(pch));

            pch = strtok(NULL, " \t\n");
            while (pch) {
               TestAddArg(tests[numTests], pch);
               pch = strtok(NULL, " \t\n");
            }

            ProgramPrintContents(p);
            TestPrintContents(tests[numTests]);
            numTests++;
         }
         else {
            printf("Bad suite file. Exiting\n");
            exit(1);
         }

         printf("\n");
      }
   }

   // teardown
   runtests(p, tests, numTests);
   teardownProgramTests(p, tests, &numTests);
   fclose(suite);

   return 0;
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

   makeDirMoveTests(p, tests, numTests);

   for (i = 0; i < numTests; i++) {
      pid_t cpid = fork();
      if (cpid < 0)
         fprintf(stderr, "Something forked up\n");
      else if (cpid > 0) 
         wait(NULL);
      else {
         printf("\ntest %d\n", i);
         printSrArgs(buildSrArgs(p, tests, i));
         // execv(SAFERUN, buildSrArgs(p, tests, i);
         exit(0);
      }
   }
}
