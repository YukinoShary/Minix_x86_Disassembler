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
    char seg_0[4];
    char seg_1[4];
    /*function code*/
    char reg[3];
    char w;
    char d;
    char s;
    char mod[2];
    char rm[2];
    char data0[8];
    char data1[8];
    char type[8];
    char asem[32];
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
void mov_I2R_interpreter(instruction* ins, char* binary_data);
void int_TS_interpreter(instruction* ins, char* binary_data);
void add_RMR2E_interpreter(instruction* ins, char* binary_data);
void sub_IRM_interpreter(instruction* ins, char* binary_data);


int main(int argc, char* argv[])
{
    exec* hd;
    read_buffer = malloc(16 * 1024);
    asem_result = malloc(sizeof(instructions_list));
    asem_result->length = 0;
    asem_result->front = NULL;
    hd = malloc(sizeof(exec));
    read_header(hd, argv[1]);
    text_to_instruction(hd);
    asem_output(asem_result);
    free(read_buffer);
    free(asem_result);
}

void read_header(exec* hdr, char* openfile)
{
    FILE* fp;
    size_t file_size;
    int i;
    unsigned char binary2long[4] = {0};
    fp = fopen(openfile, "rb");
    fread(&hdr->a_magic[0], sizeof(unsigned char), 1, fp);
    fread(&hdr->a_magic[1], sizeof(unsigned char), 1, fp);
    fread(&hdr->a_flags, sizeof(unsigned char), 1, fp);
    fread(&hdr->a_cpu, sizeof(unsigned char), 1, fp);
    fread(&(hdr->a_hdrlen), sizeof(unsigned char), 1, fp);
    fread(&hdr->a_unused, sizeof(unsigned char), 1, fp);
    fread(&hdr->a_version, sizeof(short), 1, fp);

    /*read a.out and put into buffer*/
    fseek(fp, 0L, SEEK_END);
    file_size = (size_t)ftell(fp);
    rewind(fp);
    read(read_buffer, 1, file_size, fp);
    *buffer_ptr = 8;

    /*deal with big endian*/
    for(i = 0; i < 4; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        *buffer_ptr ++;
    }
    hdr->a_text = (long)(binary2long[3] * 16 * 16 * 16 * 16 * 16 *16) +
        (binary2long[2] * 16 * 16 * 16 * 16) + (binary2long[1] * 16 * 16) + binary2long[0];

    for(i = 0; i < 4; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        *buffer_ptr ++;
    }
    hdr->a_data = (long)(binary2long[3] * 16 * 16 * 16 * 16 * 16 *16) +
        (binary2long[2] * 16 * 16 * 16 * 16) + (binary2long[1] * 16 * 16) + binary2long[0];

    for(i = 0; i < 4; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        *buffer_ptr ++;
    }
    hdr->a_bss = (long)(binary2long[3] * 16 * 16 * 16 * 16 * 16 *16) +
        (binary2long[2] * 16 * 16 * 16 * 16) + (binary2long[1] * 16 * 16) + binary2long[0];

    for(i = 0; i < 4; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        *buffer_ptr ++;
    }
    hdr->a_entry = (long)(binary2long[3] * 16 * 16 * 16 * 16 * 16 *16) +
        (binary2long[2] * 16 * 16 * 16 * 16) + (binary2long[1] * 16 * 16) + binary2long[0];

    for(i = 0; i < 4; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        *buffer_ptr ++;
    }
    hdr->a_total = (long)(binary2long[3] * 256 * 256 * 256) +
        (binary2long[2] * 256 * 256) + (binary2long[1] * 256) + binary2long[0];

    for(i = 0; i < 4; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        *buffer_ptr ++;
    }
    hdr->a_syms = (long)(binary2long[3] * 16 * 16 * 16 * 16 * 16 *16) +
        (binary2long[2] * 16 * 16 * 16 * 16) + (binary2long[1] * 16 * 16) + binary2long[0];
    
    *buffer_ptr = (int)hdr->a_hdrlen;
    printf("a_hdrlen:%hhu\n", hdr->a_hdrlen);
    printf("buffer pointer position:%d\n", *buffer_ptr);
    printf("text length:%ld\n", hdr->a_text);
}

/*byte to binary*/
void decimal2binary(int decimal, char* binary_data)
{
    int and_cul;
    and_cul = 255;
    for(int i = 0; i <= 7; i++)
    {
        if(and_cul & decimal == and_cul)
            binary_data[i] = 1;
        else
            binary_data[i] = 0;
    }
}

void asem_output(instructions_list* list)
{
    int i, position;
    char* asem;
    instruction_node* delete_node;

    position = 0;
    /*printf 1 instruction per loop*/
    while(list->length != 0)
    {
        /*TODO: printf original hexadecimal data*/
        i = 0;
        printf("%04d: ", position);
        /*read a single instruction*/
        asem = list->front->ins->asem;
        while(asem[i] != '\0')
        {
            printf("%c", asem[i]);
            i ++;
        }
        printf("\n");

        delete_node = list->front;
        list->front = list->front->next;
        free(delete_node->ins);
        free(delete_node);
        list->length --;
    }
}

