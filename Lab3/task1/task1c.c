#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  
  if (data == NULL) {
    data = (virus *)malloc(sizeof(char)*16+sizeof(unsigned short));
    data->length = 0;
  }
  
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

void detect_virus(char *buffer, link *virus_list, unsigned int size) {
  int i, vlength, cmp_result;
  virus *v;
  char *iterator;
  
  while (virus_list->next) {
    iterator = buffer;
    v = virus_list->v;
    vlength = v->length;
    for (i = 0; i < size - vlength; i++) {
      cmp_result = (size_t)memcmp(iterator, v->signature, vlength);
      if (0 == cmp_result) {
	printf("_____________________________________________________________\n");
	printf("hiding place: 0x%x\nfucker's name: %s\nfucker's size: %d\n", i, v->name, vlength);
      }
      iterator++;
    }
    virus_list = virus_list->next;
  }
}

FILE *open_file(char *path) {
  FILE *stream;
  stream = fopen(path, "r");
  if (!stream) {
    perror(path);
    return NULL;
  }
  return stream;
}

link *read_signatures(char *file_path) {
  link *list; virus *v;
  FILE *stream;
  unsigned short current_virus_size;
  int i, file_size;
  
  stream = open_file(file_path);
  if (!stream) return NULL;
  fseek (stream, 0L , SEEK_END);
  file_size = ftell (stream);
  rewind (stream);
  
  list = list_append(NULL, NULL);
  
  
  for (i = 0; file_size > 0; i++) {
    /* read current virus's length */
    fread((char *)&current_virus_size + 1, sizeof(char), 1, stream);
    fread(&current_virus_size, sizeof(char), 1, stream);
    /* allocate memory for the virus */
    v = (virus *)malloc(sizeof(char)*current_virus_size);
    
    file_size -= current_virus_size;
    
    /* read name and signature and set all values */
    fread(v->name, sizeof(char), 16, stream);
    current_virus_size = current_virus_size - prefix_size;
    v->length = current_virus_size;
    fread(v->signature, sizeof(char), current_virus_size, stream);
    
    /* append the list */
    list = list_append(list, v);
  }
  
  fclose(stream);
  return list;
}

int main(int argc, char *argv[]) {
  link *list= NULL;
  FILE *stream; size_t filesize;
  char *buffer = (char *)calloc(sizeof(char), 10*1024);
  
  stream = open_file(argv[1]);
  fseek (stream, 0L , SEEK_END);
  filesize = ftell (stream);
  rewind (stream);
  
  filesize = ((filesize > 10*1024) ? filesize : 10*1024);
  
  fread(buffer, sizeof(char), filesize, stream);
  fclose(stream);
  
  list = read_signatures(signatures_file_path);
  if (!list) {
    perror("Error reading the signatures file");
    return 0;
  }
  
  detect_virus(buffer, list, filesize);
  list_free(list);
  free(buffer);
  return 0;
}