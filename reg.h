#ifndef REG_H
#define REG_H

#include <stdint.h>

#define REG_ZERO (0)
#define REG_RA (1)
#define REG_SP (2)
#define REG_GP (3)
#define REG_TP (4)
#define REG_T0 (5)
#define REG_T1 (6)
#define REG_T2 (7)
#define REG_S0 (8)
#define REG_FP (8)
#define REG_S1 (9)
#define REG_A0 (10)
#define REG_A1 (11)
#define REG_A2 (12)
#define REG_A3 (13)
#define REG_A4 (14)
#define REG_A5 (15)
#define REG_A6 (16)
#define REG_A7 (17)
#define REG_S2 (18)
#define REG_S3 (19)
#define REG_S4 (20)
#define REG_S5 (21)
#define REG_S6 (22)
#define REG_S7 (23)
#define REG_S8 (24)
#define REG_S9 (25)
#define REG_S10 (26)
#define REG_S11 (27)
#define REG_T3 (28)
#define REG_T4 (29)
#define REG_T5 (30)
#define REG_T6 (31)

extern int32_t x[];
extern uint32_t pc;

extern void set_reg(uint8_t i, int32_t value);
extern int32_t get_reg(uint8_t i);

extern uint32_t incr_pc();
extern void reg_debug();

#endif /* REG_H */
