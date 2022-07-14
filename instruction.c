#include "tool_func_define.h"

/*TODO: deal with disp and data*/
void mov_I2R_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("mov_I2R_oper\n");
    int offset, byte_data;
    char *hexadecimal, *reg;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    hexadecimal = malloc(8);
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 8;
    ins->w = (byte_data & 0x08) >> 3;
    ins->reg = byte_data & 0x07;
    
    /*the second byte*/
    ins->data0 = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 16;
    
    /*set the asem text*/
    strcpy(&(ins->asem[0]), "MOV ");
    offset = 4;
    reg = register_addressing_16bit(ins->reg);
    strcat(&ins->asem[0], reg);
    offset += 2;
    strcat(&ins->asem[0], ", ");
    offset += 2;
    if(ins->w)
    {
        /*the third byte*/
        ins->data1 = (int)read_buffer[*buffer_ptr];
        *buffer_ptr += 1;
        ins->length = 24;
        decimalToHexadecimal(ins->data1, hexadecimal);
        strcat(&(ins->asem[0]), hexadecimal);
        offset += 2;
        decimalToHexadecimal(ins->data0, hexadecimal);
        strcat(&ins->asem[0], hexadecimal);
        offset += 2;
        ins->asem[offset] = '\0';
    }
    else
    {
        decimalToHexadecimal(ins->data0, hexadecimal);
        strcat(&ins->asem[0], "00");
        offset += 2;
        strcat(&ins->asem[0], hexadecimal);
        offset += 2;
        ins->asem[offset] = '\0';
    }  
    free(hexadecimal);
    
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void int_TS_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("int_TS_oper\n");
    int offset;
    char* hexadecimal;
    instruction_node* node;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));
    *buffer_ptr += 1;
    ins->type = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 16;

    /*set the asem*/
    strcpy(&ins->asem[0], "INT ");
    decimalToHexadecimal(ins->type, hexadecimal);
    strcat(&(ins->asem[0]), hexadecimal);
    offset = 8;
    ins->asem[offset] = '\0';
    free(hexadecimal);

    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        /*int_TS_interp(ins, ins->type, data_start)*/
    }
}

void add_RMR2E_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("add_RMR2E_oper\n");
    int byte_data;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->d = (byte_data & 0x02) >> 1;
    ins->w = byte_data & 0x01;

    /*the second byte*/
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->reg = (byte_data & 0x38) >> 3;
    ins->rm = byte_data & 0x07;
    ins->length = 16;

    /*set the asem*/
    strcat(&(ins->asem[0]), "ADD ");
    MOD_RM_REG_process(ins, 4);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void mov_RMR_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("mov_RMR_oper\n");
    int i, byte_data, offset;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->d = (byte_data & 0x02) >> 1;
    ins->w = byte_data & 0x01;

    /*read the second byte*/
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->reg = (byte_data & 0x38) >> 3;
    ins->rm = byte_data & 0x07;
    ins->length = 16;

    strcat((&ins->asem[0]), "MOV ");
    offset = 4;

    MOD_RM_REG_process(ins, offset);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void xor_RMRE_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("xor_RMRE_oper\n");
    int byte_data, offset;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->d = (byte_data & 0x02) >> 1;
    ins->w = byte_data & 0x01;

    /*read the second byte*/
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->reg = (byte_data & 0x38) >> 3;
    ins->rm = byte_data & 0x07;
    ins->length = 16;

    strcat(&ins->asem[0], "XOR ");
    offset = 4;
    MOD_RM_REG_process(ins, 4);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void lea_LEAR_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("lea_LEAR_oper\n");
    int byte_data, offset;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    /*read the second byte*/
    ins->d = (byte_data & 0x02) >> 1;
    ins->w = byte_data & 0x01;

    /*read the third byte*/
    *buffer_ptr += 1;
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->reg = (byte_data & 0x38) >> 3;
    ins->rm = byte_data & 0x07;
    ins->length = 24;

    strcat(&(ins->asem[0]), "LEA ");
    offset = 4;
    MOD_RM_REG_process(ins, offset);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void IRM_2_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("IRM_2_oper\n");
    int offset, byte_data;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->s = (byte_data & 0x02) >> 1;
    ins->w = (byte_data & 0x01);

    /*read the second byte*/
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->rm = byte_data & 0x07;
    ins->length = 16;

    if(((byte_data & 0x1c) >> 2) == 0x05)
    {
        strcat(&ins->asem[0], "SUB ");
        offset = 4;
        MOD_RM_process(ins, offset, 1);
    }
    else if(((byte_data & 0x1c) >> 2) == 0x07)
    {
        strcat(&ins->asem[0], "CMP ");
        offset = 4;
        MOD_RM_process(ins, offset, 1);
    }
    else if(((byte_data & 0x1c) >> 2) == 0x01)
    {
        strcat(&ins->asem[0], "OR ");
        offset = 3;
        MOD_RM_process(ins, offset, 1);
    }
    else if(((byte_data & 0x1c) >> 2) == 0x00)
    {
        strcat(&ins->asem[0], "ADD ");
        offset = 4;
        MOD_RM_process(ins, offset, 1);
    }
    else if(((byte_data & 0x1c) >> 2) == 0x03)
    {
        strcat(&ins->asem[0], "SBB ");
        offset = 4;
        MOD_RM_process(ins, offset, 1);
    }
    /*TODO*/
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
    
}

