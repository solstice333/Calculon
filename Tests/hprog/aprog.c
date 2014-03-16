// all 3 tests pass (gcc) (one source file)
#include <stdio.h>
#include "header.h"

int main() {
   while (getchar() != EOF);
   printf("Hello. From aprog!\n");
   return 0;
}
