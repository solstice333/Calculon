// infinite loop, sometimes runtime error, sometimes runtime error and timeout
#include <stdio.h>
#include "header.h"

int main() {
   while (getchar() != EOF);
   while(1);
   return 0;
}
