#ifndef PARSER_H
#define PARSER_H

#define MAXLEN_INST (8)

#define BITS(x, low, high) (x >> low) % (0b10 << (high - low))

#include <stdint.h>
#include <stdlib.h>

#include "asminst.h"

extern uint32_t parse(char *, asminst_t);

#endif /* PARSER_H */
