#include "tool_funcs.c"

/*TODO: deal with disp and data*/
void mov_I2R_oper(instruction* ins, char* read_buffer, int* buffer_ptr, int flag)
{
    int offset, byte_data;
    char* hexadecimal;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    hexadecimal = malloc(8);
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    ins->length = 8;
    ins->w = (byte_data & 0x08) >> 3;
    ins->reg = byte_data & 0x07;
    
    /*the second byte*/
    ins->data0 = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    ins->length = 16;
    
    /*set the asem text*/
    decimalToHexadecimal(ins->data0, hexadecimal);
    strcpy(&(ins->asem[0]), "MOV ");
    stpcpy(&(ins->asem[4]), hexadecimal);
    offset = 6;
    if(ins->w)
    {
        /*the third byte*/
        byte_data = (int)read_buffer[*buffer_ptr];
        *buffer_ptr ++;
        ins->length = 24;
        decimalToHexadecimal(byte_data, hexadecimal);
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
    
    node->ins = ins;
    if(flag == 0)
    {
        list_add(node);
    }
    else if(flag == 1)
    {
        
    }
}

void int_TS_oper(instruction* ins, char* read_buffer, int* buffer_ptr, int flag)
{
    int offset;
    char* hexadecimal;
    instruction_node* node;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));
    *buffer_ptr ++;
    ins->type = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    ins->length = 16;

    /*set the asem*/
    strcpy(&ins->asem[0], "INT ");
    decimalToHexadecimal(ins->type, hexadecimal);
    strcpy(&(ins->asem[4]), hexadecimal);
    offset = 7;
    free(hexadecimal);

    node->ins = ins;
    if(flag == 0)
    {
        list_add(node);
    }
    else if(flag == 1)
    {
        
    }
}

void add_RMR2E_oper(instruction* ins, char* read_buffer, int* buffer_ptr, int flag)
{
    int byte_data;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr++;
    ins->d = byte_data & 0x02 >> 1;
    ins->w = byte_data & 0x01;

    /*the second byte*/
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->reg = (byte_data & 0x38) >> 3;
    ins->rm = byte_data & 0x07;
    ins->length = 16;

    /*set the asem*/
    strcpy(&(ins->asem[0]), "ADD ");
    MOD_RM_REG_process(ins, 4);
    node->ins = ins;
    if(flag == 0)
    {
        list_add(node);
    }
    else if(flag == 1)
    {
        
    }
}

void mov_RMR_oper(instruction* ins, char* read_buffer, int* buffer_ptr, int flag)
{
    int i, byte_data, offset;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr++;
    ins->d = byte_data & 0x02 >> 1;
    ins->w = byte_data & 0x01;

    /*read the second byte*/
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->reg = (byte_data & 0x38) >> 3;
    ins->rm = byte_data & 0x07;
    ins->length = 16;

    strcpy(&(ins->asem[0]), "MOV ");
    offset = 4;

    MOD_RM_REG_process(ins, offset);
    node->ins = ins;
    if(flag == 0)
    {
        list_add(node);
    }
    else if(flag == 1)
    {
        
    }
}

void xor_RMRE_oper(instruction* ins, char* read_buffer, int* buffer_ptr, int flag)
{
    int byte_data, offset;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr++;
    ins->d = byte_data & 0x02 >> 1;
    ins->w = byte_data & 0x01;

    /*read the second byte*/
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->reg = (byte_data & 0x38) >> 3;
    ins->rm = byte_data & 0x07;
    ins->length = 16;

    strcpy(&ins->asem[0], "XOR ");
    offset = 4;
    MOD_RM_REG_process(ins, 4);
    node->ins = ins;
    if(flag == 0)
    {
        list_add(node);
    }
    else if(flag == 1)
    {
        
    }
}

void lea_LEAR_oper(instruction* ins, char* read_buffer, int* buffer_ptr, int flag)
{
    int byte_data, offset;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr++;
    ins->d = byte_data & 0x02 >> 1;
    ins->w = byte_data & 0x01;

    /*read the second byte*/
    *buffer_ptr ++;
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->reg = (byte_data & 0x38) >> 3;
    ins->rm = byte_data & 0x07;
    ins->length = 16;

    strcpy(&(ins->asem[0]), "LEA ");
    offset = 4;
    MOD_RM_REG_process(ins, offset);
    node->ins = ins;
    if(flag == 0)
    {
        list_add(node);
    }
    else if(flag == 1)
    {
        
    }
}

