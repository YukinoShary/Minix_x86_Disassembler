#include "instruction.c"

/*proccess functions*/
void asem_output(instructions_list* list);
char* text_to_instruction(exec* hdr);
void read_header(exec* hdr, char* openfile);
void list_add(instruction_node* node);
void instruction_jump(instruction* ins, int ins_num);
/*disassembler functions*/
void MOD_RM_REG_process(instruction* ins, int offset);
void MOD_RM_process(instruction* ins, int offset, int flag);
int read_disp(instruction* ins, int offset, int flag);
int read_data(instruction* ins, int offset, int flag);
/*tool functions*/
void decimalToHexadecimal(int decimal, char* result);
void register_addressing_8bit(int data, char* reg);
char* register_addressing_16bit(int data, char* reg);
void byte_complement(int byte_data, char* result);
/*instruction functions*/
void mov_I2R_oper(instruction* ins, char* read_buffer, int* buffer_ptr, int flag);
void int_TS_oper(instruction* ins, char* binary_data);
void add_RMR2E_oper(instruction* ins, char* binary_data);
void mov_RMR_oper(instruction* ins, char* binary_data);
void xor_RMRE_oper(instruction* ins, char* binary_data);
void lea_LEAR_oper(instruction* ins, char* binary_data);
void cmp_IRM_oper(instruction* ins, char* binary_data);
void IRM_2_oper(instruction* ins, char* binary_data);
void jnb_JNBAE_oper(instruction* ins, char* binary_data);
void IDRM_4_oper(instruction* ins, char* binary_data);
void jne_oper(instruction* ins, char* binary_data);
void push_R_oper(instruction* ins, char* binary_data);
void call_DS_oper(instruction* ins, char* binary_data);
void hlt_oper(instruction* ins);
void jmp_DS_oper(instruction* ins);
void cbw_oper(instruction* ins);