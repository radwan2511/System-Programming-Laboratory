#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include "LineParser.h"

#define BUFFER_SIZE	2048
#define HISTORY_SIZE	10
#define NO_EXEC		-1
#define BAD_EXEC_INDEX	-2

char *history[HISTORY_SIZE];
int history_index = 0;

void free_history() {
  for (history_index--; history_index > -1; history_index--) {
    free(history[history_index]);
  }
}

void add_to_history(const char *text) {
  if (history_index >= HISTORY_SIZE) {
      /* TODO: set errno to "out of bounds" */
      perror("history exceeded the limit");
    } else {
      history[history_index] = malloc(strlen(text)*sizeof(char));
      strcpy(history[history_index], text);
      history_index++;
    }
}

void execute(cmdLine *line) {
  int pid, i;
  if (!line) return;
  /* handle $history and $cd */
  if (0 == strncmp("cd", line->arguments[0], 2)) {
    if (-1 == chdir(line->arguments[1])) {
      perror("error executing cd command");
    } else execute(line->next);
    return;
  } 
  else if (0 == strncmp("history", line->arguments[0], 7)) {
      for (i = 0; i < history_index; i++) {
	printf("#%d\t%s\n", i, history[i]);
      }
      execute(line->next);
      return;
    }
  
  pid = fork();
    if (!pid) {
      execvp(line->arguments[0], line->arguments);
      perror("error executing command");
      exit(0);
    } 
    else {
      if (line->blocking)
	waitpid(-1, 0, 0);
      execute(line->next);
    }
}

int main(int argc, char *argv[]) {
  char in_buf[BUFFER_SIZE], path_buf[PATH_MAX];
  int _running = 1, exec_index = NO_EXEC;
  cmdLine *currentLine;
  
  while (_running) {
    if(!getcwd(path_buf, PATH_MAX)) {
      perror("error getting current path");
      free_history();
      exit(0x55);
    } 
    printf("%s> ", path_buf);
    fgets(in_buf, BUFFER_SIZE, stdin);
    if (feof(stdin)) {
      printf("\n");
      free_history();
      exit(0);
    }
    /* handle execute from history */
    if ('!' == in_buf[0]) {
	exec_index = atoi(in_buf+1);
	if (exec_index > -1 && exec_index < history_index)
	  add_to_history(history[exec_index]);
	else {
	  exec_index = BAD_EXEC_INDEX;
	  perror("can't execute");
	}
    } else add_to_history(in_buf);
    
    /* handle quit or execute */
    if (0 == strncmp(in_buf, "quit", 4)) {
	printf("bye bye\n");
	_running = 0;
    } 
    else if (exec_index != BAD_EXEC_INDEX) {
      if (NO_EXEC == exec_index) currentLine = parseCmdLines(in_buf);
      else currentLine = parseCmdLines(history[exec_index]);
      execute(currentLine);
      freeCmdLines(currentLine);
    }
    exec_index = NO_EXEC;
  }
  free_history();
  return 0;
}
