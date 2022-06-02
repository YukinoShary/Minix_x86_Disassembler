#include "instruction.h"

/*TODO: deal with disp and data*/
void mov_I2R_oper(instruction* ins, char* binary_data)
{
    int i, decimal;
    instruction_node* node;
    char* reg, *hexadecimal;
    node = malloc(sizeof(instruction_node));
    ins->w = binary_data[4] - '0';
    for(i = 0; i <= 2; i++)
    {
        ins->reg[i] = binary_data[i + 5];
    }

    /*the second byte*/
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    decimal2binary(decimal, binary_data);
    ins->length = 16;
    
    /*set the asem text*/
    ins->asem[0] = 'M';
    ins->asem[1] = 'O';
    ins->asem[2] = 'V';
    ins->asem[3] = ' ';
    for(i = 0; i <= 7; i++)
    {
        ins->asem[i + 4] = binary_data[i];
    }
    if(ins->w == '1')
    {
        /*the third byte*/
        decimal = (int)read_buffer[*buffer_ptr];
        *buffer_ptr ++;
        decimal2binary(decimal, binary_data);
        ins->length = 24;
        for(i = 0; i <= 7; i++)
        {
            ins->asem[i + 12] = binary_data[i];
        }
        ins->asem[21] = '\0';
    }
    else
        ins->asem[12] = '\0';
    
    node->ins = ins;
    list_add(node);
}

void int_TS_oper(instruction* ins, char* binary_data)
{
    int i, decimal;
    char* hexadecimal;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    decimal2binary(decimal, binary_data);
    for(i = 0; i <= 7; i++)
    {
        ins->type[i] = binary_data[i];
    }
    ins->length = 16;

    /*set the asem*/
    ins->asem[0] = 'I';
    ins->asem[1] = 'N';
    ins->asem[2] = 'T';
    ins->asem[3] = ' ';
    hexadecimal = convertBinaryToHexadecimal(ins->type);
    ins->asem[4] = hexadecimal[0];
    ins->asem[5] = hexadecimal[1];

    node->ins = ins;
    list_add(node);
}

void add_RMR2E_oper(instruction* ins, char* binary_data)
{
    int i, decimal;
    char *hexadecimal, *binary, *reg;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    ins->d = binary_data[6] - '0';
    ins->w = binary_data[7] - '0';

    /*the second byte*/
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    decimal2binary(decimal, binary_data);

    for(i = 0; i <= 1; i++)
    {
        ins->mod[i] = binary_data[i];
    }
    for(i = 2; i <= 4; i++)
    {
        ins->reg[i - 2] = binary_data[i];
    }
    for(i = 5; i <= 7; i++)
    {
        ins->rm[i - 5] = binary_data[i];
    }
    ins->length = 16;

    /*set the asem*/
    ins->asem[0] = 'A';
    ins->asem[1] = 'D';
    ins->asem[2] = 'D';
    ins->asem[3] = ' ';

    MOD_RM_REG_process(ins, 4);  

    node->ins = ins;
    list_add(node);
}

void mov_RMR_oper(instruction* ins, char* binary_data)
{
    int i, decimal;
    char *hexadecimal, *binary, *reg;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    ins->d = binary_data[6] - '0';
    ins->w = binary_data[7] - '0';

    /*read the second byte*/
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    decimal2binary(decimal, binary_data);

    for(i = 0; i <= 1; i++)
    {
        ins->mod[i] = binary_data[i];
    }
    for(i = 2; i <= 4; i++)
    {
        ins->reg[i - 2] = binary_data[i];
    }
    for(i = 5; i <= 7; i++)
    {
        ins->rm[i - 5] = binary_data[i];
    }
    ins->length = 16;

    ins->asem[0] = 'M';
    ins->asem[1] = 'O';
    ins->asem[2] = 'V';
    ins->asem[3] = ' ';

    MOD_RM_REG_process(ins, 4);
    node->ins = ins;
    list_add(node);
}

void xor_RMRE_oper(instruction* ins, char* binary_data)
{
    int i, decimal;
    char *hexadecimal, *binary, *reg;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    ins->d = binary_data[6] - '0';
    ins->w = binary_data[7] - '0';

    /*read the second byte*/
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    decimal2binary(decimal, binary_data);

    for(i = 0; i <= 1; i++)
    {
        ins->mod[i] = binary_data[i];
    }
    for(i = 2; i <= 4; i++)
    {
        ins->reg[i - 2] = binary_data[i];
    }
    for(i = 5; i <= 7; i++)
    {
        ins->rm[i - 5] = binary_data[i];
    }
    ins->length = 16;

    ins->asem[0] = 'X';
    ins->asem[1] = 'O';
    ins->asem[2] = 'R';
    ins->asem[3] = ' ';

    MOD_RM_REG_process(ins, 4);
    node->ins = ins;
    list_add(node);
}

