int main() {
  int iarray[] = {1,2,3};
  char carray[] = {'a','b','c'};
  int* iarrayPtr = iarray;
  char* carrayPtr = carray; 
  int *p;
  
  int i;
  for (i = 0; i < 3; i++)
    printf("%d\n", *(iarrayPtr+i));
  for (i = 0; i < 3; i++)
    printf("%c\n", *(carrayPtr+i));
  printf("%p\n", p);
  return 0;
}