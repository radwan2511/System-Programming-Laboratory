#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define 	WRITE_END	1
#define 	READ_END	0
#define 	BUF_SIZE	10

int main(int argc, char *argv[]) {
  int pipefd[2], pid1, pid2;
  char *args1[] = {"ls", "-l", 0};
  char *args2[] = {"head", "-c", "8", 0};
  
  if(pipe(pipefd) == -1) {
    perror("can't pipe.");
    return 0;
  }
  pid1 = fork();
    if (!pid1) {
      fclose(stdout);
      dup(pipefd[WRITE_END]);
      close(pipefd[WRITE_END]);
      execvp(args1[0], args1);
      _exit(0);
    }
    else {
      close(pipefd[WRITE_END]);
      pid2 = fork();
      if (!pid2) {
	fclose(stdin);
	dup(pipefd[READ_END]);
	close(pipefd[READ_END]);
	execvp(args2[0], args2);
	_exit(0);
      }
      else {
	close(pipefd[READ_END]);
	/* parent */
	waitpid(pid1, 0, 0);
	waitpid(pid2, 0, 0);
      }
    }
  return 0;
}