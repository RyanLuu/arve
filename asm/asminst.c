#include "asminst.h"

#include <stdio.h>

void print_field(char *line, field_t field) {
  printf("%.*s", field.eo - field.so, line + field.so);
}

void print_asminst(char *line, asminst_t ai) {
  if (ai.inst.eo > ai.inst.so) {
    print_field(line, ai.inst);
    printf("(");
    for (int i = 0; i < ai.nargs; i++) {
      if (i < MAX_ARGS) {
        print_field(line, ai.args[i]);
      } else {
        printf("?");
      }
      if (i < ai.nargs - 1) {
        printf(",");
      }
    }
    printf(")");
  } else {
    printf("(null)");
  }
}