void jnb_JNBAE_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("jnb_JNBAE_oper\n");
    int offset;
    char *hexadecimal;
    instruction_node* node;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));

    /*read the second byte*/
    *buffer_ptr += 1;
    ins->low_disp = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 16;
    
    decimalToHexadecimal(ins->low_disp, hexadecimal);
    strcat(&ins->asem[0],"JNB 00");
    offset = 6;
    strcat(&ins->asem[offset], hexadecimal);
    offset += 4;
    ins->asem[offset] = '\0';

    free(hexadecimal);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }   
}

void IDRM_4_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("IDRM_oper\n");
    int offset, byte_data;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;

    ins->w = byte_data & 0x01;
    
    /*read the second byte*/
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->rm = byte_data & 0x07;
    ins->w = -1;
    ins->length = 16;

    if(((byte_data & 0x1c) >> 3) == 0x00)
    {
        strcat(&ins->asem[0], "TEST ");
        offset = 5;
        MOD_RM_process(ins, offset, 1);
    }
    else if(((byte_data & 0x1c) >> 3) == 0x03)
    {
        strcat(&ins->asem[0],"NEG ");
        offset = 4;
        MOD_RM_process(ins, offset, 0);
    }
    /*TODO*/

    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void jne_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("jne_oper\n");
    int offset, byte_data;
    char* hexadecimal;
    instruction_node* node;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));

    /*read the second byte*/
    *buffer_ptr += 1;
    ins->low_disp = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 16;

    strcat(&ins->asem[0], "JNE 00");
    offset = 6;
    decimalToHexadecimal(ins->low_disp, hexadecimal);
    offset += 4;
    ins->asem[offset] = '\0';
    free(hexadecimal);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void push_R_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("push_R_oper\n");
    int offset, byte_data;
    char *reg;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));

    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 8;
    ins->reg = byte_data & 0x07;
    
    reg = register_addressing_16bit(ins->reg);
    strcat(&(ins->asem[0]), "PUSH ");
    offset = 5;
    strcat(&(ins->asem[0]), reg);
    offset += 2;
    ins->asem[offset] = '\0';
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void ff_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("ff_oper\n");
    int offset, byte_data;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    /*read the second byte*/
    *buffer_ptr += 1;
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 16;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->rm = byte_data & 0x07;
    ins->w = -1;/*for MOD_RM_process*/
    if(((byte_data & 0x38) >> 3) == 0x06)
    {
        strcpy(&ins->asem[0], "PUSH ");
        offset = 5;
        MOD_RM_process(ins, offset, 0);
    }
    else if(((byte_data & 0x38) >> 3) == 0x04)
    {
        strcpy(&ins->asem[0], "JMP ");
        offset = 4;
        MOD_RM_process(ins, offset, 0);
    }
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void call_DS_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("call_DS_oper\n");
    int offset;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    *buffer_ptr += 1;
    ins->length = 8;
    offset = 0;
    strcat(&(ins->asem[0]), "CALL ");
    offset = 5;

    offset = read_disp(ins, offset, 1);
    ins->asem[offset] = '\0';
    offset ++;
    
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void hlt_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("htl_oper\n");
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    *buffer_ptr += 1;
    ins->length = 8;
    strcat(&(ins->asem[0]), "HLT\0");
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void jmp_DS_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("jmp_oper\n");
    int offset;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    *buffer_ptr += 1;
    strcat(&(ins->asem[0]), "JMP ");
    offset = 4;
    ins->length = 8;
    offset = read_disp(ins, offset, 1);
    ins->asem[offset] = '\0';
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void cbw_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("cbw_oper\n");
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    *buffer_ptr += 1;
    strcat(&(ins->asem[0]), "CBW\0");

    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void dec_R_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("dec_R_oper\n");
    int byte_data, offset;
    char *reg;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 8;
    ins->reg = byte_data & 0x07;
    strcat(&(ins->asem[0]), "DEC ");
    offset = 4;
    reg = register_addressing_16bit(ins->reg);
    offset += 2;
    ins->asem[offset] = '\0';
    offset ++;

    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void jl_JLNGE_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("jl_JLNGE_oper\n");
    instruction_node* node;
    int offset;
    char* hexadecimal;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));
    /*read the second byte*/
    *buffer_ptr += 1;
    ins->low_disp = read_buffer[*read_buffer];
    *read_buffer += 1;
    strcat(&(ins->asem[0]), "JL 00");
    offset = 5;
    decimalToHexadecimal(ins->low_disp, hexadecimal);
    offset += 4;
    ins->asem[offset] = '\0';

    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void LOGIC_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("LOGIC_oper\n");
    instruction_node* node;
    int byte_data, offset;
    node = malloc(sizeof(instruction_node));

    byte_data = (int)read_buffer[*read_buffer];
    *buffer_ptr += 1;
    ins->v = (byte_data & 0x02) >> 1;
    ins->w = byte_data & 0x01;

    /*read the second byte*/
    byte_data = (int)read_buffer[*read_buffer];
    *buffer_ptr += 1;
    ins->length = 16;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->rm = byte_data & 0x07;
    ins->w = -1;

    if(((byte_data & 0x38) >> 3) == 0x04)
    {
        strcat(&(ins->asem[0]), "SHL ");
        offset = 4;
    }
    /*TODO*/
    MOD_RM_process(ins, offset, 0);
    ins->asem[offset] = '\0';
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void je_JEZ_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("je_JEZ_oper\n");
    instruction_node* node;
    int offset;
    char* hexadecimal;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));

    /*read the second byte*/
    *buffer_ptr += 1;
    ins->low_disp = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 16;
    strcat(&ins->asem[0], "je 00");
    offset = 5;
    decimalToHexadecimal(ins->low_disp, hexadecimal);
    offset += 2;
    ins->asem[offset] = '\0';
    free(hexadecimal);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void pop_R_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("pop_R_oper\n");
    instruction_node* node;
    int byte_data, offset;
    char* reg;
    node = malloc(sizeof(instruction_node));
    byte_data = read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 8;
    ins->reg = byte_data & 0x07;
    strcat(&ins->asem[0], "POP ");
    offset = 4;
    reg = register_addressing_16bit(ins->reg);
    strcat(&ins->asem[0], reg);
    offset += 2;
    ins->asem[offset] = '\0';
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void jnl_JNLGE_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("jnl_JNLGE_oper\n");
    int offset;
    char *hexadecimal;
    instruction_node* node;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));

    /*read the second byte*/
    *buffer_ptr += 1;
    ins->low_disp = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 16;
    
    decimalToHexadecimal(ins->low_disp, hexadecimal);
    strcat(&ins->asem[0],"JNL 00");
    offset = 6;
    strcat(&ins->asem[0], hexadecimal);
    offset += 4;
    ins->asem[offset] = '\0';

    free(hexadecimal);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }   
}