/*Handle the r/m mod reg and disp*/
/*It is a really huge function*/
void MOD_RM_REG_process(instruction* ins, int offset)
{
    int i, decimal;
    char *hexadecimal, *reg;
    char binary[8];
    /*to reg*/
    if(ins->d = '1')
    {
        if(ins->w = '0')
        {
            reg = register_addressing_8bit(ins->reg);
        }
        else
        {
            reg = register_addressing_16bit(ins->reg);
        }

        /*reg*/
        ins->asem[offset] = reg[0];
        ins->asem[offset + 1] = reg[1];
        ins->asem[offset + 2] = ',';
        ins->asem[offset + 3] = ' ';
        ins->asem[offset + 4] = '[';
        
        /*r/m & disp*/
        if(strcmp(ins->mod, "00") == 0)
        {
            if(strcmp(ins->rm, "000") == 0)
            {
                ins->asem[offset + 5] = 'B';
                ins->asem[offset + 6] = 'X';
                ins->asem[offset + 7] = '+';
                ins->asem[offset + 8] = 'S';
                ins->asem[offset + 9] = 'I';
                ins->asem[offset + 10] = ']';
                ins->asem[offset + 11] = '\0';
            }
            if(strcmp(ins->rm, "001") == 0)
            {
                ins->asem[offset + 5] = 'B';
                ins->asem[offset + 6] = 'X';
                ins->asem[offset + 7] = '+';
                ins->asem[offset + 8] = 'D';
                ins->asem[offset + 9] = 'I';
                ins->asem[offset + 10] = ']';
                ins->asem[offset + 11] = '\0';
            }
            if(strcmp(ins->rm, "010") == 0)
            {
                ins->asem[offset + 5] = 'B';
                ins->asem[offset + 6] = 'P';
                ins->asem[offset + 7] = '+';
                ins->asem[offset + 8] = 'S';
                ins->asem[offset + 9] = 'I';
                ins->asem[offset + 10] = ']';
                ins->asem[offset + 11] = '\0';
            }
            if(strcmp(ins->rm, "011") == 0)
            {
                ins->asem[offset + 5] = 'B';
                ins->asem[offset + 6] = 'P';
                ins->asem[offset + 7] = '+';
                ins->asem[offset + 8] = 'D';
                ins->asem[offset + 9] = 'I';
                ins->asem[offset + 10] = ']';
                ins->asem[offset + 11] = '\0';
            }
            if(strcmp(ins->rm, "100") == 0)
            {
                ins->asem[offset + 5] = 'S';
                ins->asem[offset + 6] = 'I';
                ins->asem[offset + 7] = ']';
                ins->asem[offset + 8] = '\0';
            }
            if(strcmp(ins->rm, "101") == 0)
            {
                ins->asem[offset + 5] = 'D';
                ins->asem[offset + 6] = 'I';
                ins->asem[offset + 7] = ']';
                ins->asem[offset + 8] = '\0';
            }
            /*here is an exception*/
            if(strcmp(ins->rm, "110") == 0)
            {
                char front[8], rear[8];
                /*low disp*/
                decimal = (int)read_buffer[*buffer_ptr];
                *buffer_ptr ++;
                decimal2binary(decimal, binary);
                ins->length += 8;
                for (i = 0; i <= 7; i++)
                {
                    front[i] = binary[i];
                }
                /*high disp*/
                decimal = (int)read_buffer[*buffer_ptr];
                *buffer_ptr ++;
                decimal2binary(decimal, binary);
                ins->length += 8;
                char front[8], rear[8];
                for (i = 0; i <= 7; i++)
                {
                    rear[i] = binary[i];
                }
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 5] = hexadecimal[0];
                ins->asem[offset + 6] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 7] = hexadecimal[0];
                ins->asem[offset + 8] = hexadecimal[1];
                ins->asem[offset + 9] = ']';
                ins->asem[offset + 10] = '\0';
            }
            if(strcmp(ins->rm, "111") == 0)
            {
                ins->asem[offset + 5] = 'B';
                ins->asem[offset + 6] = 'X';
                ins->asem[offset + 7] = ']';
                ins->asem[offset + 8] = '\0';
            }
        }
        
        else if(strcmp(ins->mod, "01") == 0)
        {
            char front[8], rear[8];
            /*low disp*/
            decimal = (int)read_buffer[*buffer_ptr];
            *buffer_ptr ++;
            decimal2binary(decimal, binary);
            ins->length += 8;
            for (i = 0; i <= 7; i++)
            {
                front[i] = binary[i];
            }
            /*high disp*/
            decimal = (int)read_buffer[*buffer_ptr];
            *buffer_ptr ++;
            decimal2binary(decimal, binary);
            ins->length += 8;
            char front[8], rear[8];
            for (i = 0; i <= 7; i++)
            {
                rear[i] = binary[i];
            }
            /*minus*/
            if(strcmp(rear, "11111111") == 0)
            {
                char* completement;
                completement = binary2complement(front);
                hexadecimal = convertBinaryToHexadecimal(completement);
            }
            /*plus*/
            else
            {
                hexadecimal = convertBinaryToHexadecimal(front);    
            }

            if(strcmp(ins->rm, "000") == 0)
            {
                ins->asem[offset + 5] = 'B';
                ins->asem[offset + 6] = 'X';
                ins->asem[offset + 7] = '+';
                ins->asem[offset + 8] = 'S';
                ins->asem[offset + 9] = 'I';
                ins->asem[offset + 10] = '+';
                ins->asem[offset + 11] = hexadecimal[0];
                ins->asem[offset + 12] = hexadecimal[1];
                ins->asem[offset + 13] = ']';
                ins->asem[offset + 14] = '\0';
            }
            if(strcmp(ins->rm, "001") == 0)
            {
                ins->asem[offset + 5] = 'B';
                ins->asem[offset + 6] = 'X';
                ins->asem[offset + 7] = '+';
                ins->asem[offset + 8] = 'D';
                ins->asem[offset + 9] = 'I';
                ins->asem[offset + 10] = '+';
                ins->asem[offset + 11] = hexadecimal[0];
                ins->asem[offset + 12] = hexadecimal[1];
                ins->asem[offset + 13] = ']';
                ins->asem[offset + 14] = '\0';
            }
            if(strcmp(ins->rm, "010") == 0)
            {
                ins->asem[offset + 5] = 'B';
                ins->asem[offset + 6] = 'P';
                ins->asem[offset + 7] = '+';
                ins->asem[offset + 8] = 'S';
                ins->asem[offset + 9] = 'I';
                ins->asem[offset + 10] = '+';
                ins->asem[offset + 11] = hexadecimal[0];
                ins->asem[offset + 12] = hexadecimal[1];
                ins->asem[offset + 13] = ']';
                ins->asem[offset + 14] = '\0';
            }
            if(strcmp(ins->rm, "011") == 0)
            {
                ins->asem[offset + 5] = 'B';
                ins->asem[offset + 6] = 'P';
                ins->asem[offset + 7] = '+';
                ins->asem[offset + 8] = 'D';
                ins->asem[offset + 9] = 'I';
                ins->asem[offset + 10] = '+';
                ins->asem[offset + 11] = hexadecimal[0];
                ins->asem[offset + 12] = hexadecimal[1];
                ins->asem[offset + 13] = ']';
                ins->asem[offset + 14] = '\0';
            }
            if(strcmp(ins->rm, "100") == 0)
            {
                ins->asem[offset + 5] = 'S';
                ins->asem[offset + 6] = 'I';
                ins->asem[offset + 7] = '+';
                ins->asem[offset + 8] = hexadecimal[0];
                ins->asem[offset + 9] = hexadecimal[1];
                ins->asem[offset + 10] = ']';
                ins->asem[offset + 14] = '\0';
            }
            if(strcmp(ins->rm, "101") == 0)
            {
                ins->asem[offset + 5] = 'D';
                ins->asem[offset + 6] = 'I';
                ins->asem[offset + 7] = '+';
                ins->asem[offset + 8] = hexadecimal[0];
                ins->asem[offset + 9] = hexadecimal[1];
                ins->asem[offset + 10] = ']';
                ins->asem[offset + 11] = '\0';
            }
            if(strcmp(ins->rm, "110") == 0)
            {
                ins->asem[offset + 5] = 'B';
                ins->asem[offset + 6] = 'P';
                ins->asem[offset + 7] = '+';
                ins->asem[offset + 8] = hexadecimal[0];
                ins->asem[offset + 9] = hexadecimal[1];
                ins->asem[offset + 10] = ']';
                ins->asem[offset + 11] = '\0';
            }
            if(strcmp(ins->rm, "111") == 0)
            {
                ins->asem[offset + 5] = 'B';
                ins->asem[offset + 6] = 'X';
                ins->asem[offset + 7] = '+';
                ins->asem[offset + 8] = hexadecimal[0];
                ins->asem[offset + 9] = hexadecimal[1];
                ins->asem[offset + 10] = ']';
                ins->asem[offset + 11] = '\0';
            }
        }
        else if(strcmp(ins->mod, "10") == 0)
        {
            char front[8], rear[8];
            /*low disp*/
            decimal = (int)read_buffer[*buffer_ptr];
            *buffer_ptr ++;
            decimal2binary(decimal, binary);
            ins->length += 8;
            for (i = 0; i <= 7; i++)
            {
                front[i] = binary[i];
            }
            /*high disp*/
            decimal = (int)read_buffer[*buffer_ptr];
            *buffer_ptr ++;
            decimal2binary(decimal, binary);
            ins->length += 8;
            char front[8], rear[8];
            for (i = 0; i <= 7; i++)
            {
                rear[i] = binary[i];
            }

            if(strcmp(ins->rm, "000") == 0)
            {
                ins->asem[offset + 5] = 'B';
                ins->asem[offset + 6] = 'X';
                ins->asem[offset + 7] = '+';
                ins->asem[offset + 8] = 'S';
                ins->asem[offset + 9] = 'I';
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 10] = hexadecimal[0];
                ins->asem[offset + 11] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 12] = hexadecimal[0];
                ins->asem[offset + 13] = hexadecimal[1];
                ins->asem[offset + 14] = ']';
                ins->asem[offset + 15] = '\0';
            }
            if(strcmp(ins->rm, "001") == 0)
            {
                ins->asem[offset + 5] = 'B';
                ins->asem[offset + 6] = 'X';
                ins->asem[offset + 7] = '+';
                ins->asem[offset + 8] = 'D';
                ins->asem[offset + 9] = 'I';
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 10] = hexadecimal[0];
                ins->asem[offset + 11] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 12] = hexadecimal[0];
                ins->asem[offset + 13] = hexadecimal[1];
                ins->asem[offset + 14] = ']';
                ins->asem[offset + 15] = '\0';
            }
            if(strcmp(ins->rm, "010") == 0)
            {
                ins->asem[offset + 5] = 'B';
                ins->asem[offset + 6] = 'P';
                ins->asem[offset + 7] = '+';
                ins->asem[offset + 8] = 'S';
                ins->asem[offset + 9] = 'I';
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 10] = hexadecimal[0];
                ins->asem[offset + 11] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 12] = hexadecimal[0];
                ins->asem[offset + 13] = hexadecimal[1];
                ins->asem[offset + 14] = ']';
                ins->asem[offset + 15] = '\0';
            }
            if(strcmp(ins->rm, "011") == 0)
            {
                ins->asem[offset + 5] = 'B';
                ins->asem[offset + 6] = 'P';
                ins->asem[offset + 7] = '+';
                ins->asem[offset + 8] = 'D';
                ins->asem[offset + 9] = 'I';
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 10] = hexadecimal[0];
                ins->asem[offset + 11] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 12] = hexadecimal[0];
                ins->asem[offset + 13] = hexadecimal[1];
                ins->asem[offset + 14] = ']';
                ins->asem[offset + 15] = '\0';
            }
            if(strcmp(ins->rm, "100") == 0)
            {
                ins->asem[offset + 5] = 'S';
                ins->asem[offset + 6] = 'I';
                ins->asem[offset + 7] = '+';
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 8] = hexadecimal[0];
                ins->asem[offset + 9] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 10] = hexadecimal[0];
                ins->asem[offset + 11] = hexadecimal[1];
                ins->asem[offset + 12] = ']';
                ins->asem[offset + 13] = '\0';
            }
            if(strcmp(ins->rm, "101") == 0)
            {
                ins->asem[offset + 5] = 'D';
                ins->asem[offset + 6] = 'I';
                ins->asem[offset + 7] = '+';
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 8] = hexadecimal[0];
                ins->asem[offset + 9] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 10] = hexadecimal[0];
                ins->asem[offset + 11] = hexadecimal[1];
                ins->asem[offset + 12] = ']';
                ins->asem[offset + 13] = '\0';
            }
            if(strcmp(ins->rm, "110") == 0)
            {
                ins->asem[offset + 5] = 'B';
                ins->asem[offset + 6] = 'P';
                ins->asem[offset + 7] = '+';
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 8] = hexadecimal[0];
                ins->asem[offset + 9] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 10] = hexadecimal[0];
                ins->asem[offset + 11] = hexadecimal[1];
                ins->asem[offset + 12] = ']';
                ins->asem[offset + 13] = '\0';
            }
            if(strcmp(ins->rm, "111") == 0)
            {
                ins->asem[offset + 5] = 'B';
                ins->asem[offset + 6] = 'P';
                ins->asem[offset + 7] = '+';
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 8] = hexadecimal[0];
                ins->asem[offset + 9] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 10] = hexadecimal[0];
                ins->asem[offset + 11] = hexadecimal[1];
                ins->asem[offset + 12] = ']';
                ins->asem[offset + 13] = '\0';
            }
        }
        else if(strcmp(ins->mod, "11") == 0)
        {
            if(ins->w == '1')
                reg = register_addressing_16bit(ins->rm);
            else
                reg = register_addressing_8bit(ins->rm);
            ins->asem[offset + 5] = reg[0];
            ins->asem[offset + 6] = reg[1];
            ins->asem[offset + 7] = ']';
            ins->asem[offset + 8] = '\0';
        }
    }
    /*from reg (reg behind)*/
    else
    {
        ins->asem[offset] = '[';
        if(ins->w = '0')
        {
            reg = register_addressing_8bit(ins->reg);
        }
        else
        {
            reg = register_addressing_16bit(ins->reg);
        }
        if(strcmp(ins->mod, "00") == 0)
        {
            if(strcmp(ins->rm, "000") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'X';
                ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = 'S';
                ins->asem[offset + 5] = 'I';
                ins->asem[offset + 6] = ']';
                ins->asem[offset + 7] = ',';
                ins->asem[offset + 8] = ' ';
                ins->asem[offset + 9] = reg[0];
                ins->asem[offset + 10] = reg[1];
                ins->asem[offset + 11] = '\0';
            }
            if(strcmp(ins->rm, "001") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'X';
                ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = 'D';
                ins->asem[offset + 5] = 'I';
                ins->asem[offset + 6] = ']';
                ins->asem[offset + 7] = ',';
                ins->asem[offset + 8] = ' ';
                ins->asem[offset + 9] = reg[0];
                ins->asem[offset + 10] = reg[1];
                ins->asem[offset + 11] = '\0';
            }
            if(strcmp(ins->rm, "010") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'P';
                ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = 'S';
                ins->asem[offset + 5] = 'I';
                ins->asem[offset + 6] = ']';
                ins->asem[offset + 7] = ',';
                ins->asem[offset + 8] = ' ';
                ins->asem[offset + 9] = reg[0];
                ins->asem[offset + 10] = reg[1];
                ins->asem[offset + 11] = '\0';
            }
            if(strcmp(ins->rm, "011") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'P';
                ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = 'D';
                ins->asem[offset + 5] = 'I';
                ins->asem[offset + 6] = ']';
                ins->asem[offset + 7] = ',';
                ins->asem[offset + 8] = ' ';
                ins->asem[offset + 9] = reg[0];
                ins->asem[offset + 10] = reg[1];
                ins->asem[offset + 11] = '\0';
            }
            if(strcmp(ins->rm, "100") == 0)
            {
                ins->asem[offset + 1] = 'S';
                ins->asem[offset + 2] = 'I';
                ins->asem[offset + 3] = ']';
                ins->asem[offset + 4] = ',';
                ins->asem[offset + 5] = ' ';
                ins->asem[offset + 6] = reg[0];
                ins->asem[offset + 7] = reg[1];
                ins->asem[offset + 8] = '\0';
            }
            if(strcmp(ins->rm, "101") == 0)
            {
                ins->asem[offset + 1] = 'D';
                ins->asem[offset + 2] = 'I';
                ins->asem[offset + 3] = ']';
                ins->asem[offset + 4] = ',';
                ins->asem[offset + 5] = ' ';
                ins->asem[offset + 6] = reg[0];
                ins->asem[offset + 7] = reg[1];
                ins->asem[offset + 8] = '\0';
            }
            if(strcmp(ins->rm, "110") == 0)
            {
                /*exception*/
                char front[8], rear[8];
                /*low disp*/
                decimal = (int)read_buffer[*buffer_ptr];
                *buffer_ptr ++;
                decimal2binary(decimal, binary);
                ins->length += 8;
                for (i = 0; i <= 7; i++)
                {
                    front[i] = binary[i];
                }
                /*high disp*/
                decimal = (int)read_buffer[*buffer_ptr];
                *buffer_ptr ++;
                decimal2binary(decimal, binary);
                ins->length += 8;
                char front[8], rear[8];
                for (i = 0; i <= 7; i++)
                {
                    rear[i] = binary[i];
                }
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 1] = hexadecimal[0];
                ins->asem[offset + 2] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 3] = hexadecimal[0];
                ins->asem[offset + 4] = hexadecimal[1];
                ins->asem[offset + 5] = ']';
                ins->asem[offset + 6] = ',';
                ins->asem[offset + 7] = ' ';
                ins->asem[offset + 8] = reg[0];
                ins->asem[offset + 9] = reg[1];
                ins->asem[offset + 10] = '\0';
            }
            if(strcmp(ins->rm, "111") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'X';
                ins->asem[offset + 3] = ']';
                ins->asem[offset + 4] = ',';
                ins->asem[offset + 5] = ' ';
                ins->asem[offset + 6] = reg[0];
                ins->asem[offset + 7] = reg[1];
                ins->asem[offset + 8] = '\0';
            }
        }
        else if(strcmp(ins->mod, "01") == 0)
        {
            char front[8], rear[8];
            /*low disp*/
            decimal = (int)read_buffer[*buffer_ptr];
            *buffer_ptr ++;
            decimal2binary(decimal, binary);
            ins->length += 8;
            for (i = 0; i <= 7; i++)
            {
                front[i] = binary[i];
            }
            /*high disp*/
            decimal = (int)read_buffer[*buffer_ptr];
            *buffer_ptr ++;
            decimal2binary(decimal, binary);
            ins->length += 8;
            char front[8], rear[8];
            for (i = 0; i <= 7; i++)
            {
                rear[i] = binary[i];
            }
            /*minus*/
            if(strcmp(rear, "11111111") == 0)
            {
                char* completement;
                completement = binary2complement(front);
                hexadecimal = convertBinaryToHexadecimal(completement);
            }
            /*plus*/
            else
            {
                hexadecimal = convertBinaryToHexadecimal(front);    
            }

            if(strcmp(ins->rm, "000") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'X';
                ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = 'S';
                ins->asem[offset + 5] = 'I';
                ins->asem[offset + 6] = '+';
                ins->asem[offset + 7] = hexadecimal[0];
                ins->asem[offset + 8] = hexadecimal[1];
                ins->asem[offset + 9] = ']';
                ins->asem[offset + 10] = ',';
                ins->asem[offset + 11] = ' ';
                ins->asem[offset + 12] = reg[0];
                ins->asem[offset + 13] = reg[1];
                ins->asem[offset + 14] = '\0';
            }
            if(strcmp(ins->rm, "001") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'X';
                ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = 'D';
                ins->asem[offset + 5] = 'I';
                ins->asem[offset + 6] = '+';
                ins->asem[offset + 7] = hexadecimal[0];
                ins->asem[offset + 8] = hexadecimal[1];
                ins->asem[offset + 9] = ']';
                ins->asem[offset + 10] = ',';
                ins->asem[offset + 11] = ' ';
                ins->asem[offset + 12] = reg[0];
                ins->asem[offset + 13] = reg[1];
                ins->asem[offset + 14] = '\0';
            }
            if(strcmp(ins->rm, "010") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'P';
                ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = 'S';
                ins->asem[offset + 5] = 'I';
                ins->asem[offset + 6] = '+';
                ins->asem[offset + 7] = hexadecimal[0];
                ins->asem[offset + 8] = hexadecimal[1];
                ins->asem[offset + 9] = ']';
                ins->asem[offset + 10] = ',';
                ins->asem[offset + 11] = ' ';
                ins->asem[offset + 12] = reg[0];
                ins->asem[offset + 13] = reg[1];
                ins->asem[offset + 14] = '\0';
            }
            if(strcmp(ins->rm, "011") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'P';
                ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = 'D';
                ins->asem[offset + 5] = 'I';
                ins->asem[offset + 6] = '+';
                ins->asem[offset + 7] = hexadecimal[0];
                ins->asem[offset + 8] = hexadecimal[1];
                ins->asem[offset + 9] = ']';
                ins->asem[offset + 10] = ',';
                ins->asem[offset + 11] = ' ';
                ins->asem[offset + 12] = reg[0];
                ins->asem[offset + 13] = reg[1];
                ins->asem[offset + 14] = '\0';
            }
            if(strcmp(ins->rm, "100") == 0)
            {
                ins->asem[offset + 1] = 'S';
                ins->asem[offset + 2] = 'I';
                ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = hexadecimal[0];
                ins->asem[offset + 5] = hexadecimal[1];
                ins->asem[offset + 7] = ']';
                ins->asem[offset + 8] = ',';
                ins->asem[offset + 9] = ' ';
                ins->asem[offset + 10] = reg[0];
                ins->asem[offset + 11] = reg[1];
                ins->asem[offset + 12] = '\0';
            }
            if(strcmp(ins->rm, "101") == 0)
            {
                ins->asem[offset + 1] = 'D';
                ins->asem[offset + 2] = 'I';
                ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = hexadecimal[0];
                ins->asem[offset + 5] = hexadecimal[1];
                ins->asem[offset + 7] = ']';
                ins->asem[offset + 8] = ',';
                ins->asem[offset + 9] = ' ';
                ins->asem[offset + 10] = reg[0];
                ins->asem[offset + 11] = reg[1];
                ins->asem[offset + 12] = '\0';
            }
            if(strcmp(ins->rm, "110") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'P';
                ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = hexadecimal[0];
                ins->asem[offset + 5] = hexadecimal[1];
                ins->asem[offset + 7] = ']';
                ins->asem[offset + 8] = ',';
                ins->asem[offset + 9] = ' ';
                ins->asem[offset + 10] = reg[0];
                ins->asem[offset + 11] = reg[1];
                ins->asem[offset + 12] = '\0';
            }
            if(strcmp(ins->rm, "111") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'X';
                ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = hexadecimal[0];
                ins->asem[offset + 5] = hexadecimal[1];
                ins->asem[offset + 7] = ']';
                ins->asem[offset + 8] = ',';
                ins->asem[offset + 9] = ' ';
                ins->asem[offset + 10] = reg[0];
                ins->asem[offset + 11] = reg[1];
                ins->asem[offset + 12] = '\0';
            }
        }
        else if(strcmp(ins->mod, "10") == 0)
        {
            char front[8], rear[8];
            /*low disp*/
            decimal = (int)read_buffer[*buffer_ptr];
            *buffer_ptr ++;
            decimal2binary(decimal, binary);
            ins->length += 8;
            for (i = 0; i <= 7; i++)
            {
                front[i] = binary[i];
            }
            /*high disp*/
            decimal = (int)read_buffer[*buffer_ptr];
            *buffer_ptr ++;
            decimal2binary(decimal, binary);
            ins->length += 8;
            char front[8], rear[8];
            for (i = 0; i <= 7; i++)
            {
                rear[i] = binary[i];
            }

            if(strcmp(ins->rm, "000") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'X';
                ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = 'S';
                ins->asem[offset + 5] = 'I';
                ins->asem[offset + 6] = '+';
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 7] = hexadecimal[0];
                ins->asem[offset + 8] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 9] = hexadecimal[0];
                ins->asem[offset + 10] = hexadecimal[1];
                ins->asem[offset + 11] = ']';
                ins->asem[offset + 12] = ',';
                ins->asem[offset + 13] = ' ';
                ins->asem[offset + 14] = reg[0];
                ins->asem[offset + 15] = reg[1];
                ins->asem[offset + 16] = '\0';
            }
            if(strcmp(ins->rm, "001") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'X';
                ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = 'D';
                ins->asem[offset + 5] = 'I';
                ins->asem[offset + 6] = '+';
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 7] = hexadecimal[0];
                ins->asem[offset + 8] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 9] = hexadecimal[0];
                ins->asem[offset + 10] = hexadecimal[1];
                ins->asem[offset + 11] = ']';
                ins->asem[offset + 12] = ',';
                ins->asem[offset + 13] = ' ';
                ins->asem[offset + 14] = reg[0];
                ins->asem[offset + 15] = reg[1];
                ins->asem[offset + 16] = '\0';
            }
            if(strcmp(ins->rm, "010") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'P';
                ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = 'S';
                ins->asem[offset + 5] = 'I';
                ins->asem[offset + 6] = '+';
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 7] = hexadecimal[0];
                ins->asem[offset + 8] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 9] = hexadecimal[0];
                ins->asem[offset + 10] = hexadecimal[1];
                ins->asem[offset + 11] = ']';
                ins->asem[offset + 12] = ',';
                ins->asem[offset + 13] = ' ';
                ins->asem[offset + 14] = reg[0];
                ins->asem[offset + 15] = reg[1];
                ins->asem[offset + 16] = '\0';
            }
            if(strcmp(ins->rm, "011") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'P';
                ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = 'D';
                ins->asem[offset + 5] = 'I';
                ins->asem[offset + 6] = '+';
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 7] = hexadecimal[0];
                ins->asem[offset + 8] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 9] = hexadecimal[0];
                ins->asem[offset + 10] = hexadecimal[1];
                ins->asem[offset + 11] = ']';
                ins->asem[offset + 12] = ',';
                ins->asem[offset + 13] = ' ';
                ins->asem[offset + 14] = reg[0];
                ins->asem[offset + 15] = reg[1];
                ins->asem[offset + 16] = '\0';
            }
            if(strcmp(ins->rm, "100") == 0)
            {
                ins->asem[offset + 1] = 'S';
                ins->asem[offset + 2] = 'I';
                ins->asem[offset + 3] = '+';
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 4] = hexadecimal[0];
                ins->asem[offset + 5] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 6] = hexadecimal[0];
                ins->asem[offset + 7] = hexadecimal[1];
                ins->asem[offset + 8] = ']';
                ins->asem[offset + 9] = ',';
                ins->asem[offset + 10] = ' ';
                ins->asem[offset + 11] = reg[0];
                ins->asem[offset + 12] = reg[1];
                ins->asem[offset + 13] = '\0';
            }
            if(strcmp(ins->rm, "101") == 0)
            {
                ins->asem[offset + 1] = 'D';
                ins->asem[offset + 2] = 'I';
                ins->asem[offset + 3] = '+';
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 4] = hexadecimal[0];
                ins->asem[offset + 5] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 6] = hexadecimal[0];
                ins->asem[offset + 7] = hexadecimal[1];
                ins->asem[offset + 8] = ']';
                ins->asem[offset + 9] = ',';
                ins->asem[offset + 10] = ' ';
                ins->asem[offset + 11] = reg[0];
                ins->asem[offset + 12] = reg[1];
                ins->asem[offset + 13] = '\0';
            }
            if(strcmp(ins->rm, "110") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'P';
                ins->asem[offset + 3] = '+';
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 4] = hexadecimal[0];
                ins->asem[offset + 5] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 6] = hexadecimal[0];
                ins->asem[offset + 7] = hexadecimal[1];
                ins->asem[offset + 8] = ']';
                ins->asem[offset + 9] = ',';
                ins->asem[offset + 10] = ' ';
                ins->asem[offset + 11] = reg[0];
                ins->asem[offset + 12] = reg[1];
                ins->asem[offset + 13] = '\0';
            }
            if(strcmp(ins->rm, "111") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'X';
                ins->asem[offset + 3] = '+';
                hexadecimal = convertBinaryToHexadecimal(rear);
                ins->asem[offset + 4] = hexadecimal[0];
                ins->asem[offset + 5] = hexadecimal[1];
                hexadecimal = convertBinaryToHexadecimal(front);
                ins->asem[offset + 6] = hexadecimal[0];
                ins->asem[offset + 7] = hexadecimal[1];
                ins->asem[offset + 8] = ']';
                ins->asem[offset + 9] = ',';
                ins->asem[offset + 10] = ' ';
                ins->asem[offset + 11] = reg[0];
                ins->asem[offset + 12] = reg[1];
                ins->asem[offset + 13] = '\0';
            }
        }
        else if(strcmp(ins->mod, "11") == 0)
        {
            if(ins->w == '1')
            {
                reg = register_addressing_16bit(ins->rm);
                ins->asem[offset + 1] = reg[0];
                ins->asem[offset + 2] = reg[1];
                ins->asem[offset + 3] = ']';
                ins->asem[offset + 4] = ',';
                ins->asem[offset + 5] = ' ';
                reg = register_addressing_16bit(ins->rm);
                ins->asem[offset + 6] = reg[0];
                ins->asem[offset + 7] = reg[1];
                ins->asem[offset + 8] = '\0';
            }
            else
            {
                reg = register_addressing_8bit(ins->rm);
                ins->asem[offset + 1] = reg[0];
                ins->asem[offset + 2] = reg[1];
                ins->asem[offset + 3] = ']';
                ins->asem[offset + 4] = ',';
                ins->asem[offset + 5] = ' ';
                reg = register_addressing_8bit(ins->rm);
                ins->asem[offset + 6] = reg[0];
                ins->asem[offset + 7] = reg[1];
                ins->asem[offset + 8] = '\0';
            }
        }
    }
}

