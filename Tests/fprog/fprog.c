// missing source file, hard exit - intended memory leaks
#include <stdio.h>
#include "SmartAlloc.h"

int main() {
   while(getchar() != EOF);
   printf("Hello. From bprog!\n");
   return 0;
}
