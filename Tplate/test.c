#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
   char *fail = "*** No rule to make target 'main.c', "
    "needed by 'a.out'.  Stop.\n";

   int fd[2];
   pipe(fd);

   pid_t cpid = fork();
   if (cpid < 0)
      fprintf(stderr, "Something forked up\n");
   if (cpid > 0) {
      int status;
      close(fd[1]);
      wait(&status);
   }
   else {
      close(fd[0]);
      dup2(fd[1], 1);
      printf("%s", fail);
      return 2;
   }

   char buf[1024];
   read(fd[0], buf, 1024); 
   fprintf(stderr, "buf: %s", buf);

   char *delim = " *,'\t\n";
   char *pch = strtok(buf, delim);

   int i;
   for (i = 0; i < 8; i++) {
      fprintf(stderr, "pch: %s\n", pch);
      pch = strtok(NULL, delim);
   }

   fprintf(stderr, "this one: %s\n", pch);

   return 0;
}