void MOD_RM_process(instruction* ins, int offset)
{
    int i, decimal;
    char *hexadecimal, *reg;
    char binary[8];

    if(strcmp(ins->mod, "00") == 0)
    {
        ins->asem[offset] = '[';
        if(strcmp(ins->rm, "000") == 0)
        {

        }
        else if(strcmp(ins->rm, "001") == 0)
        {
            
        }
        else if(strcmp(ins->rm, "010") == 0)
        {
            
        }
        else if(strcmp(ins->rm, "011") == 0)
        {
            
        }
        else if(strcmp(ins->rm, "100") == 0)
        {
            
        }
        else if(strcmp(ins->rm, "101") == 0)
        {
            
        }
        /*exception*/
        else if(strcmp(ins->rm, "110") == 0)
        {
            char front[8], rear[8];
            /*low disp*/
            decimal = (int)read_buffer[*buffer_ptr];
            *buffer_ptr ++;
            decimal2binary(decimal, binary);
            ins->length += 8;
            for (i = 0; i <= 7; i++)
            {
                front[i] = binary[i];
            }
            /*high disp*/
            decimal = (int)read_buffer[*buffer_ptr];
            *buffer_ptr ++;
            decimal2binary(decimal, binary);
            ins->length += 8;
            char front[8], rear[8];
            for (i = 0; i <= 7; i++)
            {
                rear[i] = binary[i];
            }

            /*read data*/
            if(ins->s == '0' && ins->w == '1')
            {

            }
            if(ins->s == '1' && ins->w == '1')
            {
                if(strcmp(ins->data1, '11111111') == 0)
                {
                    /*minus*/
                    char* completement, *front, *rear;
                    completement = binary2complement(ins->data0);
                    hexadecimal = convertBinaryToHexadecimal(completement);

                    ins->asem[offset + 1] = '0';
                    ins->asem[offset + 2] = '0';
                    ins->asem[offset + 3] = hexadecimal[0];
                    ins->asem[offset + 4] = hexadecimal[1];
                    ins->asem[offset + 5] = ']';
                    ins->asem[offset + 6] = ' ';

                    /*disp*/
                    /*maybe has some mistakes*/
                    decimal = (int)read_buffer[*buffer_ptr];
                    *buffer_ptr ++;
                    decimal2binary(decimal, binary);
                    ins->length += 8;
                    hexadecimal = convertBinaryToHexadecimal(rear);
                    ins->asem[offset + 7] = hexadecimal[0];
                    ins->asem[offset + 8] = hexadecimal[1];
                }
                else
                {
                    hexadecimal = convertBinaryToHexadecimal(ins->data0);
                }
            }
        }
        else if(strcmp(ins->rm, "111") == 0)
        {
            
        }
    }
    else if(strcmp(ins->mod, "01") == 0)
    {

    }
    else if(strcmp(ins->mod, "10") == 0)
    {

    }
    else if(strcmp(ins->mod, "11") == 0)
    {

    }
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
        asem_result->rear->next = node;
        asem_result->rear = node;
        asem_result->length ++;
    }
}

