#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inst.h"
#include "reg.h"
#include "types.h"

#define N_INST (4)

int main() {
  byte_t inst_mem[sizeof(inst32_t) * N_INST] = {0};
  inst32_t inst_buf[N_INST] = {
      0b00000000000100000110000110010011, 0b00000000001000000110000110010011,
      0b00000000010000000000000101101111, 0b00000000000000000110000100010011};

  for (int i = 0; i < N_INST; i++) {
    memcpy(inst_mem + i * 4, inst_buf + i, 4);
  }

  while (pc <= (N_INST - 1) * sizeof(inst32_t)) {
    inst32_t i = *(inst32_t *)(inst_mem + pc);
    inst_exec(i);
  }
  reg_debug();

  return 0;
}
