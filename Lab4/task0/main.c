#include "util.h"

#define SYS_OPEN 	5
#define SYS_CLOSE 	6
#define SYS_READ	3
#define SYS_WRITE	4
#define SYS_LSEEK	19

#define STDOUT 		1

#define O_RDRW		2
#define SEEK_END	2
#define SEEK_CUR	1
#define SEEK_SET	0
/*
int main (int argc , char* argv[], char* envp[])
{
  int i;
  char * str;
  str = itoa(argc);
  system_call(SYS_WRITE,STDOUT, str,strlen(str));
  system_call(SYS_WRITE,STDOUT," arguments \n", 12);
  for (i = 0 ; i < argc ; i++)
    {
      system_call(SYS_WRITE,STDOUT,"argv[", 5);
	  str = itoa(i);
      system_call(SYS_WRITE,STDOUT,str,strlen(str));
      system_call(SYS_WRITE,STDOUT,"] = ",4);
      system_call(SYS_WRITE,STDOUT,argv[i],strlen(argv[i]));
      system_call(SYS_WRITE,STDOUT,"\n",1);
    }
  return 0;
}
*/
int main (int argc , char* argv[], char* envp[])
{
  int file;
  int filesize, pos = 0;
  char buff[5]; /* shira */
  char *shira = "Shira\n";
  
  file = system_call(SYS_OPEN, argv[1], O_RDRW);
  filesize = system_call(SYS_LSEEK, file, 0, SEEK_END);
  system_call(SYS_WRITE,STDOUT,&filesize, 4);
  
  while (pos + 5 < filesize) {
      system_call(SYS_LSEEK, file, pos, SEEK_SET);
      system_call(SYS_READ, file, buff, 5);
      system_call(SYS_LSEEK, file, pos, SEEK_SET);
      if (pos == 0x251) {
	system_call(SYS_WRITE,STDOUT,"here\n", 5);
	system_call(SYS_WRITE,STDOUT,buff, 5);
      }
      if (0 == strncmp(shira, buff, 5)) {
	system_call(SYS_WRITE,STDOUT,"and there\n", 10);
	system_call(SYS_WRITE, file, argv[2], 5);
	return 0;
      }
      pos++;
  }
  return 0;
}