char* register_addressing_8bit(char reg[3])
{
    if(strcmp(reg, "000") == 0)
        return "AL";
    if(strcmp(reg, "001") == 0)
        return "CL";
    if(strcmp(reg, "010") == 0)
        return "DL";
    if(strcmp(reg, "011") == 0)
        return "BL";
    if(strcmp(reg, "100") == 0)
        return "AH";
    if(strcmp(reg, "101") == 0)
        return "CH";
    if(strcmp(reg, "110") == 0)
        return "DH";
    if(strcmp(reg, "111") == 0)
        return "BH";
}

char* register_addressing_16bit(char reg[3])
{
    if(strcmp(reg, "000") == 0)
        return "AX";
    if(strcmp(reg, "001") == 0)
        return "CX";
    if(strcmp(reg, "010") == 0)
        return "DX";
    if(strcmp(reg, "011") == 0)
        return "BX";
    if(strcmp(reg, "100") == 0)
        return "SP";
    if(strcmp(reg, "101") == 0)
        return "BP";
    if(strcmp(reg, "110") == 0)
        return "SI";
    if(strcmp(reg, "111") == 0)
        return "DI";
}

char* convertBinaryToHexadecimal(char* binary)
{
    char front[4], rear[4], result[2];
    int i;
    for (i = 0; i <= 3; i++)
    {
        front[i] = binary[i];
    }
    for(i = 0; i <= 3; i++)
    {
        rear[i] = binary[i + 4];
    }

    if(strcmp(front, "0000") == 0)
        result[0] = '0';
    if(strcmp(front, "0001") == 0)
        result[0] = '1';
    if(strcmp(front, "0010") == 0)
        result[0] = '2';
    if(strcmp(front, "0011") == 0)
        result[0] = '3';
    if(strcmp(front, "0100") == 0)
        result[0] = '4';
    if(strcmp(front, "0101") == 0)
        result[0] = '5';
    if(strcmp(front, "0110") == 0)
        result[0] = '6';
    if(strcmp(front, "0111") == 0)
        result[0] = '7';
    if(strcmp(front, "1000") == 0)
        result[0] = '8';
    if(strcmp(front, "1001") == 0)
        result[0] = '9';
    if(strcmp(front, "1010") == 0)
        result[0] = 'A';
    if(strcmp(front, "1011") == 0)
        result[0] = 'B';
    if(strcmp(front, "1100") == 0)
        result[0] = 'C';
    if(strcmp(front, "1101") == 0)
        result[0] = 'D';
    if(strcmp(front, "1110") == 0)
        result[0] = 'E';
    if(strcmp(front, "1111") == 0)
        result[0] = 'F';
    
    if(strcmp(rear, "0000") == 0)
        result[1] = '0';
    if(strcmp(rear, "0001") == 0)
        result[1] = '1';
    if(strcmp(rear, "0010") == 0)
        result[1] = '2';
    if(strcmp(rear, "0011") == 0)
        result[1] = '3';
    if(strcmp(rear, "0100") == 0)
        result[1] = '4';
    if(strcmp(rear, "0101") == 0)
        result[1] = '5';
    if(strcmp(rear, "0110") == 0)
        result[1] = '6';
    if(strcmp(rear, "0111") == 0)
        result[1] = '7';
    if(strcmp(rear, "1000") == 0)
        result[1] = '8';
    if(strcmp(rear, "1001") == 0)
        result[1] = '9';
    if(strcmp(rear, "1010") == 0)
        result[1] = 'A';
    if(strcmp(rear, "1011") == 0)
        result[1] = 'B';
    if(strcmp(rear, "1100") == 0)
        result[1] = 'C';
    if(strcmp(rear, "1101") == 0)
        result[1] = 'D';
    if(strcmp(rear, "1110") == 0)
        result[1] = 'E';
    if(strcmp(rear, "1111") == 0)
        result[1] = 'F';
    return result;
}