void or_RMRE_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("or_RMRE_oper\n");
    int offset, byte_data;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->d = (byte_data & 0x02) >> 1;
    ins->w = byte_data & 0x01;

    /*read the second byte*/
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 16;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->reg = (byte_data & 0x38) >> 3;
    ins->rm = byte_data & 0x07;
    strcat(&ins->asem[0], "OR ");
    offset = 3;
    MOD_RM_REG_process(ins, offset);

    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }  
}

void or_IA_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("or_IA_oper\n");
    int offset, byte_data;
    char* hexadecimal;
    instruction_node* node;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->w = byte_data & 0x01;
    strcat(&ins->asem[0], "OR ");
    offset = 3;
    ins->low_disp = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->high_disp = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 24;
    decimalToHexadecimal(ins->data1, hexadecimal);
    strcat(&ins->asem[0], hexadecimal);
    offset += 2;
    decimalToHexadecimal(ins->data0, hexadecimal);
    strcat(&ins->asem[0], hexadecimal);
    offset += 2;
    ins->asem[offset] = '\0';

    free(hexadecimal);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    } 
}

void ret_WS_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("ret_WS_oper\n");
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    *buffer_ptr += 1;
    ins->length = 8;
    strcpy(&ins->asem[0], "RET\0");
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    } 
}

void ret_WSAI_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("ret_WSAI_oper\n");
    int offset, byte_data;
    char* hexadecimal;
    instruction_node* node;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));
    *buffer_ptr += 1;
    /*read the second byte*/
    ins->data0 = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    /*read the third byte*/
    ins->data1 = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 24;
    strcpy(&ins->asem[0], "RET ");
    offset = 4;
    decimalToHexadecimal(ins->data1, hexadecimal);
    strcat(&ins->asem[0], hexadecimal);
    offset += 2;
    decimalToHexadecimal(ins->data0, hexadecimal);
    strcat(&ins->asem[0], hexadecimal);
    offset += 2;
    ins->asem[offset] = '\0';

    free(hexadecimal);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    } 
}

