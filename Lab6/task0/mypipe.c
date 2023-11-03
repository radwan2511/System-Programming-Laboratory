#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define 	WRITE_END	1
#define 	READ_END	0
#define 	BUF_SIZE	10

int main(int argc, char *argv[]) {
  int pipefd[2], pid;
  char buf[BUF_SIZE], *buf_tmp;
  
  if(pipe(pipefd) == -1) {
    perror("can't pipe.");
    return 0;
  }
  pid = fork();
    if (!pid) {
      close(pipefd[READ_END]);
      write(pipefd[WRITE_END], "hello!", strlen("hello!"));
      close(pipefd[WRITE_END]);
      
      _exit(0);
    }
    else {
      buf_tmp = buf;
      close(pipefd[WRITE_END]);
      while (read(pipefd[READ_END], buf_tmp, 1) > 0)
	buf_tmp++;
      *buf_tmp = 0;
      close(pipefd[READ_END]);
      
      printf("child says: %s\n", buf);
    }
  return 0;
}