#include <stdio.h>
#include <stdlib.h>

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
    char seg_1[4];
    char seg_2[4];
    char seg_3[8];
    /*function code*/
    char reg[3];
    char w;
    int mod[3];
    int rm[2];
    int data1[8];
    int data2[8];
    char* asem[16];
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
}   instructions_list;

unsigned char* read_buffer;         
unsigned char* instruction_cache;
instructions_list* asem_result;     /*save result*/
int *buffer_ptr;

void byte2binary(int decimal, char* binary_text);
char* text_to_instruction(exec* hdr);
void mov_i2r_interpreter(instruction *ins, char* binary_data);


int main(int argc, char* argv[])
{
    size_t file_size;
    exec* hd;
    FILE* fp;
    int i;
    unsigned char binary2long[4] = {0};
    fp = fopen(argv[1], "rb");
    read_buffer = malloc(16 * 1024);
    asem_result = malloc(sizeof(instructions_list));
    asem_result->length = 0;
    asem_result->front = NULL;
    instruction_cache = malloc(1024);
    hd = malloc(sizeof(exec));

    fread(&hd->a_magic[0], sizeof(unsigned char), 1, fp);
    fread(&hd->a_magic[1], sizeof(unsigned char), 1, fp);
    fread(&hd->a_flags, sizeof(unsigned char), 1, fp);
    fread(&hd->a_cpu, sizeof(unsigned char), 1, fp);
    fread(&(hd->a_hdrlen), sizeof(unsigned char), 1, fp);
    fread(&hd->a_unused, sizeof(unsigned char), 1, fp);
    fread(&hd->a_version, sizeof(short), 1, fp);

    /*read a.out and put into buffer*/
    fseek(fp, 0L, SEEK_END);
    file_size = (size_t)ftell(fp);
    rewind(fp);
    read(read_buffer, 1, file_size, fp);
    *buffer_ptr = 8;

    /*deal with little endian*/
    for(i = 0; i < 4; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        *buffer_ptr ++;
    }
    hd->a_text = (long)(binary2long[3] * 16 * 16 * 16 * 16 * 16 *16) +
        (binary2long[2] * 16 * 16 * 16 * 16) + (binary2long[1] * 16 * 16) + binary2long[0];

    for(i = 0; i < 4; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        *buffer_ptr ++;
    }
    hd->a_data = (long)(binary2long[3] * 16 * 16 * 16 * 16 * 16 *16) +
        (binary2long[2] * 16 * 16 * 16 * 16) + (binary2long[1] * 16 * 16) + binary2long[0];

    for(i = 0; i < 4; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        *buffer_ptr ++;
    }
    hd->a_bss = (long)(binary2long[3] * 16 * 16 * 16 * 16 * 16 *16) +
        (binary2long[2] * 16 * 16 * 16 * 16) + (binary2long[1] * 16 * 16) + binary2long[0];

    for(i = 0; i < 4; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        *buffer_ptr ++;
    }
    hd->a_entry = (long)(binary2long[3] * 16 * 16 * 16 * 16 * 16 *16) +
        (binary2long[2] * 16 * 16 * 16 * 16) + (binary2long[1] * 16 * 16) + binary2long[0];

    for(i = 0; i < 4; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        *buffer_ptr ++;
    }
    hd->a_total = (long)(binary2long[3] * 256 * 256 * 256) +
        (binary2long[2] * 256 * 256) + (binary2long[1] * 256) + binary2long[0];

    for(i = 0; i < 4; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        *buffer_ptr ++;
    }
    hd->a_syms = (long)(binary2long[3] * 16 * 16 * 16 * 16 * 16 *16) +
        (binary2long[2] * 16 * 16 * 16 * 16) + (binary2long[1] * 16 * 16) + binary2long[0];
    
    *buffer_ptr = (int)hd->a_hdrlen;
    printf("a_hdrlen:%hhu\n", hd->a_hdrlen);
    printf("buffer pointer position:%d\n", *buffer_ptr);
    printf("text length:%ld\n", hd->a_text);
    text_to_instruction(hd);
    free(read_buffer);
}

/*byte to binary*/
void byte2binary(int decimal, char* binary_text)
{
    int and_cul;
    and_cul = 255;
    for(int i = 0; i <= 7; i++)
    {
        if(and_cul & decimal == and_cul)
            binary_text[i] = 1;
        else
            binary_text[i] = 0;
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


char* text_to_instruction(exec* hdr)
{
    char* result, *binary_data;
    instruction *ins;
    int i, x, text_end, decimal, and_cul;
    ins = malloc(sizeof(instruction));
    result = malloc(16 * 1024);
    binary_data = malloc(8);
    text_end = buffer_ptr + (int)(hdr->a_text);
    /*loop until the end of text*/
    while(text_end - *buffer_ptr > 0)
    {
        /*read one byte*/
        decimal = (int)read_buffer[*buffer_ptr];
        *buffer_ptr ++;
        byte2binary(decimal, binary_data);

        /*read binary text into instruction struct*/
        i = 0;
        while(i <= 3)
        {
            ins->seg_1[i] = binary_data[i];
            i ++;
        }

        switch ((int)(ins->seg_1))
        {
            /*mov*/
            case 1011:
                mov_interpreter(ins, binary_data);
            break;
            /*int*/
            case 1100:

                break; 
            default:
            /*continue reading*/
        }
    }
    
}

void mov_i2r_interpreter(instruction *ins, char* binary_data)
{
    int i, decimal;
    instruction_node* node;
    node = malloc(sizeof(instruction_node)); 
    for(i = 4; i <= 8; i++)
    {
        ins->seg_2[i] = binary_data[i];
    }
    ins->w = (int)ins->seg_2[0];
    for(i = 1; i <= 3; i++)
    {
        ins->reg[i - 1] = ins->seg_2[i];
    }

    /*continue to read data*/
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    byte2binary(decimal, binary_data);
    for(i = 0; i <= 7; i++)
    {
        ins->data1[i] = binary_data[i];
    }
    ins->length = 16;
    
    if(ins->w == '1')
    {
        decimal = (int)read_buffer[*buffer_ptr];
        *buffer_ptr ++;
        byte2binary(decimal, binary_data);
        for(i = 0; i <= 7; i++)
        {
            ins->data2[i] = binary_data[i];
        }
        ins->length = 24;
    }

    node->ins = ins;
    list_add(node);
}

