#include "tool_func_define.h"

/*TODO: deal with disp and data*/
void mov_I2R_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    int offset, byte_data;
    char* hexadecimal;
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
    decimalToHexadecimal(ins->data0, hexadecimal);
    strcpy(&(ins->asem[0]), "MOV 00");
    stpcpy(&(ins->asem[6]), hexadecimal);
    offset = 8;
    if(ins->w)
    {
        /*the third byte*/
        ins->data1 = (int)read_buffer[*buffer_ptr];
        *buffer_ptr += 1;
        ins->length = 24;
        strcpy(&ins->asem[offset], "00");
        offset += 2;
        decimalToHexadecimal(ins->data1, hexadecimal);
        strcpy(&(ins->asem[offset]), hexadecimal);
        ins->asem[offset + 2] = '\0';
        offset += 3;
    }
    else
    {
        ins->asem[offset] = '\0';
        offset ++;
    }  
    free(hexadecimal);
    list_add(node);
    
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void int_TS_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
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
    strcpy(&ins->asem[0], "INT 00");
    decimalToHexadecimal(ins->type, hexadecimal);
    strcpy(&(ins->asem[6]), hexadecimal);
    offset = 7;
    free(hexadecimal);

    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void add_RMR2E_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    int byte_data;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->d = byte_data & 0x02 >> 1;
    ins->w = byte_data & 0x01;

    /*the second byte*/
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->reg = (byte_data & 0x38) >> 3;
    ins->rm = byte_data & 0x07;
    ins->length = 16;

    /*set the asem*/
    strcpy(&(ins->asem[0]), "ADD ");
    MOD_RM_REG_process(ins, 4);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void mov_RMR_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    int i, byte_data, offset;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->d = byte_data & 0x02 >> 1;
    ins->w = byte_data & 0x01;

    /*read the second byte*/
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->reg = (byte_data & 0x38) >> 3;
    ins->rm = byte_data & 0x07;
    ins->length = 16;

    strcpy(&(ins->asem[0]), "MOV ");
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
    int byte_data, offset;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->d = byte_data & 0x02 >> 1;
    ins->w = byte_data & 0x01;

    /*read the second byte*/
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->reg = (byte_data & 0x38) >> 3;
    ins->rm = byte_data & 0x07;
    ins->length = 16;

    strcpy(&ins->asem[0], "XOR ");
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
    int byte_data, offset;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->d = byte_data & 0x02 >> 1;
    ins->w = byte_data & 0x01;

    /*read the second byte*/
    *buffer_ptr += 1;
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->reg = (byte_data & 0x38) >> 3;
    ins->rm = byte_data & 0x07;
    ins->length = 16;

    strcpy(&(ins->asem[0]), "LEA ");
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
    int offset, byte_data;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    
    /*read the second byte*/
    *buffer_ptr += 1;
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->rm = byte_data & 0x07;
    ins->length = 16;

    if((byte_data & 0x1c >> 2) == 0x05)
    {
        strcpy(&ins->asem[0], "SUB ");
        offset = 4;
        MOD_RM_process(ins, offset, 1);
    }
    else if((byte_data & 0x1c >> 2) == 0x07)
    {
        strcpy(&ins->asem[0], "CMP ");
        offset = 4;
        MOD_RM_process(ins, offset, 1);
    }
    else if((byte_data & 0x1c >> 2) == 0x01)
    {
        strcpy(&ins->asem[0], "OR ");
        offset = 3;
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
    strcpy(&ins->asem[0],"JNB 00");
    offset = 6;
    strcpy(&ins->asem[offset], hexadecimal);
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
    ins->length = 16;

    if((byte_data & 0x1c >> 2) == 0x00)
    {
        strcpy(&ins->asem[0], "TEST ");
        offset = 5;
        MOD_RM_process(ins, offset, 1);
    }
    else if((byte_data & 0x1c >> 2) == 0x03)
    {
        strcpy(&ins->asem[0],"NGE ");
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

    strcpy(&ins->asem[0], "JNE 00");
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
    int offset, byte_data;
    char *hexadecimal;
    instruction_node* node;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));

    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 8;
    ins->reg = byte_data & 0x07;
    
    register_addressing_16bit(ins->reg, hexadecimal);
    strcpy(&(ins->asem[0]), "PUSH ");
    offset = 5;
    strcpy(&(ins->asem[offset]), hexadecimal);

    free(hexadecimal);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void call_DS_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    int offset;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    *buffer_ptr += 1;
    ins->length = 8;
    offset = 0;
    strcpy(&(ins->asem[0]), "CALL ");
    offset = 5;

    ins->low_disp = read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 16;
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

    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    *buffer_ptr += 1;
    ins->length = 8;
    strcpy(&(ins->asem[0]), "HLT\0");
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void jmp_DS_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    int offset;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    *buffer_ptr += 1;
    ins->low_disp = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 16;
    strcpy(&(ins->asem[0]), "JMP ");
    offset = 4;
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
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    *buffer_ptr += 1;
    strcpy(&(ins->asem[0]), "CBW\0");

    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void dec_R_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    int byte_data, offset;
    char* hexadecimal;
    instruction_node* node;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));
    byte_data = read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 8;
    ins->reg = byte_data & 0x07;
    strcpy(&(ins->asem[0]), "DEC ");
    offset = 4;
    register_addressing_16bit(ins->reg, hexadecimal);
    offset += 2;
    ins->asem[offset] = '\0';
    offset ++;
    free(hexadecimal);

    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void jl_JLNGE_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
    instruction_node* node;
    int offset;
    char* hexadecimal;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));
    /*read the second byte*/
    *buffer_ptr += 1;
    ins->low_disp = read_buffer[*read_buffer];
    *read_buffer += 1;
    strcpy(&(ins->asem[0]), "JL 00");
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
    instruction_node* node;
    int byte_data, offset;
    node = malloc(sizeof(instruction_node));

    byte_data = (int)read_buffer[*read_buffer];
    *buffer_ptr += 1;
    ins->v = byte_data & 0x02 >> 1;
    ins->w = byte_data & 0x01;

    /*read the second byte*/
    byte_data = (int)read_buffer[*read_buffer];
    *buffer_ptr += 1;
    ins->length = 16;
    ins->mod = byte_data & 0xc0 >> 6;
    ins->rm = byte_data & 0x03;

    if((byte_data & 0x38 >> 3) == 0x04)
    {
        strcpy(&(ins->asem[0]), "SHL ");
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
    instruction_node* node;
    int offset;
    char* hexadecimal;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));

    /*read the second byte*/
    *buffer_ptr += 1;
    ins->low_disp = (int)read_buffer[*buffer_ptr];
    ins->length = 16;
    strcpy(&ins->asem[0], "je 00");
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
    instruction_node* node;
    int byte_data, offset;
    char* hexadecimal;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));
    byte_data = read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 8;
    ins->reg = byte_data & 0x07;
    strcpy(&ins->asem[0], "pop ");
    offset = 4;
    register_addressing_16bit(ins->reg, hexadecimal);
    strcpy(&ins->asem[offset], hexadecimal);
    offset += 2;
    ins->asem[offset] = '\0';
    free(hexadecimal);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    }
}