void ret_I_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("ret_I_oper\n");
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    *buffer_ptr += 1;
    ins->length = 8;
    strcpy(&ins->asem[0], "RET\0");
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    } 
}

void ret_IAI_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("ret_IAI_oper\n");
    int offset, byte_data;
    char* hexadecimal;
    instruction_node* node;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));
    *buffer_ptr += 1;
    /*read the second byte*/
    ins->data0 = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    /*read the third byte*/
    ins->data1 = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 24;
    strcpy(&ins->asem[0], "RET ");
    offset = 4;
    decimalToHexadecimal(ins->data1, hexadecimal);
    strcat(&ins->asem[0], hexadecimal);
    offset += 2;
    decimalToHexadecimal(ins->data0, hexadecimal);
    strcat(&ins->asem[0], hexadecimal);
    offset += 2;
    ins->asem[offset] = '\0';

    free(hexadecimal);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    } 
}

void in_FP_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("in_FP_oper\n");
    instruction_node* node;
    int byte_data, offset;
    char* hexadecimal;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->w = byte_data & 0x01;
    /*read the second byte*/
    ins->port = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 16;
    strcpy(&ins->asem[0], "IN ");
    offset = 3;
    if(ins->w == 1)
    {
        strcat(&ins->asem[0], register_addressing_16bit(0));
    }
    else
    {
        strcat(&ins->asem[0], register_addressing_8bit(0));
    }
    offset += 2;
    decimalToHexadecimal(ins->port, hexadecimal);
    strcat(&ins->asem[0], hexadecimal);
    offset += 2;
    ins->asem[offset] = '\0';
    free(hexadecimal);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    } 
}

void in_VP_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("in_VP_oper\n");
    instruction_node* node;
    int byte_data, offset;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->w = byte_data & 0x01;
    ins->length = 8;
    strcpy(&ins->asem[0], "IN ");
    offset = 3;
    if(ins->w == 1)
        strcat(&ins->asem[0], register_addressing_16bit(0));
    else if(ins->w == 0)
        strcat(&ins->asem[0], register_addressing_8bit(0));
    offset += 2;
    strcat(&ins->asem[0], " dx");
    offset += 3;
    ins->asem[offset] = '\0';
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    } 
}

void sbb_RMRE_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("sbb_RMRE_oper\n");
    instruction_node* node;
    int byte_data, offset;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->d = (byte_data & 0x02) >> 1;
    ins->w = byte_data & 0x01;
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 16;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->reg = (byte_data & 0x38) >> 3;
    ins->rm = byte_data & 0x07;
    strcpy(&ins->asem[0], "SBB ");
    offset = 4;
    MOD_RM_REG_process(ins, offset);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {

    }
}

void sbb_IA_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("sbb_IA_oper\n");
    instruction_node* node;
    char* hexadecimal;
    int byte_data, offset;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->w =  byte_data & 0x01;
    /*second byte*/
    ins->data0 = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 16;
    strcpy(&ins->asem[0], "SBB ");
    offset = 4;

    if(ins->w == 1)
    {
        ins->data1 = (int)read_buffer[*buffer_ptr];
        *buffer_ptr += 1;
        ins->length = 24;
        decimalToHexadecimal(ins->data1, hexadecimal);
        strcat(&ins->asem[0], hexadecimal);
        decimalToHexadecimal(ins->data0, hexadecimal);
        strcat(&ins->asem[0], hexadecimal);
        offset += 4;
    }
    else
    {
        decimalToHexadecimal(ins->data0, hexadecimal);
        strcat(&ins->asem[0], "00");
        strcat(&ins->asem[0], hexadecimal);
        offset += 4;
    }
    ins->asem[offset] = '\0';
    free(hexadecimal);

    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {

    }
}

void jmp_DSS_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    printf("jmp_DSS_oper\n");
    instruction_node* node;
    int offset;
    char* hexadecimal;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));
    /*read the second byte*/
    *buffer_ptr += 1;
    ins->low_disp = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 16;
    strcpy(&ins->asem[0], "JMP SHORT ");
    offset = 10;
    decimalToHexadecimal(ins->low_disp, hexadecimal);
    strcat(&ins->asem[0], "00");
    offset += 2;
    strcat(&ins->asem[0], hexadecimal);
    offset += 2;
    ins->asem[offset] = '\0';
    free(hexadecimal);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {

    }
}