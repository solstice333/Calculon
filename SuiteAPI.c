#include "SuiteAPI.h"

#define DEBUG 0

const int GNUmakefile = 1;
const int makefile = 2;
const int Makefile = 3;

/* Returns 1 if |*filename| exists. Returns 0 otherwise. */
static int fileExists(char *filename) {
   FILE *file;
   if (file = fopen(filename, "r")) {
      fclose(file);
      return 1;
   }
   return 0;
}

/* Returns the GNUMakefile constant if GNUMakefile exists, makefile constant
*  if makefile exists, and the Makefile constant if Makefile exists.
*  Returns 0 otherwise, if no makefile exists. 
*/
static int makefileExists() {
   int rtn = 0;

   if (fileExists("GNUmakefile"))
      rtn = GNUmakefile;
   else if (fileExists("makefile"))
      rtn = makefile;
   else if (fileExists("Makefile"))
      rtn = Makefile;

   return rtn;
}

/* For debugging purposes. Prints out the argument list in |**argsv| 
*  to stderr with a header intro message |*intromsg| above it. The 
*  argument list must end with a NULL.
*/
static void printArgsv(char **argsv, char *intromsg) {
   fprintf(stderr, "%s\n", intromsg);
   while (*argsv) 
      fprintf(stderr, "%s\n", *argsv++);
   fprintf(stderr, "\n");
}

/* This helper function is called when Make returns a value of 2 in which
*  make has failed due to a nonexistent target needed by a dependency. The
*  error message is piped to the parent and the parent calls badMake() to
*  print out |msg[]| to stderr and the failed build line
*/
static void badMake(char msg[]) {
   fprintf(stderr, "%s", msg);

   int idx;
   char *delim = " *,'`\t\n";
   char *pch = strtok(msg, delim);

   for (idx = 0; idx < TARGET; idx++) 
      pch = strtok(NULL, delim);
   fprintf(stderr, "Failed:  %s\n", pch);
}

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

#if DEBUG
   printArgsv(args, "Printing SafeRun args:");
#endif

   return args;
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

   while (*runnerSrc) {
      if (!fileExists(*runnerSrc)) {
         fprintf(stderr, 
          "Could not find required test file '%s'\n", *runnerSrc);
         rmdir(path);
         exit(1);
      }
      *runnerFiles++ = *runnerSrc++;
   }

   while (*runnerHeader) 
      *runnerFiles++ = *runnerHeader++;
   for (idx = 0; idx < numTests; idx++) {
      *runnerFiles++ = tests[idx]->inFile;
      *runnerFiles++ = tests[idx]->outFile;
   }

   int mf = makefileExists();
   if (mf == GNUmakefile)
      *runnerFiles++ = "GNUmakefile";   
   else if (mf == makefile)
      *runnerFiles++ = "makefile";
   else if (mf == Makefile)
      *runnerFiles++ = "Makefile";

   *runnerFiles++ = path;
   *runnerFiles++ = NULL;

#if DEBUG
   printArgsv(files, "Arg list for cp:");
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

int runGccMake(Program *p) {
   int fd[2];
   pipe(fd);   

   pid_t cpid = fork();
   if (cpid < 0)
      fprintf(stderr, "Error: Something forked up\n");
   else if (cpid > 0) {
      int status;
      close(fd[W]);
      if (!makefileExists())
         close(fd[R]);
      wait(&status);

      if (WEXITSTATUS(status) == MAKE_FAIL) {
         char *buf = calloc(BUFSIZE, sizeof(char));
         read(fd[R], buf, BUFSIZE);
         badMake(buf);
         free(buf);
         return 1;
      }
   }
   else {
      if (makefileExists()) {
#if DEBUG
         fprintf(stderr, "Running make!\n");
#endif
         close(fd[R]);   
         dup2(fd[W], 2);
         close(fd[W]);
         execl(MAKE, MAKE, NULL);
      }
      else {
         close(fd[W]);
         close(fd[R]);
#if DEBUG
         fprintf(stderr, "Running gcc!\n");
#endif
         char **args = malloc(DEFAULT_SIZE); 
         char **runner = args;
         char **src = p->src;

         *runner++ = GCC;
         while (*src) 
            *runner++ = *src++;
         *runner = NULL;

#if DEBUG
         printArgsv(args, "Printing gcc args list:");
#endif

         execv(GCC, args);
      }
   }

   return 0;
}