void IRM_2_oper(instruction* ins, char* read_buffer, int* buffer_ptr, int flag)
{
    int offset, byte_data;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    
    /*read the second byte*/
    *buffer_ptr ++;
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
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
    node->ins = ins;
    if(flag == 0)
    {
        list_add(node);
    }
    else if(flag == 1)
    {

    }
    
}

void jnb_JNBAE_oper(instruction* ins, char* read_buffer, int* buffer_ptr, int flag)
{
    int offset;
    char *hexadecimal;
    instruction_node* node;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));

    /*read the second byte*/
    *buffer_ptr ++;
    ins->low_disp = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    ins->length = 16;
    
    decimalToHexadecimal(ins->low_disp, hexadecimal);
    strcpy(&ins->asem[0],"JNB ");
    offset = 4;
    strcpy(&ins->asem[offset], hexadecimal);
    offset += 3;

    free(hexadecimal);
    node->ins = ins;
    if(flag == 0)
    {
        list_add(node);
    }
    else if(flag == 1)
    {

    }
}

void IDRM_4_oper(instruction* ins, char* read_buffer, int* buffer_ptr, int flag)
{
    int offset, byte_data;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;

    ins->w = byte_data & 0x01;
    
    /*read the second byte*/
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    ins->mod = (byte_data & 0xc0) >> 6;
    ins->rm = byte_data & 0x07;
    ins->length = 16;

    if((byte_data & 0x1c >> 2) == 0)
    {
        strcpy(&ins->asem[0], "TEST ");
        offset = 5;
        MOD_RM_process(ins, offset, 1);
    }

    node->ins = ins;
    if(flag == 0)
    {
        list_add(node);
    }
    else if(flag == 1)
    {

    }
}

void jne_oper(instruction* ins, char* read_buffer, int* buffer_ptr, int flag)
{
    int offset, byte_data;
    char* hexadecimal;
    instruction_node* node;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));

    /*read the second byte*/
    *buffer_ptr ++;
    ins->low_disp = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    ins->length = 16;

    strcpy(&ins->asem[0], "JNE ");
    offset = 4;
    decimalToHexadecimal(ins->low_disp, hexadecimal);
    offset += 3;
    node->ins = ins;
    free(hexadecimal);
    if(flag == 0)
    {
        list_add(node);
    }
    else if(flag == 1)
    {

    }
}

void push_R_oper(instruction* ins, char* read_buffer, int* buffer_ptr, int flag)
{
    int offset, byte_data;
    char *hexadecimal;
    instruction_node* node;
    hexadecimal = malloc(8);
    node = malloc(sizeof(instruction_node));

    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    ins->length = 8;
    ins->reg = byte_data & 0x07;
    
    register_addressing_16bit(ins->reg, hexadecimal);
    strcpy(&(ins->asem[0]), "PUSH ");
    offset = 5;
    strcpy(&(ins->asem[offset]), hexadecimal);

    free(hexadecimal);
    if(flag == 0)
    {
        list_add(node);
    }
    else if(flag == 1)
    {

    }
}

void call_DS_oper(instruction* ins, char* read_buffer, int* buffer_ptr, int flag)
{
    int offset;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    *buffer_ptr ++;
    offset = 0;
    strcpy(&(ins->asem[0]), "CALL ");
    offset = 5;
    offset = read_disp(ins, offset, 1);
    ins->asem[offset] = '\0';
    offset ++;
    if(flag == 0)
    {
        list_add(node);
    }
    else if(flag == 1)
    {

    }
}

void hlt_oper(instruction* ins, char* read_buffer, int* buffer_ptr, int flag)
{

    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    *buffer_ptr ++;
    strcpy(&(ins->asem[0]), "HLT\0");
    if(flag == 0)
    {
        list_add(node);
    }
    else if(flag == 1)
    {

    }
}

void jmp_DS_oper(instruction* ins, char* read_buffer, int* buffer_ptr, int flag)
{
    int offset, byte_data;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    byte_data = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    strcpy(&(ins->asem[0]), "JMP ");
    offset = 4;
    offset = read_disp(ins, offset, 1);
    ins->asem[offset] = '\0';
    if(flag == 0)
    {
        list_add(node);
    }
    else if(flag == 1)
    {

    }
}

void cbw_oper(instruction* ins, char* read_buffer, int* buffer_ptr, int flag)
{
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    *buffer_ptr ++;
    strcpy(&(ins->asem[0]), "CBW\0");
    if(flag == 0)
    {
        list_add(node);
    }
    else if(flag == 1)
    {

    }
}