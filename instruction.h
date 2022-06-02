#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disassembler.h"

void mov_I2R_oper(instruction* ins, char* binary_data);
void int_TS_oper(instruction* ins, char* binary_data);
void add_RMR2E_oper(instruction* ins, char* binary_data);
void mov_RMR_oper(instruction* ins, char* binary_data);
void xor_RMRE_oper(instruction* ins, char* binary_data);
void lea_LEAR_oper(instruction* ins, char* binary_data);
void cmp_IRM_oper(instruction* ins, char* binary_data);
void IRM_2_oper(instruction* ins, char* binary_data);
void IDRM_4_oper(instruction* ins, char* binary_data);
void jne_oper(instruction* ins, char* binary_data);
void push_R_oper(instruction* ins, char* binary_data);
void call_DS_oper(instruction* ins, char* binary_data);
void hlt_oper(instruction* ins, char* binary_data);