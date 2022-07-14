#include "tool_func_define.h"

const char hexadecimal_table[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
const char *register_16bit_table[8] = {"AX\0", "CX\0", "DX\0", "BX\0", "SP\0", "BP\0", "SI\0", "DI\0"};
const char *register_8bit_table[8] = {"AL\0", "CL\0", "DL\0", "BL\0", "AH\0", "CH\0", "DH\0", "BH\0"};
int register_status[8];
unsigned char* virtual_memory;
int *mem_ptr;

unsigned char* read_buffer;         
instructions_list* asem_result;     /*save result*/
int *buffer_ptr; 

void asem_output(instructions_list* list)
{
    int i, position;
    char* asem;
    instruction_node* delete_node;

    position = 0;
    /*printf 1 instruction per loop*/
    while(list->length > 0)
    {
        /*TODO: printf original hexadecimal data*/
        i = 0;
        printf("%04x: ", position);
        /*read a single instruction*/
        asem = list->front->ins->asem;
        while(asem[i] != '\0')
        {
            printf("%c", asem[i]);
            i ++;
        }
        printf("\n");
        position += (list->front->ins->length) / 8;

        delete_node = list->front;
        list->front = list->front->next;
        free(delete_node);
        list->length -= 1;
    }

}

/* when flag == 1, read 2 byte*/
int read_disp(instruction* ins, int offset, int flag)
{
    char hexadecimal[5], complement[5];
    int i;
    if(flag)
    {
        /*2 byte*/
        ins->low_disp = (int)read_buffer[*buffer_ptr];
        *buffer_ptr += 1;
        ins->length += 8;
        ins->high_disp = (int)read_buffer[*buffer_ptr];
        *buffer_ptr += 1;
        ins->length += 8;
        decimalToHexadecimal(ins->high_disp, hexadecimal);
        ins->asem[offset] = hexadecimal[0];
        ins->asem[offset + 1] = hexadecimal[1];
        decimalToHexadecimal(ins->low_disp, hexadecimal);
        ins->asem[offset + 2] = hexadecimal[0];
        ins->asem[offset + 3] = hexadecimal[1];
        offset += 5;
    }
    else
    {
        /*1 byte*/
        ins->low_disp = (int)read_buffer[*buffer_ptr];
        *buffer_ptr += 1;
        ins->length += 8;

        if((ins->low_disp & 0x80) >> 7)
        {
            /*minus*/
            byte_complement(ins->low_disp, complement);
            ins->asem[offset] = '-';
            ins->asem[offset + 1] = complement[0];
            ins->asem[offset + 2] = complement[1];
            offset += 3;
        }
        else
        {
            decimalToHexadecimal(ins->low_disp, hexadecimal);
            ins->asem[offset] = hexadecimal[0];
            ins->asem[offset + 1] = hexadecimal[1];
            offset += 2;
        }
    }
    return offset;
}

/* when flag == 1, read 2 byte*/
int read_data(instruction* ins, int offset, int flag)
{
    char hexadecimal[5], complement[5];
    if(flag)
    {
        /*2 byte*/
        ins->data0 = (int)read_buffer[*buffer_ptr];
        *buffer_ptr += 1;
        ins->length += 8;
        ins->data1 = (int)read_buffer[*buffer_ptr];
        *buffer_ptr += 1;
        ins->length += 8;

        decimalToHexadecimal(ins->data1, hexadecimal);
        ins->asem[offset] = hexadecimal[0];
        ins->asem[offset + 1] = hexadecimal[1];
        decimalToHexadecimal(ins->data0, hexadecimal);
        ins->asem[offset + 2] = hexadecimal[0];
        ins->asem[offset + 3] = hexadecimal[1];
        offset += 4;
    }
    else
    {
        /*1 byte*/
        ins->data0 = (int)read_buffer[*buffer_ptr];
        *buffer_ptr += 1;
        ins->length += 8;

        if((ins->data0 & 0x80) >> 7)
        {
            /*minus*/
            byte_complement(ins->data0, complement);
            ins->asem[offset] = '-';
            ins->asem[offset + 1] = complement[0];
            ins->asem[offset + 2] = complement[1];
            offset += 3;
        }
        else
        {
            decimalToHexadecimal(ins->data0, hexadecimal);
            ins->asem[offset] = hexadecimal[0];
            ins->asem[offset + 1] = hexadecimal[1];
            offset += 2;
        }
    }
    return offset;
}

/*add item to instructions list*/
void list_add(instruction_node* node)
{
    if(asem_result->length == 0)
    {
        asem_result->front = asem_result->rear = node;
        asem_result->length = 1;
    }
    else
    {
        instruction_node* old;
        asem_result->rear->next = node;
        asem_result->rear = node;
        asem_result->length += 1;
    }
}

char* register_addressing_8bit(int data)
{
    return register_8bit_table[data];
}

char* register_addressing_16bit(int data)
{
    return register_16bit_table[data];
}

void decimalToHexadecimal(int decimal, char* result)
{
    int i, x;
    x = (decimal & 0xf0) >> 4;
    result[0] = hexadecimal_table[x];
    x = decimal & 0x0f;
    result[1] = hexadecimal_table[x];
    result[2] = '\0';
}

void byte_complement(int byte_data, char* result)
{
    int complement;
    complement = (~byte_data) + 1;
    result[0] = (complement & 0xf0) >> 4;
    result[1] = complement & 0x0f;
    result[2] = '\0';
}

/*Handle the r/m mod reg and disp*/
/*It is a really huge function*/
void MOD_RM_REG_process(instruction* ins, int offset)
{
    int i, decimal;
    char *reg;
    /*to reg*/
    if(ins->d == 1)
    {
        if(ins->w == 0)
        {
            reg = register_addressing_8bit(ins->reg);
        }
        else
        {
            reg = register_addressing_16bit(ins->reg);
        }

        /*reg*/
        strcpy(&ins->asem[offset], reg);
        offset += 2;
        strcpy(&ins->asem[offset], ", ");
        offset += 2;
        
        /*r/m & disp*/
        if(ins->mod == 0x00)
        {
            if(ins->rm == 0x00)
            {
                strcpy(&ins->asem[offset], "BX+SI");
                offset += 5;
            }
            else if(ins->rm == 0x01)
            {
                strcpy(&ins->asem[offset], "BX+DI");
                offset += 5;
            }
            else if(ins->rm == 0x02)
            {
                strcpy(&ins->asem[offset], "BP+SI");
                offset += 5;
            }
            else if(ins->rm == 0x03)
            {
                strcpy(&ins->asem[offset], "BP+DI");
                offset += 5;
            }
            else if(ins->rm == 0x04)
            {
                strcpy(&ins->asem[offset], "SI");
                offset += 2;
            }
            else if(ins->rm == 0x05)
            {
                strcpy(&ins->asem[offset], "DI");
                offset += 2;
            }
            /*here is an exception*/
            else if(ins->rm == 0x06)
            {
                offset = read_disp(ins, offset, 1);
            }
            else if(ins->rm == 0x07)
            {
                strcpy(&ins->asem[offset], "BX");
                offset += 2;
            }
            
        }
        
        else if(ins->mod == 0x01)
        {
            if(ins->rm == 0x00)
            {
                strcpy(&ins->asem[offset], "[BX+SI");
                offset += 6;
                offset = read_disp(ins, offset, 0);
                ins->asem[offset] = ']';
                ins->asem[offset + 1] = '\0';
                offset += 2;
            }
            else if(ins->rm == 0x01)
            {
                strcpy(&ins->asem[offset], "[BX+DI");
                offset += 6;
                offset = read_disp(ins, offset, 0);
                ins->asem[offset] = ']';
                ins->asem[offset + 1] = '\0';
                offset += 2;
            }
            else if(ins->rm == 0x02)
            {
                strcpy(&ins->asem[offset], "[BP+SI");
                offset += 6;
                offset = read_disp(ins, offset, 0);
                ins->asem[offset] = ']';
                ins->asem[offset + 1] = '\0';
                offset += 2;
            }
            else if(ins->rm == 0x03)
            {
                strcpy(&ins->asem[offset], "[BP+DI");
                offset += 6;
                offset = read_disp(ins, offset, 0);
                ins->asem[offset] = ']';
                ins->asem[offset + 1] = '\0';
                offset += 2;
            }
            else if(ins->rm == 0x04)
            {
                strcpy(&ins->asem[offset], "[SI");
                offset += 3;
                offset = read_disp(ins, offset, 0);
                ins->asem[offset] = ']';
                ins->asem[offset + 1] = '\0';
                offset += 2;
            }
            else if(ins->rm == 0x05)
            {
                strcpy(&ins->asem[offset], "[DI");
                offset += 3;
                offset = read_disp(ins, offset, 0);
                ins->asem[offset] = ']';
                ins->asem[offset + 1] = '\0';
                offset += 2;
            }
            else if(ins->rm == 0x06)
            {
                strcpy(&ins->asem[offset], "[BP");
                offset += 3;
                offset = read_disp(ins, offset, 0);
                ins->asem[offset] = ']';
                ins->asem[offset + 1] = '\0';
                offset += 2;
            }
            else if(ins->rm == 0x07)
            {
                strcpy(&ins->asem[offset], "[BX");
                offset += 3;
                offset = read_disp(ins, offset, 0);
                ins->asem[offset] = ']';
                ins->asem[offset + 1] = '\0';
                offset += 2;
            }
        }
        else if(ins->mod == 0x02)
        {
            if(ins->rm == 0x00)
            {
                strcpy(&ins->asem[offset], "[BX+SI");
                offset += 6;
                offset = read_disp(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset++;
            }
            if(ins->rm == 0x01)
            {
                strcpy(&ins->asem[offset], "[BX+DI");
                offset += 6;
                offset = read_disp(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset++;
            }
            if(ins->rm == 0x02)
            {
                strcpy(&ins->asem[offset], "[BP+SI");
                offset += 6;
                offset = read_disp(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset++;
            }
            if(ins->rm == 0x03)
            {
                strcpy(&ins->asem[offset], "[BP+DI");
                offset += 6;
                offset = read_disp(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset++;
            }
            if(ins->rm == 0x04)
            {
                strcpy(&ins->asem[offset], "[SI+");
                offset += 4;
                offset = read_disp(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset++;
            }
            if(ins->rm == 0x05)
            {
                strcpy(&ins->asem[offset], "[DI+");
                offset += 4;
                offset = read_disp(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset++;
            }
            if(ins->rm == 0x06)
            {
                strcpy(&ins->asem[offset], "[BP+");
                offset += 4;
                offset = read_disp(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset++;
            }
            if(ins->rm == 0x07)
            {
                strcpy(&ins->asem[offset], "[BX+");
                offset += 4;
                offset = read_disp(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset++;
            }
        }
        else if(ins->mod == 0x03)
        {
            if(ins->w == 1)
            {
                reg = register_addressing_16bit(ins->reg);
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                strcpy(&ins->asem[offset], ", ");
                offset += 2;
                reg = register_addressing_16bit(ins->rm);
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
                
            else
            {
                reg = register_addressing_8bit(ins->reg);
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                strcpy(&ins->asem[offset], ", ");
                offset += 2;
                reg = register_addressing_8bit(ins->rm);
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
    }
    /*from reg (reg behind)*/
    else if(ins->d == 0)
    {
        if(ins->mod == 0x00)
        {
            char *reg;
            if(ins->w == -1 || ins->w == 1)
                reg = register_addressing_16bit(ins->reg);
            else if(ins->w == 0)
                reg = register_addressing_8bit(ins->reg);
            if(ins->rm == 0x00)
            {
                strcpy(&ins->asem[offset], "[BX+SI], ");
                offset += 9;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->rm == 0x01)
            {
                strcpy(&ins->asem[offset], "[BX+DI], ");
                offset += 9;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->rm == 0x02)
            {
                strcpy(&ins->asem[offset], "[BP+SI], ");
                offset += 9;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->rm == 0x03)
            {
                strcpy(&ins->asem[offset], "[BP+DI], ");
                offset += 9;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->rm == 0x04)
            {
                strcpy(&ins->asem[offset], "SI, ");
                offset += 4;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->rm == 0x05)
            {
                strcpy(&ins->asem[offset], "DI, ");
                offset += 4;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            /*exception*/
            else if(ins->rm == 0x06)
            {
                offset = read_disp(ins, offset, 1);
                strcpy(&ins->asem[offset], ", ");
                offset += 2;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->rm == 0x07)
            {
                strcpy(&ins->asem[offset], "BX, ");
                offset += 4;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->mod == 0x01)
        {
            if(ins->w == -1 || ins->w == 1)
                reg = register_addressing_16bit(ins->reg);
            else if(ins->w == 0)
                reg = register_addressing_8bit(ins->reg);
            if(ins->rm == 0x00)
            {
                strcpy(&ins->asem[offset], "[BX+SI");
                offset += 6;
                offset = read_disp(ins, offset, 0);
                strcpy(&ins->asem[offset], "], ");
                offset += 3;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            if(ins->rm == 0x01)
            {
                strcpy(&ins->asem[offset], "[BX+DI");
                offset += 6;
                offset = read_disp(ins, offset, 0);
                strcpy(&ins->asem[offset], "], ");
                offset += 3;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            if(ins->rm == 0x02)
            {
                strcpy(&ins->asem[offset], "[BP+SI");
                offset += 6;
                offset = read_disp(ins, offset, 0);
                strcpy(&ins->asem[offset], "], ");
                offset += 3;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            if(ins->rm == 0x03)
            {
                strcpy(&ins->asem[offset], "[BP+DI");
                offset += 6;
                offset = read_disp(ins, offset, 0);
                strcpy(&ins->asem[offset], "], ");
                offset += 3;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            if(ins->rm == 0x04)
            {
                strcpy(&ins->asem[offset], "[SI");
                offset += 3;
                offset = read_disp(ins, offset, 0);
                strcpy(&ins->asem[offset], "], ");
                offset += 3;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            if(ins->rm == 0x05)
            {
                strcpy(&ins->asem[offset], "[DI");
                offset += 3;
                offset = read_disp(ins, offset, 0);
                strcpy(&ins->asem[offset], "], ");
                offset += 3;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            if(ins->rm == 0x06)
            {
                strcpy(&ins->asem[offset], "[BP");
                offset += 3;
                offset = read_disp(ins, offset, 0);
                strcpy(&ins->asem[offset], "], ");
                offset += 3;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            if(ins->rm == 0x07)
            {
                strcpy(&ins->asem[offset], "[BX");
                offset += 3;
                offset = read_disp(ins, offset, 0);
                strcpy(&ins->asem[offset], "], ");
                offset += 3;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->mod == 0x02)
        {
            if(ins->w == -1 || ins->w == 1)
                reg = register_addressing_16bit(ins->reg);
            else if(ins->w == 0)
                reg = register_addressing_8bit(ins->reg);
            if(ins->rm == 0x00)
            {
                strcpy(&ins->asem[offset], "[BX+SI+");
                offset += 7;
                offset = read_disp(ins, offset, 1);
                strcpy(&ins->asem[offset], "], ");
                offset += 3;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            if(ins->rm == 0x01)
            {
                strcpy(&ins->asem[offset], "[BX+DI+");
                offset += 7;
                offset = read_disp(ins, offset, 1);
                strcpy(&ins->asem[offset], "], ");
                offset += 3;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            if(ins->rm == 0x02)
            {
                strcpy(&ins->asem[offset], "[BP+SI+");
                offset += 7;
                offset = read_disp(ins, offset, 1);
                strcpy(&ins->asem[offset], "], ");
                offset += 3;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            if(ins->rm == 0x03)
            {
                strcpy(&ins->asem[offset], "BP+DI+");
                offset += 7;
                offset = read_disp(ins, offset, 1);
                strcpy(&ins->asem[offset], "], ");
                offset += 3;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            if(ins->rm == 0x04)
            {
                strcpy(&ins->asem[offset], "[SI+");
                offset += 4;
                offset = read_disp(ins, offset, 1);
                strcpy(&ins->asem[offset], "], ");
                offset += 3;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            if(ins->rm == 0x05)
            {
                strcpy(&ins->asem[offset], "[DI+");
                offset += 4;
                offset = read_disp(ins, offset, 1);
                strcpy(&ins->asem[offset], "], ");
                offset += 3;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            if(ins->rm == 0x06)
            {
                strcpy(&ins->asem[offset], "[BP+");
                offset += 4;
                offset = read_disp(ins, offset, 1);
                strcpy(&ins->asem[offset], "], ");
                offset += 3;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
            if(ins->rm == 0x07)
            {
                strcpy(&ins->asem[offset], "[BX+");
                offset += 4;
                offset = read_disp(ins, offset, 1);
                strcpy(&ins->asem[offset], "], ");
                offset += 3;
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->mod == 0x03)
        {
            if(ins->w == 1)
            {
                reg = register_addressing_16bit(ins->rm);
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                strcpy(&ins->asem[offset], ", ");
                offset += 2;
                reg = register_addressing_16bit(ins->reg);
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }              
            else if(ins->w == 0)
            {
                reg = register_addressing_8bit(ins->rm);
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                strcpy(&ins->asem[offset], ", ");
                offset += 2;
                reg = register_addressing_8bit(ins->reg);
                strcpy(&ins->asem[offset], reg);
                offset += 2;
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
    }
}

/*when flag == 1, read data*/
void MOD_RM_process(instruction* ins, int offset, int flag)
{
    int i, decimal;
    char *reg;
    if(ins->mod == 0x00)
    {
        if(ins->rm == 0x00)
        {
            strcpy(&ins->asem[offset], "[BX+SI], ");
            offset += 9;

            if(ins->s == 0 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->rm == 0x01)
        {
            strcpy(&ins->asem[offset], "[BX+DI], ");
            offset += 9;
            
            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->rm == 0x02)
        {
            strcpy(&ins->asem[offset], "[BP+SI], ");
            offset += 9;

            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->rm == 0x03)
        {
            strcpy(&ins->asem[offset], "[BP+DI], ");
            offset += 9;

            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->rm == 0x04)
        {
            strcpy(&ins->asem[offset], "SI, ");
            offset += 4;

            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 0 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->rm == 0x05)
        {
            strcpy(&ins->asem[offset], "DI, ");
            offset += 4;

            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        /*exception*/
        else if(ins->rm == 0x06)
        {          
            /*disp*/
            offset = read_disp(ins, offset, 1);
            offset ++;

            strcpy(&ins->asem[offset], ", ");
            offset += 2;

            /*read data*/
            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->rm == 0x07)
        {
            strcpy(&ins->asem[offset], "BX, ");
            offset += 4;

            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }  
    }
    else if(ins->mod == 0x01)
    {
        if(ins->rm == 0x00)
        {
            strcpy(&ins->asem[offset], "[BX+SI+");
            offset += 7;
            offset = read_disp(ins, offset, 0);
            strcpy(&ins->asem[offset], "], ");
            offset += 3;

            if(ins->s == 1 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->rm == 0x01)
        {
            strcpy(&ins->asem[offset], "[BX+DI+");
            offset += 7;
            offset = read_disp(ins, offset, 0);
            strcpy(&ins->asem[offset], "], ");
            offset += 3;

            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->rm == 0x02)
        {
            strcpy(&ins->asem[offset], "[BP+SI+");
            offset += 7;
            offset = read_disp(ins, offset, 0);
            strcpy(&ins->asem[offset], "], ");
            offset += 3;

            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        if(ins->rm == 0x03)
        {
            strcpy(&ins->asem[offset], "[BP+DI+");
            offset += 7;
            offset = read_disp(ins, offset, 0);
            strcpy(&ins->asem[offset], "], ");
            offset += 3;

            if(ins->s == 0 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        if(ins->rm == 0x04)
        {
            strcpy(&ins->asem[offset], "[SI+");
            offset += 4;
            offset = read_disp(ins, offset, 0);
            strcpy(&ins->asem[offset], "], ");
            offset += 3;

            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        if(ins->rm == 0x05)
        {
            strcpy(&ins->asem[offset], "[DI+");
            offset += 4;
            offset = read_disp(ins, offset, 0);
            strcpy(&ins->asem[offset], "], ");
            offset += 3;

            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        if(ins->rm == 0x06)
        {
            strcpy(&ins->asem[offset], "[BP+");
            offset += 4;
            offset = read_disp(ins, offset, 0);
            strcpy(&ins->asem[offset], "], ");
            offset += 3;

            if(ins->s == 0 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        if(ins->rm == 0x07)
        {
            strcpy(&ins->asem[offset], "[BX+");
            offset += 4;
            offset = read_disp(ins, offset, 0);
            strcpy(&ins->asem[offset], "], ");
            offset += 3;

            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
    }
    else if(ins->mod == 0x02)
    {
        if(ins->rm == 0x00)
        {   
            strcpy(&ins->asem[offset], "[BX+SI+");
            offset += 7;
            offset = read_disp(ins, offset, 1);
            strcpy(&ins->asem[offset], "], ");
            offset += 3;

            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->rm == 0x01)
        {         
            strcpy(&ins->asem[offset], "[BX+DI+");
            offset += 7;
            offset = read_disp(ins, offset, 1);
            strcpy(&ins->asem[offset], "], ");
            offset += 3;

            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->rm == 0x02)
        {
            strcpy(&ins->asem[offset], "[BP+SI+");
            offset += 7;
            offset = read_disp(ins, offset, 1);
            strcpy(&ins->asem[offset], "], ");
            offset += 3;

            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->rm == 0x03)
        {
            strcpy(&ins->asem[offset], "[BX+DI+");
            offset += 7;
            offset = read_disp(ins, offset, 1);
            strcpy(&ins->asem[offset], "], ");
            offset += 3;

            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->rm == 0x04)
        {
            strcpy(&ins->asem[offset], "[SI+");
            offset += 4;
            offset = read_disp(ins, offset, 1);
            strcpy(&ins->asem[offset], "], ");
            offset += 3;

            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->rm == 0x05)
        {
            strcpy(&ins->asem[offset], "[DI+");
            offset += 4;
            offset = read_disp(ins, offset, 1);
            strcpy(&ins->asem[offset], "], ");
            offset += 3;
            
            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->rm == 0x06)
        {
            strcpy(&ins->asem[offset], "[BP+");
            offset += 4;
            offset = read_disp(ins, offset, 1);
            strcpy(&ins->asem[offset], "], ");
            offset += 3;
            
            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
        else if(ins->rm == 0x07)
        {
            strcpy(&ins->asem[offset], "[BX+");
            offset += 4;
            offset = read_disp(ins, offset, 1);
            strcpy(&ins->asem[offset], "], ");
            offset += 3;
            
            if(ins->s == 0 && ins->w == 1 &&  flag == 1)
            {
                offset = read_data(ins, offset, 1);
                ins->asem[offset] = '\0';
                offset ++;
            }
            else if(ins->s == 1 && ins->w == 1 && flag == 1)
            {
                offset = read_data(ins, offset, 0);
                ins->asem[offset] = '\0';
                offset ++;
            }
        }
    }
    else if(ins->mod == 0x03)
    {
        if(ins->w == -1 || ins->w == 1)
            reg = register_addressing_16bit(ins->rm);
        else
            reg = register_addressing_8bit(ins->rm);
        strcpy(&ins->asem[offset], reg);
        offset += 2; 
        strcpy(&ins->asem[offset], ", ");
        offset += 2;
        
        if(ins->s == 0 && ins->w == 1 && flag == 1)
        {
            offset = read_data(ins, offset, 1);
            ins->asem[offset] = '\0';
            offset ++;
        }
        else if(ins->s == 1 && ins->w == 1 && flag == 1)
        {
            offset = read_data(ins, offset, 0);
            ins->asem[offset] = '\0';
            offset ++;
        }
    }
}


void intialize_register_table()
{
    int i;
    for(i = 0; i <= 7; i++)
    {
        register_status[i] = 0;
    }
}

void register_writeline()
{
    int i;
    for (i = 0; i <=7; i++)
    {
        printf("%s   ",register_16bit_table[i]);
    }
    printf("\n");
}

void register_content_output()
{
    
}