char* binary2complement(char binary[8])
{
    char complement[8];
    int i;
    for(i = 0; i <= 7; i++)
    {
        if(binary[i] == '1')
            complement[i] = 0;
        else
            complement[i] = '1';
    }
    for(i = 7; i >= 0; i--)
    {
        if(complement[i] == '0')
        {
            complement[i] = '1';
            break;
        }
        else
            complement[i] = '0';
    }
    return complement;
}

char* text_to_instruction(exec* hdr)
{
    char* binary_data;
    int i, x, text_end, decimal, and_cul;
    binary_data = malloc(8);
    text_end = buffer_ptr + (int)(hdr->a_text);
    /*loop until the end of text*/
    while(text_end - *buffer_ptr > 0)
    {
        /*new memory allocation for every instruction*/
        instruction *ins;
        ins = malloc(sizeof(instruction));

        /*read one byte*/
        decimal = (int)read_buffer[*buffer_ptr];
        *buffer_ptr ++;
        decimal2binary(decimal, binary_data);

        /*read binary text into instruction struct*/
        i = 0;
        while(i <= 3)
        {
            ins->seg_0[i] = binary_data[i];
            i ++;
        }

        if(strcmp(ins->seg_0, "1011") == 0)
            mov_I2R_interpreter(ins, binary_data);
        else if(strcmp(ins->seg_0, "1100") == 0)
        {
            for(i = 4; i <= 7; i++)
            {
                ins->seg_1[i] = binary_data[i];
            }
            if(ins->seg_1 == "1101")
                int_TS_interpreter(ins, binary_data);
        }
        else if(strcmp(ins->seg_0, "0000") == 0)
        {
            for(i = 4; i <= 7; i++)
            {
                ins->seg_1[i] = binary_data[i];
            }
            if(ins->seg_1[0] == '0' && ins->seg_1[1] == '0')
                add_RMR2E_interpreter(ins, binary_data);
        }
        else if(strcmp(ins->seg_0, "1000") == 0)
        {
            if(ins->seg_1[0] == '0' && ins->seg_1[1] == '0')
            {

            }
        }
    }
    
}

