#include "label.h"

#include <stdio.h>

void add_label(char *name, uint32_t pc) {
  labeltable[nlabel].name = name;
  labeltable[nlabel].pc = pc;
  nlabel++;
}

void print_label(label_t label) { printf("%s (%X)", label.name, label.pc); }

void print_labels() {
  for (int i = 0; i < nlabel; i++) {
    printf("%s (%X)\n", labeltable[i].name, labeltable[i].pc);
  }
}
