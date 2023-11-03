#include <stdio.h>
#include <stdlib.h>

int plus_one(int n) {
  return n+1;
}

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

int quit(int n) {
  exit(n);
}

int* map(int *array, int arrayLength, int (*f) (int)){
  int* mappedArray = (int*)(malloc(arrayLength*sizeof(int)));
  int i;
  for(i = 0; i < arrayLength; i++) {
    mappedArray[i] = f(array[i]);
  }
  return mappedArray;
}

struct fun_desc {
  char *name;
  int (*fun)(int);
};

int main(int argc, char **argv){
  char *input; char tmp_buffer[10]; /* for reading input */
  int *tmp_array;
  int * iarray = malloc(sizeof(int)*5);
  int item_count, user_input;
  
  struct fun_desc *iterator;
  struct fun_desc funs[7] = {
    {"plus_one", plus_one},
    {"abs", abs},
    {"iprt", iprt},
    {"cprt", cprt},
    {"my_get", my_get},
    {"quit", quit},
    {NULL, NULL} 
  };
  while (1) {
    
    /* print menu */
    iterator = funs;
    item_count = 0;
    printf("Please choose a function: \n");
    while(iterator->fun) {
      printf("%d)%s\n", item_count++, iterator->name);
      iterator++;
    }
    printf("Option: ");
    
    /* get input */
    input = fgets(tmp_buffer, 10, stdin);
    if (feof(stdin)) {
      printf("\n");
      quit(0);
    }
    user_input = atoi(input);
    
    /* handle option */
    if (user_input < 0 || user_input >= item_count)
      printf("not within bounds\n");
    
    else {
      printf("within bounds\n");
      tmp_array = map(iarray, 5, funs[user_input].fun);
      free(iarray);
      iarray = tmp_array;
    }
    printf("DONE.\n");
  }
  return 0;
} 