#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disassembler.h"

void mov_I2R_interpreter(instruction* ins, char* binary_data);
void int_TS_interpreter(instruction* ins, char* binary_data);
void add_RMR2E_interpreter(instruction* ins, char* binary_data);
void sub_IRM_interpreter(instruction* ins, char* binary_data);
void mov_RMR_interpreter(instruction* ins, char* binary_data);
void xor_RMRE_interpreter(instruction* ins, char* binary_data);
void lea_LEAR_interpreter(instruction* ins, char* binary_data);
void cmp_IRM_interpreter(instruction* ins, char* binary_data);