#include "structure.h"

unsigned char* read_buffer;         
instructions_list* asem_result;     /*save result*/
int *buffer_ptr;
char hexadecimal_table[16];
char* register_16bit_table[8];
char* register_8bit_table[8];
int register_status[8];

void asem_output(instructions_list* list);
void list_add(instruction_node* node);
void instruction_jump(instruction* ins, int ins_num);
void decimalToHexadecimal(int decimal, char* result);
void register_addressing_8bit(int data, char* reg);
void register_addressing_16bit(int data, char* reg);
void byte_complement(int byte_data, char* result);
/*disassembler functions*/
void MOD_RM_REG_process(instruction* ins, int offset);
void MOD_RM_process(instruction* ins, int offset, int flag);
int read_disp(instruction* ins, int offset, int flag);
int read_data(instruction* ins, int offset, int flag);
void intialize_register_table();
void register_writeline();
void register_content_output();