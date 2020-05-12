#ifndef INST_H
#define INST_H

#include <stdint.h>

#define IALIGN (32)

/* Integer Register-Register */

#define OP (0b0110011)

#define ADD_SUB (0b000)
#define SLL (0b001)
#define SLT (0b010)
#define SLTU (0b011)
#define XOR (0b100)
#define SRL_SRA (0b101)
#define OR (0b110)
#define AND (0b111)
#define ADD_SLT_SLTU_F7 (0b0000000)
#define AND_OR_XOR_F7 (0b0000000)
#define SLL_SRL_F7 (0b0000000)
#define SUB_SRA_F7 (0b0100000)

typedef struct {
  unsigned int func7 : 7;
  unsigned int rs2 : 5;
  unsigned int rs1 : 5;
  unsigned int func3 : 3;
  unsigned int rd : 5;
  unsigned int opcode : 7;
} R;

/* Integer Register-Immediate */

#define OP_IMM (0b0010011)

#define ADDI (0b000)
#define SLTI (0b010)
#define SLTIU (0b011)
#define XORI (0b100)
#define ORI (0b110)
#define ANDI (0b111)
#define SLLI (0b001)
#define SRLI_SRAI (0b101)
#define SLLI_F7 (0b0000000)  // technically not the funct7 field, but similar
#define SRLI_F7 (0b0000000)
#define SRAI_F7 (0b0100000)

/* Load */

#define LOAD (0b0000011)

#define LB (0b000)
#define LH (0b001)
#define LW (0b010)
#define LBU (0b100)
#define LHU (0b101)

#define JALR (0b1100111)

#define MISC_MEM (0b0001111)

#define FENCE (0b000)
#define FENCE_NORMAL (0b0000)
#define FENCE_TSO (0b1000)

#define SYSTEM (0b1110011)

#define PRIV (0b000)
#define ECALL_F12 (0b000000000000)
#define EBREAK_F12 (0b000000000001)

typedef struct {
  signed int imm : 12;
  unsigned int rs1 : 5;
  unsigned int func3 : 3;
  unsigned int rd : 5;
  unsigned int opcode : 7;
} I;

/* Store */

#define STORE (0b0100011)

#define SB (0b000)
#define SH (0b001)
#define SW (0b010)

typedef struct {
  unsigned int imm : 12;
  unsigned int rs2 : 5;
  unsigned int rs1 : 5;
  unsigned int func3 : 3;
  unsigned int opcode : 7;
} S;

#define BRANCH (0b1100011)

#define BEQ (0b000)
#define BNE (0b001)
#define BLT (0b100)
#define BGE (0b101)
#define BLTU (0b110)
#define BGEU (0b111)

typedef struct {
  int imm : 12;
  unsigned int rs2 : 5;
  unsigned int rs1 : 5;
  unsigned int func3 : 3;
  unsigned int opcode : 7;
} B;

#define LUI (0b0110111)
#define AUIPC (0b0010111)

typedef struct {
  unsigned int imm : 20;
  unsigned int rd : 5;
  unsigned int opcode : 7;
} U;

#define JAL (0b1101111)

typedef struct {
  int imm : 20;
  unsigned int rd : 5;
  unsigned int opcode : 7;
} J;

typedef union dinst32_t {
  R r;
  I i;
  S s;
  B b;
  U u;
  J j;
} dinst32_t;

typedef uint32_t inst32_t;

extern void inst_exec(const uint32_t);
extern void inst_debug(const uint32_t);

#endif /* INST_H */