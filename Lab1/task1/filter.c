		  
#include<stdio.h>
#define 	CAPITAL_NONCAPITAL_DIFFERENCE 	32
#define 	CAPITALS_START 			65
#define 	CAPITAL_END 			90
#define 	NONCAPITALS_START 		97
#define 	NONCAPITAL_END 			122
		  
int toFilter(char c, char filter) {
		    if (c == filter)
		      return 1;
		    else if (filter >= CAPITALS_START && filter <= CAPITAL_END) {
		      if (c == filter + CAPITAL_NONCAPITAL_DIFFERENCE)
			return 1;
		    }
		    else if (filter >= NONCAPITALS_START && filter <= NONCAPITAL_END) {
		      if (c == filter - CAPITAL_NONCAPITAL_DIFFERENCE)
			return 1;
		    }
		    return 0;
		  }
		  
int main(int argc, char *args[]) {
  char c; int i; int input_file_path_index; int to_filter;
  char * currentString; char * filterChars = NULL;
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
      }
      else { 
	fputs("here.\n", stdout);
	filterChars = fgets("", 30, stream);
	fputs("and there.\n", stdout);
      }
  }
  if (output) {
    fputs("Enter file path:\n", stdout);
    out_stream = fopen(gets(""), "r+");
  }
  if (filterChars) {
    while ((c = fgetc(stdin)) != EOF) {
      to_filter = 0;
      currentString = filterChars;
      while (*currentString) {
	if (toFilter(c, *currentString))
	  to_filter = 1;
	currentString++;
      }
      if (!(to_filter))
	fputc(c, out_stream);
      to_filter = 0;
    }
  }
  else {
    while ((c = fgetc(stdin)) != EOF) {
      switch (c) {
	case 'h': case 'H': case EOF: break;
	default: fputc(c, out_stream); break;
      }
    }
  }
  if (output)
    fclose(out_stream);
  return 0;
}
  
  /*
  
  switch (argc) {
    case 1: while (!feof(stdin)) {
      switch (c = fgetc(stdin)) {
	case 'h': case 'H': case EOF: break;
	default: fputc(c, stdout); break;
      }
    } break;
    case 3: if (0 == strcmp("-i", args[1])) {
      stream = fopen(args[2], "r");
      if (!stream) {
	fputs("error opening the file.\n", stdout);
	return 0; //error, return.
      }
      else { 
	fileContent = fgets(fileContent, 30, stream);
	while ((c = fgetc(stdin)) != EOF) {
	  currentString = fileContent;
	  while (*currentString) {
	    if (toFilter(c, *currentString))
	      to_filter = 1;
	    currentString++;
	  }
	  if (!to_filter)
	    fputc(c, stdout);
	  to_filter = 0;
	}
      }
    } //first argument is not -i, so now were rolling down to the default case... ***not breaking***
    default: while ((c = fgetc(stdin)) != EOF) {
      to_filter = 0;
      i = 1;
      while ((currentString = args[i]) != NULL) {
	while (*currentString) {
	  if (toFilter(c, *currentString))
	    to_filter = 1;
	  currentString++;
	}
	i++;
      }
      if (!(to_filter))
	fputc(c, stdout);
      to_filter = 0;
    } break;
  }
  return 0;
}
*/