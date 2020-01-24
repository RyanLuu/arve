#include "reg.h"

#include <stdint.h>
#include <stdio.h>

#include "inst.h"

int32_t x[32] = {0};
uint32_t pc = 0;

void set_reg(uint8_t i, int32_t value) {
  x[i] = value;
  x[0] = 0;
}

int32_t get_reg(uint8_t i) { return x[i]; }

uint32_t incr_pc() {
  pc += sizeof(inst32_t);
  return pc;
}

void reg_debug() {
  for (int i = 0; i < 32; i++) {
    printf("x%d\t0x%08x\n", i, x[i]);
  }
  printf("pc\t0x%08x\n", pc);
}