void lea_LEAR_oper(instruction* ins, char* binary_data)
{
    int i, decimal;
    char *hexadecimal, *binary, *reg;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    ins->d = binary_data[6] - '0';
    ins->w = binary_data[7] - '0';

    /*read the second byte*/
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    decimal2binary(decimal, binary_data);

    for(i = 0; i <= 1; i++)
    {
        ins->mod[i] = binary_data[i];
    }
    for(i = 2; i <= 4; i++)
    {
        ins->reg[i - 2] = binary_data[i];
    }
    for(i = 5; i <= 7; i++)
    {
        ins->rm[i - 5] = binary_data[i];
    }
    ins->length = 16;

    ins->asem[0] = 'L';
    ins->asem[1] = 'E';
    ins->asem[2] = 'A';
    ins->asem[3] = ' ';

    MOD_RM_REG_process(ins, 4);
    node->ins = ins;
    list_add(node);
}

void IRM_2_oper(instruction* ins, char* binary_data)
{
    int i, decimal;
    char *hexadecimal, *binary, *reg;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    
    /*read the second byte*/
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    decimal2binary(decimal, binary_data);
    for(i = 0; i <=1; i++)
    {
        ins->mod[i] = binary_data[i];
    }
    for(i = 5; i <= 7; i++)
    {
        ins->rm[i - 5] = binary_data[i];
    }
    ins->length = 16;

    if(binary_data[2] == '1' && binary_data[3] == '0' && binary_data[4] == '1')
    {
        ins->asem[0] = 'S';
        ins->asem[1] = 'U';
        ins->asem[2] = 'B';
        ins->asem[3] = ' ';
        MOD_RM_process(ins, 4, 1);
    }
    else if(binary_data[2] == '1' && binary_data[3] == '1' && binary_data[4] == '1')
    {
        ins->asem[0] = 'C';
        ins->asem[1] = 'M';
        ins->asem[2] = 'P';
        ins->asem[3] = ' ';
        MOD_RM_process(ins, 4, 1);
    }

    node->ins = ins;
    list_add(node);
}

void jnb_JNBAE_interpreter(instruction* ins, char* binary_data)
{
    int i, decimal;
    char *hexadecimal, *binary, *reg;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));

    /*read the second byte*/
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    decimal2binary(decimal, binary_data);
    ins->length = 16;
    
    hexadecimal = convertBinaryToHexadecimal(binary_data);
    ins->asem[0] = 'J';
    ins->asem[1] = 'N';
    ins->asem[2] = 'B';
    ins->asem[3] = ' ';
    ins->asem[4] = hexadecimal[0];
    ins->asem[5] = hexadecimal[1];

    node->ins = ins;
    list_add(node);
}

void IDRM_4_oper(instruction* ins, char* binary_data)
{
    int i, decimal;
    char *hexadecimal, *binary, *reg;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    ins->w = binary_data[7] - '0';
    
    /*read the second byte*/
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    decimal2binary(decimal, binary_data);
    for(i = 0; i <=1; i++)
    {
        ins->mod[i] = binary_data[i];
    }
    for(i = 5; i <= 7; i++)
    {
        ins->rm[i - 5] = binary_data[i];
    }
    ins->length = 16;

    if(binary_data[2] == '0' && binary_data[3] == '0' && binary_data[4] == '0')
    {
        ins->asem[0] = 'T';
        ins->asem[1] = 'E';
        ins->asem[2] = 'S';
        ins->asem[3] = 'T';
        ins->asem[4] = ' ';

        MOD_RM_process(ins, 5, 1);
    }

    node->ins = ins;
    list_add(node);
}

void jne_oper(instruction* ins, char* binary_data)
{
    int i, decimal;
    char *hexadecimal;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));

    /*read the second byte*/
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    decimal2binary(decimal, binary_data);
    ins->length = 16;

    ins->asem[0] = 'J';
    ins->asem[1] = 'N';
    ins->asem[2] = 'E';
    ins->asem[3] = ' ';
    for(i = 0; i <= 7; i++)
    {
        ins->asem[i + 4] = binary_data[i];
    }
    ins->asem[12] = '\0';
    node->ins = ins;
    list_add(node);
}

void push_R_oper(instruction* ins, char* binary_data)
{
    int i, decimal;
    char *hexadecimal;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));

    for(i = 4; i <= 7; i++)
    {
        ins->reg[i - 4] = binary_data[i];
    }
    ins->length = 8;
    hexadecimal = register_addressing_16bit(ins->reg);
    ins->asem[0] = 'P';
    ins->asem[1] = 'U';
    ins->asem[2] = 'S';
    ins->asem[3] = 'H';
    ins->asem[4] = ' ';
    ins->asem[5] = hexadecimal[0];
    ins->asem[6] = hexadecimal[1];
    ins->asem[7] = '\0';

    node->ins = ins;
    list_add(node);
}

void call_DS_oper(instruction* ins, char* binary_data)
{
    int offset;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    offset = 0;
    ins->asem[0] = 'C';
    ins->asem[1] = 'A';
    ins->asem[2] = 'L';
    ins->asem[3] = 'L';
    ins->asem[4] = ' ';
    offset = 5;
    offset = read_disp(ins, offset, 1);
    ins->asem[offset] = '\0';
    
    node->ins = ins;
    list_add(node);
}

void hlt_oper(instruction* ins, char* binary_data)
{
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    ins->asem[0] = 'H';
    ins->asem[1] = 'L';
    ins->asem[2] = 'T';
    ins->asem[3] = '\0';
    node->ins = ins;
    list_add(node);
}