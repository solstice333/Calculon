#include "SuiteAPI.h"

#define DEBUG 1

Program *ProgramCreate() {
   Program *p = malloc(sizeof(Program));
   p->name[0] = '\0';
   p->src[p->idxSrc = 0] = NULL;
   p->header[p->idxHeader = 0] = NULL;
   return p;
}

void ProgramSetName(Program *p, char *name) {
   strcpy(p->name, name);
}

void ProgramAddSrc(Program *p, char *src) {
   p->src[p->idxSrc] = malloc(strlen(src) + 1);
   strcpy(p->src[p->idxSrc], src); 
   p->src[++p->idxSrc] = NULL;
}

void ProgramAddHeader(Program *p, char *header) {
   p->header[p->idxHeader] = malloc(strlen(header) + 1);
   strcpy(p->header[p->idxHeader], header);
   p->header[++p->idxHeader] = NULL;
}

void ProgramPrintContents(Program *p) {
   printf("name: %s\n", p->name);

   printf("src: \n");
   char **runner = p->src;
   while (*runner) 
      printf(" %s\n", *runner++);

   printf("headers: \n");
   runner = p->header;
   while (*runner) 
      printf(" %s\n", *runner++);
}

void ProgramDelete(Program *p) {
   char **del = p->src;

   while(*del)
      free(*del++);

   del = p->header;
   while (*del)
      free(*del++);

   free(p);
}

Test *TestCreate() {
   Test *t = malloc(sizeof(Test));
   t->inFile[0] = t->outFile[0] = '\0';
   t->args[t->idx = t->timeout = 0] = NULL;
   return t;
}

void TestSetInFile(Test *t, char *in) {
   strcpy(t->inFile, in);
}

void TestSetOutFile(Test *t, char *out) {
   strcpy(t->outFile, out);
}

void TestSetTimeout(Test *t, int ms) {
   t->timeout = ms;
}

void TestAddArg(Test *t, char *arg) {
   t->args[t->idx] = malloc(strlen(arg) + 1);
   strcpy(t->args[t->idx++], arg);
   t->args[t->idx] = NULL;
}

void TestPrintContents(Test *t) {
   printf("inFile: %s\n", t->inFile);
   printf("outFile: %s\n", t->outFile);
   printf("timeout: %d\n", t->timeout);
   printf("args:\n");

   char **runner = t->args;
   while (*runner) 
      printf(" %s\n", *runner++);
}

void TestDelete(Test *t) {
   char **del = t->args;
   while (*del) 
      free(*del++);
   free(t);
}

char **buildSrArgs(Program *p, Test *tests[], int testNum) {
   char **args = malloc(DEFAULT_SIZE * sizeof(char *));
   char **runner = args;
   char **testArgP = tests[testNum]->args;

   *runner++ = SAFERUN;
   *runner++ = P30;

   *runner = malloc(DEFAULT_SIZE);
   sprintf(*runner++, "-t%d", tests[testNum]->timeout);

   *runner = malloc(DEFAULT_SIZE);
   sprintf(*runner++, "-T%d", MULT * tests[testNum]->timeout);

   *runner = malloc(DEFAULT_SIZE);
   sprintf(*runner++, "./%s", p->name);

   while (*testArgP) {
      *runner = malloc(DEFAULT_SIZE);
      strcpy(*runner++, *testArgP++); 
   }

   *runner = NULL;
   return args;
}

void printSrArgs(char **args) {
   char **runner = args;
   while (*runner) 
      printf("%s ", *runner++);
   printf("\n");
}

char *mkDirMvTests(Program *p, Test *tests[], int numTests) {
   char *path = malloc(DEFAULT_SIZE/2);
   sprintf(path, ".%d", getpid());
   mkdir(path, 0744);

   int idx;
   char *files[DEFAULT_SIZE];
   char **runnerFiles = files;
   char **runnerSrc = p->src; 
   char **runnerHeader = p->header;

   *runnerFiles++ = CP;
   while (*runnerSrc) 
      *runnerFiles++ = *runnerSrc++;
   while (*runnerHeader) 
      *runnerFiles++ = *runnerHeader++;
   for (idx = 0; idx < numTests; idx++) {
      *runnerFiles++ = tests[idx]->inFile;
      *runnerFiles++ = tests[idx]->outFile;
   }
   *runnerFiles++ = path;
   *runnerFiles++ = NULL;

#if DEBUG
   fprintf(stderr, "arg list for cp: \n"); 
   runnerFiles = files;
   while (*runnerFiles) 
      fprintf(stderr, "%s\n", *runnerFiles++);
#endif
   
   pid_t cpid = fork();
   if (cpid < 0)
      fprintf(stderr, "Error: Something forked up\n");
   else if (cpid > 0) 
      wait(NULL);
   else 
      execv(CP, files);

   return path;
}

void rmDirRmTests(char *path) {
   chdir(path);
   DIR *dir = opendir(".");
   dirent *direntry;

   while (direntry = readdir(dir)) 
      remove(direntry->d_name);
   closedir(dir);

   chdir("..");
   rmdir(path);
   free(path);
}

/* Returns 1 if makefile exists. Otherwise 0 */
static int makefileExists() {
   FILE *mf;
   int rtn = 0;

   if (mf = fopen("GNUMakefile", "r")) {
      fclose(mf);
      rtn = 1;
   }
   else if (mf = fopen("makefile", "r")) {
      fclose(mf);
      rtn = 1;
   }
   else if (mf = fopen("Makefile", "r")) {
      fclose(mf);
      rtn = 1;
   }

   return rtn;
}

// TODO come back here and implement this
void runGccMake(Program *p) {
   pid_t cpid = fork();
   if (cpid < 0)
      fprintf(stderr, "Error: Something forked up\n");
   else if (cpid > 0)
      wait(NULL);
   else {
      if (makefileExists()) 
         execl(MAKE, MAKE, NULL);
          



      char **args = malloc(DEFAULT_SIZE); 
      char **runner = args;
      char **src = p->src;

      *runner++ = GCC;
      while (*src) 
         *runner++ = *src++;
      *runner = NULL;

#if DEBUG
      fprintf(stderr, "\nPrinting gcc args list: \n");
      runner = args;
      while (*runner) 
         fprintf(stderr, "%s\n", *runner++);
      fprintf(stderr, "\n");
#endif

      execv(GCC, args);


   }
}
