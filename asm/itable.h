typedef enum type_t { R, I, S, B, U, J } type_t;

typedef struct instent_t {
  char *name;
  unsigned int nargs;
  unsigned short opcode;
  type_t type;
  unsigned short func3;
} instent_t;

const instent_t itable[] = {
    {"lui", 2, 0b0110111, U},          {"auipc", 2, 0b0010111, U},
    {"jal", 2, 0b1101111, J},          {"jalr", 3, 0b1100111, I, 0b000},
    {"beq", 3, 0b1100011, B, 0b000},   {"bne", 3, 0b1100011, B, 0b001},
    {"blt", 3, 0b1100011, B, 0b100},   {"bge", 3, 0b1100011, B, 0b101},
    {"bltu", 3, 0b1100011, B, 0b110},  {"bgeu", 3, 0b1100011, B, 0b111},
    {"lb", 3, 0b0000011, I, 0b000},    {"lh", 3, 0b0000011, I, 0b001},
    {"lw", 3, 0b0000011, I, 0b010},    {"lbu", 3, 0b0000011, I, 0b100},
    {"lhu", 3, 0b0000011, I, 0b101},   {"sb", 3, 0b0100011, S, 0b000},
    {"sh", 3, 0b0100011, S, 0b001},    {"sw", 3, 0b0100011, S, 0b010},
    {"addi", 3, 0b0010011, I, 0b000},  {"slti", 3, 0b0010011, I, 0b010},
    {"sltiu", 3, 0b0010011, I, 0b011}, {"xori", 3, 0b0010011, I, 0b100},
    {"ori", 3, 0b0010011, I, 0b110},   {"andi", 3, 0b0010011, I, 0b111},
    {"slli", 3, 0b0010011, I, 0b001},  {"srli", 3, 0b0010011, I, 0b101},
    {"srai", 3, 0b0010011, I, 0b101},  {"add", 3, 0b0110011, R, 0b000},
    {"sub", 3, 0b0110011, R, 0b000},   {"sll", 3, 0b0110011, R, 0b001},
    {"slt", 3, 0b0110011, R, 0b010},   {"sltu", 3, 0b0110011, R, 0b011},
    {"xor", 3, 0b0110011, R, 0b100},   {"srl", 3, 0b0110011, R, 0b101},
    {"sra", 3, 0b0110011, R, 0b101},   {"or", 3, 0b0110011, R, 0b110},
    {"and", 3, 0b0110011, R, 0b111},   {"fence", 2, 0b0110011, I, 0b000},
    {"ecall", 0, 0b1110011, I, 0b000}, {"ebreak", 0, 0b1110011, I, 0b000},
};

const int ninsts = sizeof(itable) / sizeof(instent_t);
