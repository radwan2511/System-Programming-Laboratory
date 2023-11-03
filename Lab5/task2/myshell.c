#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <linux/limits.h>
#include "LineParser.h"

#define BUFFER_SIZE	2048
#define HISTORY_SIZE	100
#define NEW_EXEC		-1
#define BAD_EXEC_INDEX	-2

struct variable {
  char *name;
  char *val;
  struct variable *next;
};
typedef struct variable var;

char *history[HISTORY_SIZE];
int history_index = 0;
var *first_var = 0, *last_var = 0, *cached;
char *home_directory;

void free_env() {
  var *tmp;
  while(first_var) {
    tmp = first_var->next;
    free(first_var->name);
    free(first_var->val);
    free(first_var);
    first_var = tmp;
  }
}

void free_history() {
  for (history_index--; history_index > -1; history_index--) {
    free(history[history_index]);
  }
}

void add_to_history(const char *text) {
  if (history_index >= HISTORY_SIZE) {
      fprintf(stderr, "history exceeded the limit\n");
    } else {
      history[history_index] = malloc(strlen(text)*sizeof(char)+1);
      strncpy(history[history_index], text, strlen(text));
      history[history_index][strlen(text)*sizeof(char)] = 0; /* put null termination */
      history_index++;
    }
}

int get_history(char *cmd) {
  int i, size = strlen(cmd)-1; /* dont compare the newline character */
  for (i = history_index-1; i >= 0; i--) {
    if (0 == strncmp(cmd, history[i], size)) {
      return i;
    }
  }
  return -1;
}

void env() {
  var *v = first_var;
  while (v) {
    printf("%s = %s\n", v->name, v->val);
    v=v->next;
  }
}

var *get_node_by_name(char *name) {
  var *v = first_var; 
  cached = 0;
  while (v && 0 != strcmp(v->name, name)) {
    cached = v;
    v = v->next;
  }
  return v;
}

var *make_new_node(char *name, char *value) {
    var *v = malloc(2*sizeof(char *) + sizeof(struct variable *));
    v->name = malloc(strlen(name)*sizeof(char)+1);
    v->val = malloc(strlen(value)*sizeof(char)+1);
    strncpy(v->name, name, strlen(name));
    strncpy(v->val, value, strlen(value));
    v->name[strlen(name)*sizeof(char)] = 0; /* add null termination */
    v->val[strlen(value)*sizeof(char)] = 0;
    v->next = 0;
    return v;
}

void set(char *name, char *value) {
  var *v;
  
  if (!first_var) {
    last_var = first_var = make_new_node(name, value);
    return;
  }
  
  v = get_node_by_name(name);
   
  if(v) {
    free(v->val);
    v->val = malloc(strlen(value)*sizeof(char)+1);
    strncpy(v->val, value, strlen(value));
    v->val[strlen(value)*sizeof(char)] = 0; /* put null termination */
  }
  else {
    last_var->next = make_new_node(name, value);
    last_var = last_var->next;
    return;
  }
}

void execute(cmdLine *line) {
  int pid, i, status;
  var *v;
  /* return if line==null */
  if (!line) return;
  
  /* handle variables */
  for (i = 0; i < line->argCount; i++) {
    if ('$' == line->arguments[i][0]) {
      if ((v = get_node_by_name(line->arguments[i]+1)))
	replaceCmdArg(line, i, v->val);
      else {
	fprintf(stderr, "can't load value: variable doesn't exist\n");
	return;
      }
    }
  }
  
  /* handle history, cd, set, env */
  if (0 == strncmp("cd", line->arguments[0], 2)) {
    for (i = 1; i < line->argCount; i++) {
      if (0 == strcmp(line->arguments[i], "~"))
	replaceCmdArg(line, i, home_directory);
    }
    if (-1 == chdir(line->arguments[1])) {
      fprintf(stderr, "error executing cd command\n");
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
  else if (0 == strncmp("set", line->arguments[0], 3)) {
    set(line->arguments[1], line->arguments[2]);
    execute(line->next);
    return;
  } 
  else if (0 == strncmp("delete", line->arguments[0], 6)) {
    v = get_node_by_name(line->arguments[1]);
    if (v) {
      if (cached) cached->next = v->next; /* v->next = cached->next->next */
      else first_var = first_var->next;	/* cached=null --> v = first_var  */
      
      free(v->name);
      free(v->val);
      free(v);
    } 
    else {
      fprintf(stderr, "can't delete: variable doesn't exist\n");
    }
    execute(line->next);
    return;
  } 
  else if (0 == strncmp("env", line->arguments[0], 3)) {
    env();
    execute(line->next);
    return;
  }
  
  pid = fork();
    if (!pid) {
      execvp(line->arguments[0], line->arguments);
      fprintf(stderr, "error executing command\n");
      _exit(0);
    }
    else {
      if (line->blocking)
	waitpid(pid, &status, 0);
      execute(line->next);
    }
}

char *get_input(char *buf, int size, FILE *fd) {
  return fgets(buf, size, fd);
}

int main(int argc, char *argv[]) {
  char in_buf[BUFFER_SIZE], path_buf[PATH_MAX];
  int _running = 1, exec_index = NEW_EXEC;
  cmdLine *currentLine;
  
  /* set home_directory variable */
  home_directory = getenv("HOME");
  
  while (_running) {
    if(!getcwd(path_buf, PATH_MAX)) {
      fprintf(stderr, "error getting current path\n");
      free_history();
      exit(0x55);
    } 
    printf("%s> ", path_buf);
    get_input(in_buf, BUFFER_SIZE, stdin);
    if (feof(stdin)) {
      printf("\n");
      free_history();
      exit(0);
    }
    /* handle execute from history */
    if ('!' == in_buf[0]) {
	exec_index = get_history(in_buf+1);
	if (exec_index > -1 && exec_index < history_index)
	  add_to_history(history[exec_index]);
	else {
	  exec_index = BAD_EXEC_INDEX;
	  fprintf(stderr, "command doesn't exist in history\n");
	}
    } else add_to_history(in_buf);
    
    /* handle quit or execute */
    if (0 == strncmp(in_buf, "quit", 4)) {
	printf("bye bye\n");
	_running = 0;
    } 
    else if (exec_index != BAD_EXEC_INDEX) {
      if (NEW_EXEC == exec_index) currentLine = parseCmdLines(in_buf);
      else currentLine = parseCmdLines(history[exec_index]);
      execute(currentLine);
      freeCmdLines(currentLine);
    }
    exec_index = NEW_EXEC;
  }
  free_env();
  free_history();
  return 0;
}
