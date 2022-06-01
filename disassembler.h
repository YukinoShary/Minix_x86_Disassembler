#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct exec
{
    unsigned char a_magic[2];
    unsigned char a_flags;
    unsigned char a_cpu;
    unsigned char a_hdrlen;
    unsigned char a_unused;
    unsigned short a_version;
    long a_text;
    long a_data;
    long a_bss;
    long a_entry;
    long a_total;
    long a_syms;

    /*short from ends here */
    long a_trsize;
    long a_drsize;
    long a_tbase;
    long a_dbase;
} exec;

typedef struct instruction
{
    /*function code*/
    char reg[3];
    char w;
    char d;
    char s;
    char mod[2];
    char rm[2];
    char low_disp[8];
    char high_disp[8];
    char data0[8];
    char data1[8];
    char type[8];
    char asem[64];
    int length;
} instruction;

typedef struct instruction_node
{
    void* next;
    instruction* ins;

} instruction_node;

typedef struct instructions_list
{
    int length;
    instruction_node* front;
    instruction_node* rear;
} instructions_list;

unsigned char* read_buffer;         
instructions_list* asem_result;     /*save result*/
int *buffer_ptr; 

void asem_output(instructions_list* list);
void decimal2binary(int decimal, char* binary_text);
void MOD_RM_REG_process(instruction* ins, int offset);
void MOD_RM_process(instruction* ins, int offset);
void read_header(exec* hdr, char* openfile);
char* convertBinaryToHexadecimal(char* binary);
void list_add(instruction_node* node);
char* register_addressing_8bit(char* reg);
char* register_addressing_16bit(char* reg);
char* binary2complement(char binary[8]);
char* text_to_instruction(exec* hdr);
