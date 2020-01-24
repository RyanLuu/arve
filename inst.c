#include "inst.h"

#include <stdint.h>
#include <stdio.h>

#include "reg.h"

static int pop_field(inst32_t *i, const uint8_t n) {
  int ret = ~(~0 << n) & *i;
  *i >>= n;
  return ret;
}

static void r_exec(const R r) {
  if (r.rd == 0) {
    return;
  }
  switch (r.func3) {
    case ADD_SUB:
      if (r.func7 == ADD_SLT_SLTU_F7) {
        set_reg(r.rd, get_reg(r.rs1) + get_reg(r.rs2));
      } else if (r.func7 == SUB_SRA_F7) {
        set_reg(r.rd, get_reg(r.rs1) - get_reg(r.rs2));
      } else {
        printf("[ERROR] Unknown R-type instruction: 0x%02x 0x%01x", r.func7,
               r.func3);
      }
      break;
    case SLL:
      if (r.func7 == SLL_SRL_F7) {
        set_reg(r.rd, get_reg(r.rs1) << (get_reg(r.rs2) & 0b11111));
      }
      break;
    case SLT:
      if (r.func7 == ADD_SLT_SLTU_F7) {
        if (get_reg(r.rs1) < get_reg(r.rs2)) {
          set_reg(r.rd, 1);
        } else {
          set_reg(r.rd, 0);
        }
      }
      break;
    case SLTU:
      if (r.func7 == ADD_SLT_SLTU_F7) {
        if ((uint32_t)get_reg(r.rs1) < (uint32_t)get_reg(r.rs2)) {
          set_reg(r.rd, 1);
        } else {
          set_reg(r.rd, 0);
        }
      }
      break;
    case XOR:
      if (r.func7 == AND_OR_XOR_F7) {
        set_reg(r.rd, get_reg(r.rs1) ^ get_reg(r.rs2));
      }
      break;
    case SRL_SRA:
      if (r.func7 == SLL_SRL_F7) {
        set_reg(r.rd, (inst32_t)get_reg(r.rs1) >> (get_reg(r.rs2) & 0b11111));
      } else if (r.func7 == SUB_SRA_F7) {
        if (get_reg(r.rs1) & (1 << 31)) {  // negative
          set_reg(r.rd, ~(~get_reg(r.rs1) >> get_reg(r.rs2)));
        } else {  // positive
          set_reg(r.rd, get_reg(r.rs1) >> (get_reg(r.rs2) & 0b11111));
        }
      }
      break;
    case OR:
      if (r.func7 == AND_OR_XOR_F7) {
        set_reg(r.rd, get_reg(r.rs1) | get_reg(r.rs2));
      }
      break;
    case AND:
      if (r.func7 == AND_OR_XOR_F7) {
        set_reg(r.rd, get_reg(r.rs1) & get_reg(r.rs2));
      }
      break;
  }
  incr_pc();
}

static void i_exec(const I i) {
  switch (i.opcode) {
    case FENCE:
      switch (i.imm & 0b111100000000) {
        case FENCE_NORMAL:
          break;
        case FENCE_TSO:
          break;
        default:
          break;
      }
      incr_pc();
      break;
    case LOAD:
      printf("LOAD instruction unimplemented");
      incr_pc();
      break;
    case SYSTEM:
      switch (i.func3 == PRIV) {
        case PRIV:
          switch (i.imm) {
            case ECALL_F12:
              break;
            case EBREAK_F12:
              break;
            default:
              break;
          }
          break;
        default:
          break;
      }
      incr_pc();
      break;
    case JALR:
      pc = (i.imm + get_reg(i.rs1)) & (~1);
      if (pc % (IALIGN / 8) != 0) {
        // TODO: report instruction-address-misaligned exception
      }
      set_reg(i.rd, pc + 4);
      break;
      break;
    case OP_IMM:
      switch (i.func3) {
        case ADDI:
          set_reg(i.rd, get_reg(i.rs1) + i.imm);
          break;
        case SLTI:
          if (get_reg(i.rs1) < i.imm) {
            set_reg(i.rd, 1);
          } else {
            set_reg(i.rd, 0);
          }
          break;
        case SLTIU:
          if ((inst32_t)get_reg(i.rs1) < (unsigned int)i.imm) {
            set_reg(i.rd, 1);
          } else {
            set_reg(i.rd, 0);
          }
          break;
        case ANDI:
          set_reg(i.rd, get_reg(i.rs1) & i.imm);
          break;
        case ORI:
          set_reg(i.rd, get_reg(i.rs1) | i.imm);
          break;
        case XORI:
          set_reg(i.rd, get_reg(i.rs1) ^ i.imm);
          break;
        case SLLI:
          if (i.imm >> 5 == SLLI_F7) {
            set_reg(i.rd, get_reg(i.rs1) << i.imm & 0b11111);
          }
          break;
        case SRLI_SRAI:
          if (i.imm >> 5 == SRLI_F7) {
            set_reg(i.rd, get_reg(i.rs1) >> (i.imm & 0b11111));
          } else if (i.imm >> 5 == SRAI_F7) {
            if (get_reg(i.rs1) & (1 << 31)) {  // negative
              set_reg(i.rd, ~(~get_reg(i.rs1) >> (i.imm & 0b11111)));
            } else {  // positive
              set_reg(i.rd, get_reg(i.rs1) >> (i.imm & 0b11111));
            }
          }
          break;
        default:
          printf("[ERROR]\t\tUnknown I-type instruction: 0x%01x\n", i.func3);
          break;
      }
      incr_pc();
      break;
    default:
      printf(
          "[ERROR]\t\tUnable to execute I-type instruction with opcode "
          "0x%02x\n",
          i.opcode);
      incr_pc();
      break;
  }
}