/*have to solve the little endian problem*/
void mov_I2R_interpreter(instruction* ins, char* binary_data)
{
    int i, decimal, n, k;
    instruction_node* node;
    char* reg, *hexadecimal;
    node = malloc(sizeof(instruction_node));
    for(i = 4; i <= 7; i++)
    {
        ins->seg_1[i] = binary_data[i];
    }
    ins->w = (int)ins->seg_1[0];
    for(i = 1; i <= 3; i++)
    {
        ins->reg[i - 1] = ins->seg_1[i];
    }

    /*the second byte*/
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    decimal2binary(decimal, binary_data);
    for(i = 0; i <= 7; i++)
    {
        ins->data0[i] = binary_data[i];
    }
    ins->length = 16;
    
    if(ins->w == '1')
    {
        /*the third byte*/
        decimal = (int)read_buffer[*buffer_ptr];
        *buffer_ptr ++;
        decimal2binary(decimal, binary_data);
        for(i = 0; i <= 7; i++)
        {
            ins->data0[i] = binary_data[i];
        }
        ins->length = 24;
    }

    /*set the asem text*/
    ins->asem[0] = 'M';
    ins->asem[1] = 'O';
    ins->asem[2] = 'V';
    ins->asem[3] = ' ';
    reg = register_addressing_16bit(ins->reg);
    for(i = 0; i <= 2; i++)
    {
        ins->asem[i + 4] = reg[i];
    }
    ins->asem[7] = ',';
    ins->asem[8] = ' ';
    if(ins->w = '1')
    {   
        /*little endian*/
        hexadecimal = convertBinaryToHexadecimal(ins->data1);
        printf("data: %s\n", hexadecimal);
        ins->asem[9] = hexadecimal[0];
        ins->asem[10] = hexadecimal[1];

        hexadecimal = convertBinaryToHexadecimal(ins->data0);
        printf("data: %s\n", hexadecimal);
        ins->asem[11] = hexadecimal[0];
        ins->asem[12] = hexadecimal[1];      
    }
    else
    {
        hexadecimal = convertBinaryToHexadecimal(ins->data0);
        printf("data: %s\n", hexadecimal);
        ins->asem[9] = hexadecimal[0];
        ins->asem[10] = hexadecimal[1]; 
    }

    node->ins = ins;
    list_add(node);
}

