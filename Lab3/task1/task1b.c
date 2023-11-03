#include <stdio.h>
#include <stdlib.h>

char *signatures_file_path = "signatures";
size_t prefix_size = 18;

typedef struct virus virus;
typedef struct link link;

struct virus {
    unsigned short length;
    char name[16];
    char signature[];
};

struct link {
    virus *v;
    link *next;
};

int printHex(char *buffer, int length) {
  int i;
  for (i = 0; i < length; i++) {
    printf("%02x ", 0xff & (int)*buffer++);
  }
  printf("\n");
  return 0;
}

void list_print(link *virus_list) {
  while(virus_list->next) {
    printf("Virus name: %s\nVirus length: %d\n", virus_list->v->name, virus_list->v->length);
    printHex(virus_list->v->signature, virus_list->v->length);
    virus_list = virus_list->next;
  }
}

link* list_append(link* virus_list, virus* data) {
  link* new_link = (link *)malloc(sizeof(link));
  new_link->next = virus_list;
  new_link->v = data;
  return new_link;
}
 
void list_free(link *virus_list) {
  link *tmp;
  while(virus_list) {
    tmp = virus_list->next;
    free(virus_list->v);
    free(virus_list);
    virus_list = tmp;
  }
}

int main(int argc, char *argv[]) {
  link *list; virus *v;
  FILE *signatures;
  unsigned short current_virus_size;
  int i, file_size;
  
  signatures = fopen(signatures_file_path, "r");
  if (!signatures) {
    perror("Signatures error");
    return 0;
  }
  fseek (signatures, 0L , SEEK_END);
  file_size = ftell (signatures);
  rewind (signatures);
  
  list = list_append(NULL, NULL);
  
  for (i = 0; file_size > 0; i++) {
    /* read current virus's length */
    fread((char *)&current_virus_size + 1, sizeof(char), 1, signatures);
    fread(&current_virus_size, sizeof(char), 1, signatures);
    /* allocate memory for the virus */
    v = (virus *)malloc(sizeof(char)*current_virus_size);
    
    file_size -= current_virus_size;
    
    /* read name and signature and set all values */
    fread(v->name, sizeof(char), 16, signatures);
    current_virus_size = current_virus_size - prefix_size;
    v->length = current_virus_size;
    fread(v->signature, sizeof(char), current_virus_size, signatures);
    
    /* append the list */
    list = list_append(list, v);
  }
  fclose(signatures);
  list_print(list);
  list_free(list);
  return 0;
}