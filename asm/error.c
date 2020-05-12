#include "error.h"

#include <stdarg.h>
#include <stdio.h>

#include "main.h"

void print_lineerror(char *errorfmt, ...) {
  va_list args;
  fprintf(stderr, "%s:%d: " ERROR_EFFECT("error:") " ", infile, line_number);
  va_start(args, errorfmt);
  vfprintf(stderr, errorfmt, args);
  va_end(args);
  fprintf(stderr, "\n");
}

void print_error(char *errorfmt, ...) {
  va_list args;
  fprintf(stderr, ERROR_EFFECT("error:") " ");
  va_start(args, errorfmt);
  vfprintf(stderr, errorfmt, args);
  va_end(args);
  fprintf(stderr, "\n");
}