void int_TS_interpreter(instruction* ins, char* binary_data)
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
    printf("type: %s\n", hexadecimal);
    ins->asem[4] = hexadecimal[0];
    ins->asem[5] = hexadecimal[1];

    node->ins = ins;
    list_add(node);
}

void add_RMR2E_interpreter(instruction* ins, char* binary_data)
{
    int i, decimal;
    char *hexadecimal, *binary, *reg;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    ins->d = ins->seg_1[2];
    ins->w = ins->seg_1[3];

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

void sub_IRM_interpreter(instruction* ins, char* binary_data)
{
    int i, decimal;
    char *hexadecimal, *binary, *reg;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    ins->s = ins->seg_1[6];
    ins->w = ins->seg_1[7];

    /*the second byte*/
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

    /*the third byte*/
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    decimal2binary(decimal, binary_data);
    for(i = 0; i <= 7; i++)
    {
        ins->data0[i] = binary_data[i];
    }
    ins->length = 24;
    /*the forth byte*/
    if(ins->s == '0' && ins->w == '1')
    {
        decimal = (int)read_buffer[*buffer_ptr];
        *buffer_ptr ++;
        decimal2binary(decimal, binary_data);
        for(i = 0; i <= 7; i++)
        {
            ins->data0[i] = binary_data[i];
        }
        ins->length = 32;
    }

    ins->asem[0] = 'S';
    ins->asem[1] = 'U';
    ins->asem[2] = 'B';
    ins->asem[3] = ' ';
}