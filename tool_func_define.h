#include "structure.h"

unsigned char* read_buffer;         
instructions_list* asem_result;     /*save result*/
int *buffer_ptr;
const char hexadecimal_table[16];
const char* register_16bit_table[8];
const char* register_8bit_table[8];
int register_status[8];
unsigned char* virtual_memory;
int *mem_ptr;

void asem_output(instructions_list* list);
void list_add(instruction_node* node);
void instruction_jump(instruction* ins, int ins_num);
void decimalToHexadecimal(int decimal, char* result);
char* register_addressing_8bit(int data);
char* register_addressing_16bit(int data);
void byte_complement(int byte_data, char* complement);
/*disassembler functions*/
void MOD_RM_REG_process(instruction* ins, int offset);
void MOD_RM_process(instruction* ins, int offset, int flag);
int read_disp(instruction* ins, int offset, int flag);
int read_data(instruction* ins, int offset, int flag);
void intialize_register_table();
void register_writeline();
void register_content_output();