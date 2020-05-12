#ifndef LABEL_H
#define LABEL_H

#define MAX_LABELS (4096)

#include <stdint.h>

typedef struct label_t {
  char *name;
  uint32_t pc;
} label_t;

label_t labeltable[MAX_LABELS];
uint8_t nlabel = 0;

extern void add_label(char *, uint32_t);
extern void print_label(label_t);
extern void print_labels();

#endif /* LABEL_H */
