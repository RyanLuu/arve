#ifndef ASMINST_H
#define ASMINST_H

#define MAX_ARGS (3)

typedef struct field_t {
  unsigned int so;
  unsigned int eo;
} field_t;

typedef struct asminst_t {
  field_t inst;
  field_t args[MAX_ARGS];
  int nargs;
} asminst_t;

extern void print_field(char *, field_t);
extern void print_asminst(char *, asminst_t);

#endif /* ASMINST_H */
