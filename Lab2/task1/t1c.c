 
int main() {
  int iarr[] = {1, 2, 3};
  char carr[] = {'a', 'b', 'c'};
  int i;
  for (i = 0; i < 3; i++) {
    printf("i%d: %p\n", i, iarr+i);
  }
  for (i = 0; i < 3; i++) {
    printf("c%d: %p\n", i, carr+i);
  }
  return 0;
}