#include "main.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "asminst.h"
#include "error.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char **argv) {
  // parse arguments

  int option;
  infile = NULL;
  outfile = NULL;
  while (optind < argc) {
    if ((option = getopt(argc, argv, "o:")) != -1) {
      switch (option) {
        case 'o':
          outfile = optarg;
          break;
        case '?':
          exit(EXIT_FAILURE);
        default:
          abort();
      }
    } else {
      if (infile == NULL) {
        infile = argv[optind];
      } else {
        exit(EXIT_FAILURE);
      }
      optind++;
    }
  }

  if (infile == NULL) {
    print_error("No input file specified.");
    exit(EXIT_FAILURE);
  }
  if (outfile == NULL) {
    outfile = "a.out";
  }

  FILE *fp = fopen(infile, "r");
  if (fp == NULL) {
    print_error("unable to open %s", infile);
    exit(1);
  }

  char *line = NULL;
  line_number = 0;
  size_t len;
  uint32_t instruction;

  asminst_t ai;
  while ((len = getline(&line, &len, fp)) != -1) {
    line_number++;
    if (isempty(line, len)) {
      continue;
    }
    if (tokenize(line, &ai, len) == 0) {
      if ((instruction = parse(line, ai)) == 0) {
        break;
      }
      print_asminst(line, ai);
      printf(" -> ");
      for (int j = 0; j < 32; j++) {
        printf("%d", instruction >> 31);
        instruction <<= 1;
      }
      printf("\n");

    } else {
      printf("Error on line %d.\n", line_number);
    }
  }

  fclose(fp);
  free(line);

  return 0;
}
