#include <stdio.h>

/* task2_a */

int plus_one(int n) {
  return n+1;
}
 
int* map(int *array, int arrayLength, int (*f) (int)){
  int* mappedArray = (int*)(malloc(arrayLength*sizeof(int)));
  int i;
  for(i = 0; i < arrayLength; i++) {
    mappedArray[i] = f(array[i]);
  }
  return mappedArray;
}
 
/* task2_b */

int abs(int n) {
  if (n < 0)
    return -n;
  return n;
}

int iprt(int n) {
  printf("%#x\n", n);
  return n;
}

int cprt(int n) {
  if (n < 0x20 || n > 0x7E)
    printf(".\n");
  else
    printf("%c\n", n);
  return n;
}

int my_get(int n) {
  char temp_buffer[30];
  char * res = gets(temp_buffer);
  if (feof(stdin))
    exit(-1);
  return atoi(res);
}
 
/* task2_c */
int quit(int n) {
  exit(n);
}
 
int main(int argc, char **argv){
  /***********************************************t2_a**/
  int array[] = {3, 5, 72, 2, 4, 6};
  int *narray = map(array, 6, *plus_one);
  int i;
  for(i = 0; i < 6; i++) {
    printf("%d ", narray[i]);
  }
  free(narray);
  printf("\n");
  
  /***********************************************t2_b**/
  int *barray = map(array, 3, *cprt);
  
  iprt(78);
  iprt(abs(2));
  iprt(abs(-3));
  cprt(98);
  cprt(0x21);
  cprt(0x7F);
  iprt(my_get(15));
  
  free(barray);
  
  /***********************************************t2_c**/
  printf("quitting...\n");
  quit(0);
  printf("its not working :(\n");
  return 0;
} 