void jnl_JNLGE_oper(instruction* ins, unsigned char* read_buffer, int* buffer_ptr, int flag)
{
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
    strcpy(&ins->asem[0],"JNL 00");
    offset = 6;
    strcpy(&ins->asem[offset], hexadecimal);
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
    int offset, byte_data;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->d = byte_data & 0x02 >> 1;
    ins->w = byte_data & 0x01;

    /*read the second byte*/
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 16;
    ins->mod = byte_data & 0xc0 >> 6;
    ins->reg = byte_data & 0x38 >> 3;
    ins->rm = byte_data & 0x07;
    ins->length = 16;
    strcpy(&ins->asem[0], "OR ");
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
    int offset, byte_data;
    char* hexadecimal;
    instruction_node* node;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->w = byte_data & 0x01;
    strcpy(&ins->asem[0], "OR ");
    offset = 3;
    ins->low_disp = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->high_disp = (int)read_buffer[*buffer_ptr];
    *buffer_ptr += 1;
    ins->length = 24;
    decimalToHexadecimal(ins->data1, hexadecimal);
    strcpy(&ins->asem[offset], hexadecimal);
    offset += 2;
    decimalToHexadecimal(ins->data0, hexadecimal);
    strcpy(&ins->asem[offset], hexadecimal);
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
    strcpy(&ins->asem[offset], hexadecimal);
    offset += 2;
    strcpy(&ins->asem[offset], hexadecimal);
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
    strcpy(&ins->asem[offset], hexadecimal);
    offset += 2;
    strcpy(&ins->asem[offset], hexadecimal);
    offset += 2;
    ins->asem[offset] = '\0';

    free(hexadecimal);
    node->ins = ins;
    list_add(node);
    if(flag == 1)
    {
        
    } 
}