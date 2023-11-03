#include<stdio.h>
#include<errno.h>

int PrintHex(char *buffer, int length) {
  int i;
  for (i = 0; i < length; i++) {
    printf("%02x ", 0xff & (int)*buffer++);
  }
  printf("\n");
  return 0;
}
int main(int argc, char *argv[]) {
  char *filepath;
  char buffer[30];
  FILE *stream;
  size_t filesize;
  
  filepath = *(argv+1);
  stream = fopen(filepath, "r");
  
  if (!stream) {
    perror("Error");
    return 0;
  }
  
  fseek (stream, 0L , SEEK_END);
  filesize = ftell (stream);
  rewind (stream);
  
  filesize = fread(buffer, sizeof(char), filesize, stream);
  
  PrintHex(buffer, filesize);
  
  return 0;
}
