#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <unistd.h>
#include "elf.h"

#define IN_BUF_SIZE		20
#define FILENAME_MAX_LENGTH	100
#define EQUAL			0

struct fun_desc {
  char *name;
  void (*fun)();
};
char *map_base_ptr = NULL;
int Currentfd = -1, file_size, dmode = 0;

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

void close_file() {
  if (map_base_ptr != NULL) {
    munmap(map_base_ptr, file_size);
    map_base_ptr = NULL;
  }
  if (Currentfd != -1) {
    close(Currentfd);
    Currentfd = -1;
  }
}

void open_new_file(char *filename) {
  struct stat fd_stat;
  int fd; 
  char *map_addr;
  
  mylog2("filename: %s", (int)filename, 0, 0);
  if ((fd = open(filename, O_RDWR)) == -1) {
    perror("can't open file");
    return;
  }
  
  if (fstat(fd, &fd_stat) != 0 ) {
      perror("stat failed");
      close(fd);
      return;
   }
   
  if ((map_addr = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
    perror("mmap failed");
    close(fd);
    return;
  }
  
  if (memcmp(map_addr, ELFMAG, 4*sizeof(char)) != EQUAL) {
    fprintf(stderr, "error: not an elf file\n");
    munmap(map_addr, fd_stat.st_size);
    close(fd);
    return;
  }
  
  /* success:	
    	close the last file 	*/
  close_file();
  /* 	change state 		*/
  Currentfd = fd;
  map_base_ptr = map_addr;
  file_size = fd_stat.st_size;
}

void quit() {
  close_file();
  mylog("quitting");
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

int check_fd() {
  if (map_base_ptr == NULL || Currentfd == -1) {
    fprintf(stderr, "no file loaded\n");
    return 0;
  }
  return 1;
}

char *get_name_from_str_table(char *str_table, int index) {
  if (index == 0)
    return "";
  else return str_table + index;
}

void print_symbols() {
  Elf32_Sym *symbol_table = NULL;
  Elf32_Ehdr *header = (Elf32_Ehdr *)map_base_ptr;
  Elf32_Shdr *sections = (Elf32_Shdr *)(map_base_ptr + header->e_shoff);
  char *header_str_table = map_base_ptr + sections[header->e_shstrndx].sh_offset; /* first header + index of string-table header*/
  char *symbol_str_table;
  char *current_section_name, *current_symbol_name;
  int current_section_index;
  int num_of_entries = header->e_shnum;
  int num_of_symbols;
  int i;
  
  for (i = 0; i < num_of_entries; i++) {
    if (sections[i].sh_type == SHT_SYMTAB) {
      
      symbol_table = (Elf32_Sym *)(map_base_ptr + sections[i].sh_offset);
      num_of_symbols = sections[i].sh_size / sections[i].sh_entsize;
      
      mylog("found SHT_SYMTAB!");
      mylog2("symbol count: %d; entry size: %d; total size: %d", num_of_symbols, sections[i].sh_entsize, sections[i].sh_size);
      
      /* check if the symbol table is empty */
      if (sections[i].sh_size <= 0) {
	printf("symbol table empty!\n");
	return;
      }
      
      /* aquire appropriate string table */
      symbol_str_table = map_base_ptr + sections[sections[i].sh_link].sh_offset;
      printf("hi five: %p : %s\n", symbol_str_table, symbol_str_table + 20);
      break;
    }
  }
  
  if (symbol_table == NULL) {
    fprintf(stderr, "error: couldn't find symbol table\n");
    return;
  }
  
  printf("[Num] \tvalue \tNdx \tsection_name \tsymbol_name\n");
  for (i = 0; i < num_of_symbols; i++) {
    
    current_section_index = symbol_table->st_shndx;
    
    if (current_section_index == SHN_ABS) {
      current_section_name = "ABS";
    } else current_section_name = get_name_from_str_table(header_str_table, sections[current_section_index].sh_name);
    
    current_symbol_name = get_name_from_str_table(symbol_str_table, symbol_table->st_name);
    
    /* [index] value section_index section_name symbol_name */
    printf("[%02d] \t%08x  \t%d  \t%s  \t%s\n", i, symbol_table->st_value, current_section_index, current_section_name, current_symbol_name);
    
    symbol_table++;
  }
}

void print_section_names() {
  Elf32_Ehdr *header = (Elf32_Ehdr *)map_base_ptr;
  int num_of_entries = header->e_shnum;
  Elf32_Shdr *current_header = (Elf32_Shdr *)(map_base_ptr + header->e_shoff); /* now the first header */
  
  char *str_table = map_base_ptr + ((Elf32_Shdr *)(current_header + header->e_shstrndx))->sh_offset; /* first header + index of string-table header*/
  char *current_name;
  int i;
  
  if (!check_fd()) return;
  
  for (i = 0; i < num_of_entries; i++) {
    
    current_name = get_name_from_str_table(str_table, current_header->sh_name);
    
    /* [index] section_name section_address section_offset section_size  section_type */
    printf("[%02d] \tname: %s \t\taddr:%p \toffset: %x \tsize: %x \ttype: %x\n", i, 
	   current_name, (void *)current_header->sh_addr, current_header->sh_offset, current_header->sh_size, current_header->sh_type);
    current_header++;
  }
}

void examine_elf_file() {
  Elf32_Ehdr *header;
  char *encoding[] = {"Invalid data encoding", "2's complement, little endian", "2's complement, big endian", "ELFDATANUM"};
  char in_buf[IN_BUF_SIZE];
  
  printf("please enter file name\n");
  get_input(in_buf, IN_BUF_SIZE);
  in_buf[strlen(in_buf)-1] = '\0'; /* remove new-line character */
  open_new_file(in_buf);
  
  header = (Elf32_Ehdr *)map_base_ptr;
  
  /* 
   * Print Data:
   * 
   * Bytes 1,2,3 of the magic number (in ASCII). Henceforth, you should check that the number is consistent with an ELF file, and refuse to continue if it is not.
   * The data encoding scheme of the object file.
   * Entry point (hexadecimal address).
   * The file offset in which the section header table resides.
   * The number of section header entries.
   * The size of each section header entry.
   * The file offset in which the program header table resides.
   * The number of program header entries.
   * The size of each program header entry.
   * 
   */
  
  printf("magic number's 3 bytes (ascii) : \t%c%c%c\n", header->e_ident[EI_MAG1], header->e_ident[EI_MAG2], header->e_ident[EI_MAG3]);
  printf("data encoding type: \t\t\t%s\n", encoding[header->e_ident[EI_DATA]]);
  printf("entry point (virtual address): \t\t%p\n", (void *)header->e_entry);
  printf("section header table file offset: \t%x\n", (unsigned int)header->e_shoff);
  printf("section header table num of sections: \t%d\n", (int)header->e_shnum);
  printf("program header table file offset: \t%x\n", (unsigned int)header->e_phoff);
  printf("program header table num of sections: \t%d\n", (int)header->e_phnum);
  printf("program header table entry size: \t%d\n", (int)header->e_phentsize);
  printf("\n");
}

void toggle_debug_mode() {
  mylog("Debug flag now off");
  dmode = 1 - dmode;
  mylog("debug flag now on");
}

int print_menu(struct fun_desc *funs) {
  struct fun_desc *iterator;
  int item_count;
  
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
    {"Examine ELF File", examine_elf_file},
    {"Print Section Names", print_section_names},
    {"Print Symbols", print_symbols},
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