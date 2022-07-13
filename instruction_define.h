#include "structure.h"

/*instruction functions*/
void mov_I2R_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void int_TS_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void add_RMR2E_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void mov_RMR_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void xor_RMRE_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void lea_LEAR_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void cmp_IRM_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void IRM_2_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void jnb_JNBAE_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void IDRM_4_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void jne_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void push_R_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void ff_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void call_DS_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void hlt_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void jmp_DS_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void cbw_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void dec_R_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void jl_JLNGE_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void LOGIC_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void je_JEZ_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void je_JEZ_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void pop_R_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void jnl_JNLGE_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void or_RMRE_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void or_IA_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void ret_WS_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void ret_WSAI_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void ret_I_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void ret_IAI_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void in_FP_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void in_VP_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void sbb_RMRE_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
void jmp_DSS_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int data_start, int flag);