static void s_exec(const S s) {
  printf("[ERROR]\t\tS-type instructions unimplemented.");
  incr_pc();
}

static void b_exec(const B b) {
  switch (b.opcode) {
    case BEQ:
      if (get_reg(b.rs1) == get_reg(b.rs2)) {
        pc += b.imm;
      } else {
        incr_pc();
      }
      break;
    case BNE:
      if (get_reg(b.rs1) == get_reg(b.rs2)) {
        pc += b.imm;
      } else {
        incr_pc();
      }
      break;
    case BLT:
      if (get_reg(b.rs1) < get_reg(b.rs2)) {
        pc += b.imm;
      } else {
        incr_pc();
      }
      break;
    case BLTU:
      if ((uint32_t)get_reg(b.rs1) < (uint32_t)get_reg(b.rs2)) {
        pc += b.imm;
      } else {
        incr_pc();
      }
      break;
    case BGE:
      if (get_reg(b.rs1) >= get_reg(b.rs2)) {
        pc += b.imm;
      } else {
        incr_pc();
      }
      break;
    case BGEU:
      if ((uint32_t)get_reg(b.rs1) < (uint32_t)get_reg(b.rs2)) {
        pc += b.imm;
      } else {
        incr_pc();
      }
      break;
    default:
      incr_pc();
      break;
  }
}

static void u_exec(const U u) {
  switch (u.opcode) {
    case LUI:
      set_reg(u.rd, u.imm << 12);
      break;
    case AUIPC:
      set_reg(u.rd, pc + (u.imm << 12));
      break;
    default:
      break;
  }
  incr_pc();
}

static void j_exec(const J j) {
  switch (j.opcode) {
    case JAL:
      pc += j.imm;
      if (pc % (IALIGN / 8) != 0) {
        // TODO: report instruction-address-misaligned exception
      }
      set_reg(j.rd, pc + 4);
      break;
    default:
      incr_pc();
      break;
  }
}

static void r_decode(R *r, const inst32_t mli) {
  inst32_t temp = mli;
  r->opcode = pop_field(&temp, 7);
  r->rd = pop_field(&temp, 5);
  r->func3 = pop_field(&temp, 3);
  r->rs1 = pop_field(&temp, 5);
  r->rs2 = pop_field(&temp, 5);
  r->func7 = pop_field(&temp, 7);
}

static void i_decode(I *i, const inst32_t mli) {
  inst32_t temp = mli;
  i->opcode = pop_field(&temp, 7);
  i->rd = pop_field(&temp, 5);
  i->func3 = pop_field(&temp, 3);
  i->rs1 = pop_field(&temp, 5);
  i->imm = pop_field(&temp, 12);
}

static void s_decode(S *s, const inst32_t mli) {
  inst32_t temp = mli;
  s->opcode = pop_field(&temp, 7);
  s->imm = pop_field(&temp, 5);
  s->func3 = pop_field(&temp, 3);
  s->rs1 = pop_field(&temp, 5);
  s->rs2 = pop_field(&temp, 5);
  s->imm |= pop_field(&temp, 7) << 5;
}

static void b_decode(B *b, const inst32_t mli) {
  inst32_t temp = mli;
  b->opcode = pop_field(&temp, 7);
  b->imm = pop_field(&temp, 1) << 11;
  b->imm |= pop_field(&temp, 4) << 1;
  b->func3 = pop_field(&temp, 3);
  b->rs1 = pop_field(&temp, 5);
  b->rs2 = pop_field(&temp, 5);
  b->imm |= pop_field(&temp, 5) << 5;
  b->imm |= pop_field(&temp, 1) << 12;
}

static void u_decode(U *u, const inst32_t mli) {
  inst32_t temp = mli;
  u->opcode = pop_field(&temp, 7);
  u->rd = pop_field(&temp, 5);
  u->imm = pop_field(&temp, 20);
}

static void j_decode(J *j, const inst32_t mli) {
  inst32_t temp = mli;
  j->opcode = pop_field(&temp, 7);
  j->rd = pop_field(&temp, 5);
  j->imm = pop_field(&temp, 8) << 12;   // imm[19:12]
  j->imm |= pop_field(&temp, 1) << 11;  // imm[11]
  j->imm |= pop_field(&temp, 10) << 1;  // imm[10:1]
  j->imm |= pop_field(&temp, 1) << 20;  // imm[20]
}

void inst_exec(const inst32_t mli) {
  dinst32_t u = {{0}};
  uint8_t opcode = mli & 0b1111111;
  switch (opcode) {
    case OP:
      r_decode(&u.r, mli);
      r_exec(u.r);
      break;
    case OP_IMM:
    case LOAD:
    case MISC_MEM:
    case SYSTEM:
      i_decode(&u.i, mli);
      i_exec(u.i);
      break;
    case STORE:
      s_decode(&u.s, mli);
      s_exec(u.s);
      break;
    case BRANCH:
      b_decode(&u.b, mli);
      b_exec(u.b);
      break;
    case LUI:
    case AUIPC:
      u_decode(&u.u, mli);
      u_exec(u.u);
      break;
    case JAL:
      j_decode(&u.j, mli);
      j_exec(u.j);
      break;
    default:
      // TODO: raise illegal-instruction exception
      // technically UNSPECIFIED behavior
      printf("[ERROR]\t\tUnknown opcode: 0x%02x\n", opcode);
      return;
  }
}

void inst_debug(const inst32_t mli) {
  uint8_t opcode = mli & 0b1111111;
  switch (opcode) {
    case OP:
      printf("type r instrution\n");
      break;
    case OP_IMM:
      printf("type i instruction\n");
      break;
    default:
      printf("unknown type\n");
      break;
  }
}
