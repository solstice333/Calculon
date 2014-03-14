#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SuiteAPI.h"

#define WORD_SIZE 128
#define LINE_SIZE 512
#define MAX_TESTS 128

/* cleans up a Program object |p| and all the Test objects within the container
*  |tests| where the amount of Test objects within the container is tracked by
*  |*numTests|
*/
void teardownProgramAndTests(Program *p, Test *tests[], int *numTests);

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
   Test *t[MAX_TESTS];
   int i, numTests = 0;

   // body
   while (fgets(line, LINE_SIZE, suite)) {
      char *pch = strtok(line, " \t\n");

      if (pch) {
         if (*pch == 'P') {   // Create Program object
            if (p) {
               // do tests, teardown, reset
               printf("Doing tests... Tests done!\n");
               teardownProgramAndTests(p, t, &numTests);
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
            t[numTests] = TestCreate();
            pch = strtok(NULL, " \t\n");
            TestSetInFile(t[numTests], pch);
            pch = strtok(NULL, " \t\n");
            TestSetOutFile(t[numTests], pch);
            pch = strtok(NULL, " \t\n");
            TestSetTimeout(t[numTests], atoi(pch));

            pch = strtok(NULL, " \t\n");
            while (pch) {
               TestAddArg(t[numTests], pch);
               pch = strtok(NULL, " \t\n");
            }

            ProgramPrintContents(p);
            TestPrintContents(t[numTests]);
            numTests++;
         }
         else {
            printf("Bad suite file. Exiting\n");
            exit(1);
         }

         printf("\n");
      }
   }

   // do tests and teardown
   printf("Doing tests... Tests done!\n");
   teardownProgramAndTests(p, t, &numTests);
   fclose(suite);

   return 0;
}

void teardownProgramAndTests(Program *p, Test *tests[], int *numTests) {
   ProgramDelete(p);

   int i;
   for (i = 0; i < *numTests; i++) 
      TestDelete(tests[i]);
   *numTests = 0;
}
