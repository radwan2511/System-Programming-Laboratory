 
#include<stdio.h>

int main(int argc, char *argv[]) {
  int i;
  for (i = 1; i < argc; i++) {
    printf("%s", argv[i]);
  }
  printf("\n");
  
  /* stuff */
  printf("%d\n", (int)argv);
  
  int a = 17, b = 12, c = -1;
  
  printf("Adrresses: (2)\n");
  printf("a:%d\n", &b - 1);
  printf("b:%d\n", &b);
  printf("c:%d\n", &b + 1);
  
  printf("Values: \n");
  printf("a:%d\n", *(&b - 1));
  printf("b:%d\n", *&b);
  printf("c:%d\n", *(&b + 1));
  
  return 0;
}