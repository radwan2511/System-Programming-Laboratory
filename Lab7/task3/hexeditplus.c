#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define IN_BUF_SIZE		20
#define BUFFER_SIZE		4*1024
#define FILENAME_MAX_LENGTH	100

struct fun_desc {
  char *name;
  void (*fun)();
};
int usize = 1, dmode = 0;
char buffer[BUFFER_SIZE];
char *filename = 0;

void mylog(char *msg) {
  if (dmode) {
    fprintf(stderr, "< ");
    fprintf(stderr, "%s", msg);
    fprintf(stderr, " >\n");
  }
}
void mylog2(char *msg, int a, int b, int c) {
  if (dmode) {
    fprintf(stderr, "< ");
    fprintf(stderr, msg, a, b, c);
    fprintf(stderr, " >\n");
  }
}

void quit() {
  mylog("quitting");
  if (filename) {
    free(filename);
  }
  exit(0);
}

char *get_input(char *buf, int buf_size) {
  char *input;
  
  printf(">> ");
  
  input = fgets(buf, buf_size, stdin);
  if (feof(stdin)) {
    printf("\n");
    quit();
  }
  if (!input) {
    perror("error reading input via gets");
    quit();
  }
  return input;
}

void toggle_debug_mode() {
  mylog("Debug flag now off");
  dmode = 1 - dmode;
  mylog("debug flag now on");
}

void set_file_name() {
  if (filename) free(filename);
  filename = malloc(sizeof(char)*FILENAME_MAX_LENGTH);
  
  printf("please enter file name\n");
  get_input(filename, FILENAME_MAX_LENGTH);
  filename[strlen(filename)-1] = 0; /* replace new-line with null termination */
  mylog2("Debug: filename set to \"%s\"", (int)filename, 0, 0);
}

void set_unit_size() {
  char buf[10];
  char *input;
  int chosen_size;
  
  printf("enter unit-size (1, 2 or 4 bytes)\n");
  input = get_input(buf, 10); 
  chosen_size = atoi(input);
  if (chosen_size == 1 || chosen_size == 2 || chosen_size == 4) {
    /* OK */
    mylog2("Debug: set unit-size to %d", chosen_size, 0, 0);
    usize = chosen_size;
  } else {
    /* NOT OK */
    printf("incompatible size: %d\n", chosen_size);
  }
}

void mem_display() {
  char *adr;
  int length, i, j;
  char in_buf[IN_BUF_SIZE];
  
  printf("Please enter <address> <length>\n");
  get_input(in_buf, IN_BUF_SIZE);
  sscanf(in_buf, "%x %d", (unsigned int *)&adr, &length);
  if (adr == 0)
    adr = buffer;
  
  mylog2("Default value: %p", (int)buffer, 0, 0);
  
  for (i = 0; i < length*usize; i += usize) {
    for (j = 0; j < usize; j++) {
      printf("%02x", (unsigned int)adr[i+j] & 0xff);
    }
    printf(" ");
  }
  printf("\n");
}

void load_file_to_memory() {
  char *adr;
  int location, length;
  FILE *fd;
  char in_buf[IN_BUF_SIZE];
  
  if (filename == NULL) {
    mylog("filename is null!");
    return;
  }
  if ((fd = fopen(filename, "r")) == NULL) {
    perror("can't open file");
    return;
  }
  
  printf("Please enter <mem-address> <location> <length>\n");
  get_input(in_buf, IN_BUF_SIZE);
  sscanf(in_buf, "%x %x %d", (unsigned int *)&adr, (unsigned int *)&location, &length);
  
  if (adr == 0)
    adr = buffer;
  
  mylog2("filename: %s", (int)filename, 0, 0);
  mylog2("mem-address: %p; location: %d; length: %d;", (int)adr, location, length); 
  
  fseek(fd, location, SEEK_SET);
  fread(buffer, sizeof(char)*usize, (length*usize > BUFFER_SIZE) ? BUFFER_SIZE : length, fd);
  fclose(fd);
  
  printf("Loaded %d units into %p\n", length, adr);
}

