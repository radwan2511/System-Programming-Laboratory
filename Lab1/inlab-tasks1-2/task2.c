		  
#include<stdio.h>
#define NEW_LINE '\n'
		  
int toFilter(char c, char filter) {
  if (c == filter)
    return 1;
  return 0;
}
		  
int main(int argc, char *args[]) {
  char c; int i; int input_file_path_index;
  char * currentString; char * filterChars = NULL; 
  char buffer_in[30]; char buffer_out[30]; char * user_input;
  
  FILE * stream; FILE * out_stream = stdout;
  // flags
  int input = 0; int output = 0;
  
  for (i = 1; i < argc; i++) {
    if (0 == strcmp("-i", args[i])) {
      input = 1;
      input_file_path_index = i + 1;
    }
    else if (0 == strcmp("-o", args[i]))
      output = 1;
  } i = 0;
  if (argc > 1 && !input && 0 != strcmp(args[1], "-o"))
    filterChars = args[1];
  
  if (input) {
    stream = fopen(args[input_file_path_index], "r");
      if (!stream) {
	fputs("error opening the file.\n", stdout);
	return 0; //error, return.
      } else {
	filterChars = fgets(buffer_in, 30, stream);
	fclose(stream);
      }
  }
  if (output) {
    fputs("Enter file path:\n", stdout);
    out_stream = fopen(gets(buffer_out), "w");
    if (!out_stream) {
      fputs("Error opening the file.\n", stdout);
      return 0;
    }
  }
  if (filterChars) { // filter characters were given as an argument, or an input file was specified
    c = fgetc(stdin);
    while (!feof(stdin)) {
      currentString = filterChars;
      if (toFilter(c, NEW_LINE)) {
	fputc(c, out_stream);
      }
      else while (*currentString) {
	if (toFilter(c, *currentString)) {
	  fputc(c, out_stream);
	  break;
	}
	currentString++;
      }
      c = fgetc(stdin);
    }
  }
  else { // filter 'h' and 'H' only; default case.
    c = fgetc(stdin);
    while (!feof(stdin)) {
      switch (c) {
	case 'h': case 'H': case NEW_LINE: fputc(c, out_stream); break;
	default: break;
      }
      c = fgetc(stdin);
    }
  }
  if (output)
    fclose(out_stream);
  return 0;
}