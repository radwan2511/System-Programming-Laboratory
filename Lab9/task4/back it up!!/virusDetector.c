#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus virus;
 
struct virus {
    unsigned short length;
    char name[16];
    char signature[];
};

typedef struct link link;
 
struct link {
    virus *v;
    link *next;
};

void PrintHex(char* data, int length){
  int i;
  for(i = 0 ; i < length; i++){
    printf("%02X ", 0xff&data[i]);
  }
  printf("\n");
}



void list_print(link *virus_list){
  if(virus_list == NULL)
    return;
  link* temp = virus_list;
  while(temp->next != NULL){
    printf("Virus name: %s\nVirus size: %x \nsignature:\n",temp->v->name,temp->v->length);
    PrintHex(temp->v->signature, temp->v->length);
    printf("\n");
    temp = temp->next;
  }
    printf("%s %d \n",temp->v->name,temp->v->length);
    PrintHex(temp->v->signature, temp->v->length);
}
 
link* list_append(link* virus_list, virus* data){
  if(virus_list == NULL){
    virus_list = (struct link*)malloc(sizeof(link));
    virus_list->v=NULL;
    virus_list->next=NULL;
  }
  if(virus_list->v == NULL){
    virus_list->v = data;
    virus_list->next = NULL;
    return virus_list;
  }
  link* temp = virus_list;
  while(temp->next != NULL)
    temp = temp->next;
  temp->next = (struct link*)malloc(sizeof(link));
  temp->next->v=data;
  temp->next->next=NULL;
  return virus_list;
    
}
 
void list_free(link *virus_list){
    if(virus_list == NULL)
      return;
    link* temp = virus_list;
    while(virus_list->next != NULL){
      temp = virus_list;
      free(temp->v);
      virus_list = virus_list->next;
      free(temp);
  }
  free(virus_list->v);
  free(virus_list);
}


void detect_virus(char *buffer, link *virus_list, unsigned int size){
  int i;
  int j;
  char* temp;
  int list_size = 1;
  link* temp_pointer = virus_list;
  while(temp_pointer->next!=NULL){
    list_size = list_size + 1;
    temp_pointer = temp_pointer->next;
  }
  for(i = 0 ; i < list_size ; i ++){
    temp = buffer;
    for(j = 0 ; j < size; j ++){
      if(memcmp(temp, virus_list->v->signature, virus_list->v->length) == 0){
	printf("Virus Detected!!\nLocation: %x \nName: %s\nSize: %d\n\n",j,virus_list->v->name,virus_list->v->length);
      }
      temp = temp + 1;
    }
   virus_list = virus_list->next;
  }
}


int main(int argc, char **argv) {
    char* fileName = "signatures.txt";
    virus* v;
    FILE* fpRead;
    int first;
    int sizeRead;
    int second;
    link* virus_list;
    int size;
    char toCheck[1024*10];
    FILE* fp = fopen(fileName,"r+");
    if(fp == NULL) 
    {
	perror("Error in opening file");
	return(-1);
    }  
    virus_list = (struct link*)malloc(sizeof(link));
    virus_list->v = NULL;
    virus_list->next = NULL;
    fseek (fp, 0L , SEEK_END);
    size = ftell (fp);
    rewind (fp);
    while(size>0){
      first = fgetc(fp);
      second = fgetc(fp);
      v =  (struct virus*)(malloc((first*16 + second)*sizeof(char)));
      v->length = first*16 + second - 18;
      fread(v->name, sizeof(char), 16, fp);
      fread(v->signature, sizeof(char), v->length, fp);
      size = size - (v->length + 18);
      virus_list = list_append(virus_list, v);
      
    }
    list_print(virus_list);
       
    fclose(fp);
    
    
    
    
    
    if(argc==1){
      perror("No Input File!");
      return(-1);
    }
    fpRead = fopen(argv[1], "r+");
    if(fpRead == NULL) 
    {
	perror("Error in opening file");
	return(-1);
    }  
    printf("\n--------------------\nDetecting viruses in %s!\n\n",argv[1]);
    sizeRead = fread(toCheck, sizeof(char), 1024*10, fpRead);
    detect_virus(toCheck, virus_list, sizeRead) ;
    fclose(fpRead);
    list_free(virus_list);
    return 0;
}