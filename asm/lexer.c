#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

/**
 * Advance i until a non-whitespace character is found.
 */
static void ignore_whitespace(size_t *i, char *line) {
  while (isspace((unsigned char)line[*i])) {
    (*i)++;
  }
}

static void get_field(size_t *i, char *line, field_t *field) {
  if (field) {
    field->so = *i;
  }
  while (!isspace((unsigned char)line[*i]) && line[*i] != '\0' &&
         line[*i] != ',') {
    (*i)++;
  }
  if (field) {
    field->eo = *i;
  }
}

static void get_arg(size_t *i, char *line, asminst_t *ai) {
  if (ai->nargs >= MAX_ARGS) {
    get_field(i, line, NULL);
  } else {
    get_field(i, line, &ai->args[ai->nargs]);
  }
  (ai->nargs)++;
}

static size_t remove_comment(char *line, size_t n) {
  for (int i = 0; i < n; i++) {
    if (line[i] == LINE_COMMENT) {
      line[i] = '\0';
      return i;
    }
  }
  return n;
}

int tokenize(char *line, asminst_t *ai, size_t n) {
  n = remove_comment(line, n);

  size_t i = 0;

  ignore_whitespace(&i, line);

  // get instruction

  get_field(&i, line, &(ai->inst));

  ignore_whitespace(&i, line);

  if (line[i] == ',') {
    return 1;
  }
  ai->nargs = 0;
  while (i < n) {
    ignore_whitespace(&i, line);
    get_arg(&i, line, ai);

    ignore_whitespace(&i, line);
    if (line[i] == ',') {
      i++;
    } else if (line[i] == '\0') {
    } else {
      return 1;
    }
  }
  return 0;
}

int isempty(char *line, unsigned int len) {
  for (int i = 0; i < len; i++) {
    if (!isspace((unsigned char)line[i])) {
      return 0;
    }
  }
  return 1;
}
