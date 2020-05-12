#include "parser.h"

#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "error.h"
#include "itable.h"
#include "main.h"

static uint8_t parse_reg(char *s, size_t);
static int32_t parse_imm(char *s, size_t);
static int strncicmp(const char *, const char *, size_t);

/**
 * Returns 0 if line cannot be parsed. Returns machine instruction
 * otherwise.
 */
uint32_t parse(char *line, asminst_t ai) {
  if (ai.inst.eo <= ai.inst.so) {
    return 0;
  }

  uint32_t instruction = 0;
  instent_t instent;

  for (int i = 0; i <= ninsts; i++) {
    if (i == ninsts) {  // out-of-bounds <=> instruction not found
      print_lineerror("invalid instruction " BOLD("\"%.*s\""),
                      ai.inst.eo - ai.inst.so, &line[ai.inst.so]);
      return 0;
    }
    if (strlen(itable[i].name) == ai.inst.eo - ai.inst.so &&
        !strncicmp(line + ai.inst.so, itable[i].name, strlen(itable[i].name))) {
      instent = itable[i];
      break;
    }
  }
  if (ai.nargs != instent.nargs) {
    print_lineerror("incorrect number of arguments to instruction " BOLD(
                        "\"%s\"") " (expected %d, got %d)",
                    instent.name, instent.nargs, ai.nargs);
    return 0;
  }

  // write machine instruction

  instruction |= instent.opcode;
  switch (instent.type) {
    uint8_t rd, rs1, rs2;
    int32_t imm;
    case R:
      (void)rs1;
      (void)rs2;
      break;
    case I:
      if (!strcmp(instent.name, "ecall")) {
        break;
      } else if (!strcmp(instent.name, "ebreak")) {
        instruction |= 1 << 20;
        break;
      }
      rd = parse_reg(line + ai.args[0].so, ai.args[0].eo - ai.args[0].so);
      if (errno) {
        return 0;
      }
      rs1 = parse_reg(line + ai.args[1].so, ai.args[1].eo - ai.args[1].so);
      if (errno) {
        return 0;
      }
      imm = parse_imm(line + ai.args[2].so, ai.args[2].eo - ai.args[2].so);
      if (errno) {
        return 0;
      }
      instruction |= rd << 7;
      instruction |= instent.func3 << 12;
      instruction |= rs1 << 15;
      instruction |= BITS(imm, 0, 11) << 20;
      break;
    case S:
      rs1 = parse_reg(line + ai.args[0].so, ai.args[0].eo - ai.args[0].so);
      if (errno) {
        return 0;
      }
      rs2 = parse_reg(line + ai.args[1].so, ai.args[1].eo - ai.args[1].so);
      if (errno) {
        return 0;
      }
      imm = parse_imm(line + ai.args[2].so, ai.args[2].eo - ai.args[2].so);
      if (errno) {
        return 0;
      }
      instruction |= rs1 << 15;
      instruction |= rs2 << 20;
      instruction |= BITS(imm, 0, 4) << 7;
      instruction |= BITS(imm, 5, 11) << 25;
      break;
    case B:
      rs1 = parse_reg(line + ai.args[0].so, ai.args[0].eo - ai.args[0].so);
      if (errno) {
        return 0;
      }
      rs2 = parse_reg(line + ai.args[1].so, ai.args[1].eo - ai.args[1].so);
      if (errno) {
        return 0;
      }
      imm = parse_imm(line + ai.args[2].so, ai.args[2].eo - ai.args[2].so);
      if (errno) {
        return 0;
      }
      instruction |= rs1 << 15;
      instruction |= rs2 << 20;
      instruction |= BITS(imm, 1, 4) << 8;
      instruction |= BITS(imm, 11, 11) << 7;
      instruction |= BITS(imm, 12, 12) << 31;
      instruction |= BITS(imm, 5, 10) << 25;
      break;
    case U:
      rd = parse_reg(line + ai.args[0].so, ai.args[0].eo - ai.args[0].so);
      if (errno) {
        return 0;
      }
      imm = parse_imm(line + ai.args[1].so, ai.args[1].eo - ai.args[1].so);
      if (errno) {
        return 0;
      }
      instruction |= rd << 7;
      instruction |= BITS(imm, 12, 31) << 12;
      break;
    case J:
      // TODO: implement J-type
      break;
  }
  return instruction;
}

/**
 * strncmp(), but case-insensitive.
 */
static int strncicmp(const char *a, const char *b, size_t n) {
  for (int i = 0; i < n; i++, a++, b++) {
    int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
    if (d != 0) {
      return d;
    }
  }
  return 0;
}

/**
 * Converts a register name (e.g. x0) into a number. Returns -1 if the name is
 * not valid.
 */
static uint8_t parse_reg(char *s, size_t len) {
  errno = 0;
  char *end;
  long l = -1;
  if (s[0] == 'x') {
    l = strtol(s + 1, &end, 10);
  }
  // TODO: implement ABI names of registers

  if (errno != 0 || l < 0 || l >= 32 || end - s != len) {
    errno = EINVAL;
    print_lineerror("invalid register " BOLD("\"%.*s\""), len, s);
    return 0;
  }
  return (uint8_t)l;
}

static int32_t parse_imm(char *s, size_t len) {
  errno = 0;
  char *end = NULL;
  long l = strtol(s, &end, 0);
  if (errno != 0 || end - s != len) {
    print_lineerror("invalid immediate " BOLD("\"%.*s\""), len, s);
    errno = EINVAL;
    return 0;
  }
  return (int32_t)l;
}
