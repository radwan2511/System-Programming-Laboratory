#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define 	WRITE_END	1
#define 	READ_END	0
#define 	BUF_SIZE	10

int main(int argc, char *argv[]) {
  int pipefd1[2], pipefd2[2], pid1, pid2, pid3;
  char *args1[] = {"ls", "-l", 0};
  char *args2[] = {"tail", "-n", "2", 0};
  char *args3[] = {"wc", "-l", 0};
  
  /* creating pipes... */
  if(pipe(pipefd1) == -1) {
    perror("can't pipe1.");
    return 0;
  }
  if(pipe(pipefd2) == -1) {
    perror("can't pipe2.");
    return 0;
  }
  
  /* code from here... */
  pid1 = fork();
    if (!pid1) {
      fclose(stdout);
      dup(pipefd1[WRITE_END]);
      close(pipefd1[WRITE_END]);
      execvp(args1[0], args1);
      _exit(0);
    }
    else {
      close(pipefd1[WRITE_END]);
      pid2 = fork();
      if (!pid2) {
	/* in */
	fclose(stdin);
	dup(pipefd1[READ_END]);
	close(pipefd1[READ_END]);
	/* out */
	fclose(stdout);
	dup(pipefd2[WRITE_END]);
	close(pipefd2[WRITE_END]);
	
	execvp(args2[0], args2);
	_exit(0);
      }
      else {
	close(pipefd1[READ_END]);
	close(pipefd2[WRITE_END]);
	
	pid3 = fork();
	if (!pid3) {
	  fclose(stdin);
	  dup(pipefd2[READ_END]);
	  close(pipefd2[READ_END]);
	  execvp(args3[0], args3);
	  _exit(0);
	}
	else {
	  close(pipefd2[READ_END]);
	  /* parent */
	  waitpid(pid1, 0, 0);
	  waitpid(pid2, 0, 0);
	  waitpid(pid3, 0, 0);
	}
      }
    }
  return 0;
}