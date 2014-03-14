#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SuiteAPI.h"

#define WORD_SIZE 128
#define LINE_SIZE 512
#define MAX_TESTS 128

int main(int argc, char **argv) {
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
   int numTests = 0;

   while (fgets(line, LINE_SIZE, suite)) {
      char *pch = strtok(line, " \t\n");

      if (pch) {
         if (*pch == 'P') {   // Create Program object
            // TODO If p != NULL then test the first Program object
            // Against its Test objects
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
            printf("\n");
            numTests++;
         }

         printf("\n");
      }
   }

   ProgramDelete(p);
   int i;
   for (i = 0; i < numTests; i++) 
      TestDelete(t[i]);
   fclose(suite);

   return 0;
}
