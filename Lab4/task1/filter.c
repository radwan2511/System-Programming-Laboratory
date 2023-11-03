
#include "util.h"

#define SYS_OPEN 	5
#define SYS_CLOSE 	6
#define SYS_READ	3
#define SYS_WRITE	4
#define SYS_LSEEK	19

#define STDIN 		0
#define STDOUT 		1
#define STDERR 		2

#define O_RDONLY	0
#define O_RDRW		2
#define O_CREAT		64

#define SEEK_END	2
#define SEEK_CUR	1
#define SEEK_SET	0

#define NEW_LINE 	'\n'

int system_call(int num, ...);

/* debug flag */
int debug = 0;
int log_file;

void mylog(char *text, int length) {
  if (debug) {
    system_call(SYS_WRITE,log_file, text, length);
  }
}
void mylogs(char *text) {
  mylog(text, strlen(text));
}

void error(char *text) {
  mylogs("error: ");
  mylogs(text);
  system_call(1, 0x55, 0, 0); /* exit with status 0x55 */
}

int open_file(char *path, int flags, int mode) {
  int file = system_call(SYS_OPEN, path, flags, mode);
  if (debug) {
    mylogs("called system_call 5 (SYS_OPEN) with arguments: ");
    mylogs(path);
    mylogs(", ");
    mylogs(itoa(flags));
    mylogs(", ");
    mylogs(itoa(mode));
    mylogs("\n");
    mylogs("system_call 5 (SYS_OPEN) returned \n");
    mylogs(itoa(file));
    mylogs("\n");
  }
  if (file == -1) error("opening directory\n");
  return file;
}
int read_byte(int fd, char *c) {
  return system_call(SYS_READ, fd, c, sizeof(char));
}
int print(char *text, int length, int stream) {
  int retval = system_call(SYS_WRITE, stream, text, length); /*
  if (debug) {
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
int prints(char *text, int stream) {
  return print(text, strlen(text), stream);
}
void filter_print(char c, int stream) {
  switch(c) {
    case 'h': case 'H': case NEW_LINE:
    print(&c, sizeof(char), stream);
  }
}

int main(int argc, char *argv[]) {
  int out_stream = STDOUT;
  int input_stream = STDIN;
  int i;
  char c;
  
  log_file = open_file("log", O_RDRW | O_CREAT, 0777);
  
  /* scan arguments */
  for (i = 0; i < argc; i++) {
    if (0 == strcmp("-d", argv[i])) {
      debug = 1;
      mylogs("found -d \n");
    } else if (0 == strcmp("-i", argv[i])) {
      input_stream = open_file(argv[++i], O_RDONLY, 0);
      mylogs("found -i ");
      mylogs(argv[i]);
      mylogs("\n");
    } else if (0 == strcmp("-o", argv[i])) {
      out_stream = open_file(argv[++i], O_RDRW | O_CREAT, 0777);
      mylogs("found -o ");
      mylogs(argv[i]);
      mylogs("\n");
    }
  }
  
  i = read_byte(input_stream, &c);
  while (i != 0 && i != -1) {
    filter_print(c, out_stream);
    i = read_byte(input_stream, &c);
  }
  return 0;
}