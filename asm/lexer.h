#ifndef LEXER_H
#define LEXER_H

#define LINE_COMMENT '#'

#include <stdlib.h>

#include "asminst.h"

extern int tokenize(char *, asminst_t *, size_t);
extern int isempty(char *, unsigned int);

#endif /* LEXER_H */