void save_buffer_into_file() {
  char *source;
  unsigned int target_location;
  int length;
  FILE *fd;
  char in_buf[IN_BUF_SIZE];
  
  if (filename == NULL) {
    mylog("filename is null!");
    return;
  }
  if ((fd = fopen(filename, "r+")) == NULL) {
    perror("can't open file");
    return;
  }
  
  printf("Please enter <s-address> <t-location> <length>\n");
  get_input(in_buf, IN_BUF_SIZE);
  sscanf(in_buf, "%x %x %d", (unsigned int *)&source, &target_location, &length);
  
  if (source == 0)
    source = buffer;
  
  mylog2("filename: %s", (int)filename, 0, 0);
  mylog2("mem-address: %p; location: %d; length: %d;", (int)source, target_location, length); 
  
  fseek(fd, 0, SEEK_END);
  if (ftell(fd) < target_location) {
    mylog2("<t-location>(=%d) is greater than the size of %s!", target_location, (int)filename, 0);
    return;
  }
  
  fseek(fd, target_location, SEEK_SET);
  fwrite(source, usize, length, fd);
  fclose(fd);
  
  printf("Saved %d units into %s\n", length, filename);
}

void modify_memory() {
  char *adr; 
  unsigned int val = 0, tmp_val; /* tmp_val holds only one byte of the desired input*/
  char in_buf[IN_BUF_SIZE];
  char *buf_iter = in_buf;
  int i;
  
  printf("Please enter <address> <value>\n");
  get_input(in_buf, IN_BUF_SIZE);
  sscanf(in_buf, "%x %n", (unsigned int *)&adr, &i);
  
  if (adr == 0)
    adr = buffer;
  
  buf_iter = in_buf + i;
  
  for (i = 0; i < usize; i++) {
    sscanf(buf_iter, "%02x", (unsigned int *)&tmp_val);
    val += tmp_val << 8*i; /*  */
    buf_iter += 2;
  }
  
  mylog2("address: %x; value: %x;\n", (int)adr, (int)val, 0);
  
  memcpy(adr, (char *)&val, usize);
}

int print_menu(struct fun_desc *funs) {
  struct fun_desc *iterator;
  int item_count;
  
  /* log */
  mylog2("unit-size: %d", usize, 0, 0);
  mylog2("filename: %s", (int)filename, 0, 0);
  mylog2("buffer address: %p", (int)buffer, 0, 0);
  
  /* print menu */
  iterator = funs;
  item_count = 0;
  printf("Please choose a function: \n");
  while(iterator->fun) {
    printf("%d) %s\n", item_count++, iterator->name);
    iterator++;
  }
  return item_count;
}

int check_bounds(int index, int min, int max) {
  /* mylog2("checking bounds for %d < %d < %d", min-1, index, max+1); */
  if (index < min || index > max) {
      return 0;
  }
  else return 1;
}

int main(int argc, char **argv){
  char *input; 
  char in_buf[IN_BUF_SIZE]; /* for reading input */
  int item_count, index;
  
  struct fun_desc funs[9] = {
    {"Toggle Debug Mode", toggle_debug_mode},
    {"Set File Name", set_file_name},
    {"Set Unit Size", set_unit_size},
    {"Mem Display", mem_display},
    {"Load Into Memory", load_file_to_memory},
    {"Save Into File", save_buffer_into_file},
    {"Mem Modify", modify_memory},
    {"Quit", quit},
    {NULL, NULL} 
  };
  
  while (1) {
    item_count = print_menu(funs);
    
    input = get_input(in_buf, IN_BUF_SIZE);
    /* convert input to int */
    index = atoi(input);
    
    /* handle option */
    if (check_bounds(index, 0, item_count - 1))
      funs[index].fun();
  }
  return 0;
} 