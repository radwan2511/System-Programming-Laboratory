#include <stdio.h>
#include <stdlib.h>

char *signatures_file_path = "signatures";
size_t prefix_size = 18;

typedef struct virus virus;

struct virus {
    unsigned short length;
    char name[16];
    char signature[];
};

int printHex(char *buffer, int length) {
  int i;
  for (i = 0; i < length; i++) {
    printf("%02x ", 0xff & (int)*buffer++);
  }
  printf("\n");
  return 0;
}

int main(int argc, char *argv[]) {
  struct virus *viruses[10];
  FILE *signatures;
  int file_size;
  unsigned short current_virus_size;
  int i;
  
  signatures = fopen(signatures_file_path, "r");
  if (!signatures) {
    perror("Signatures error");
    return 0;
  }
  
  fseek (signatures, 0L , SEEK_END);
  file_size = ftell (signatures);
  rewind (signatures);
  
  for (i = 0; file_size > 0; i++) {
    /* read current virus's length */
    fread((char *)&current_virus_size + 1, sizeof(char), 1, signatures);
    fread(&current_virus_size, sizeof(char), 1, signatures);
    
    viruses[i] = (struct virus *)malloc(sizeof(char)*current_virus_size);
    
    fread(viruses[i]->name, sizeof(char), 16, signatures);
    
    file_size -= current_virus_size;
    
    current_virus_size = current_virus_size - prefix_size;
    viruses[i]->length = current_virus_size;
    
    fread(viruses[i]->signature, sizeof(char), current_virus_size, signatures);
  }
  for (i = 0; i < 10; i++) {
    printf("%s : %d\n", viruses[i]->name, viruses[i]->length);
    printHex(viruses[i]->signature, viruses[i]->length);
    free(viruses[i]);
  }
  
  fclose(signatures);
  return 0;
}