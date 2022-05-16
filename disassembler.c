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
    char seg_0[4];
    char seg_1[4];
    /*function code*/
    char reg[3];
    char w;
    char d;
    char mod[3];
    char rm[2];
    char data0[8];
    char data1[8];
    char type[8];
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
char hex[16] = "0123456789ABCDEF"; 

void byte2binary(int decimal, char* binary_text);
void read_header(exec* hdr, char* openfile);
int convertBinaryToHexadecimal(int n);
char* register_addressing(char* reg);
char* text_to_instruction(exec* hdr);
void mov_I2R_interpreter(instruction *ins, char* binary_data);
void int_TS_interpreter(instruction *ins, char* binary_data);
void add_RMR2E_interpreter(instruction *ins, char* binary_data);


int main(int argc, char* argv[])
{
    exec* hd;
    FILE* fp;
    read_buffer = malloc(16 * 1024);
    asem_result = malloc(sizeof(instructions_list));
    asem_result->length = 0;
    asem_result->front = NULL;
    instruction_cache = malloc(1024);
    hd = malloc(sizeof(exec));

    read_header(hd, argv[1]);
    text_to_instruction(hd);
    free(read_buffer);
}

void read_header(exec* hdr, char* openfile)
{
    size_t file_size;
    unsigned char binary2long[4] = {0};
    fp = fopen(argv[1], "rb");
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

    /*deal with little endian*/
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
    
    *buffer_ptr = (int)hd->a_hdrlen;
    printf("a_hdrlen:%hhu\n", hd->a_hdrlen);
    printf("buffer pointer position:%d\n", *buffer_ptr);
    printf("text length:%ld\n", hd->a_text);
}

/*byte to binary*/
void byte2binary(int decimal, char* binary_data)
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

char* register_addressing(char* reg)
{
    switch (reg))
    {
    case "000":
        return "AX"
        break;
    case "001":
        return "CX";
        break;
    case "010":
        return "DX";
        break;
    case "011":
        return "BX";
        break;
    case "100":
        return "SP";
        break;
    case "101":
        return "BP";
        break;
    case "110":
        return "SI";
        break;
    case "111":
        return "DI";
        break;
    default:
        printf("register addressing error\n");
        return "";
        break;
    }
}

char* convertBinaryToHexadecimal(char* inStr)
{
    
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
            ins->seg_0[i] = binary_data[i];
            i ++;
        }

        switch ((int)(ins->seg_0))
        {
            /*mov*/
            case "1011":
                mov_interpreter(ins, binary_data);
            break;
            /*int*/
            case "1100": 
                for(i = 4; i <= 7; i++)
                {
                    ins->seg_1[i] = binary_data[i];
                }
                if(ins->seg_1 == "1101")
                    int_TS_interpreter(ins, binary_data);
                break; 
            case "0000":
                for(i = 4; i <= 7; i++)
                {
                    ins->seg_1[i] = binary_data[i];
                }
                if(ins->seg_1[0] == '0' && ins->seg_1[1] == '0')
                    add_RMR2E_interpreter(ins, binary_data);
            default:
            /*continue reading*/
        }
    }
    
}

/*have to solve the little endian problem*/
void mov_I2R_interpreter(instruction *ins, char* binary_data)
{
    int i, decimal, hexadecimal, binary, n, k;
    instruction_node* node;
    char* reg[3];
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

    /*continue to read data*/
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    byte2binary(decimal, binary_data);
    for(i = 0; i <= 7; i++)
    {
        ins->data0[i] = binary_data[i];
    }
    ins->length = 16;
    
    if(ins->w == '1')
    {
        /*solve little endian*/
        ins->data1 = ins->data0;

        decimal = (int)read_buffer[*buffer_ptr];
        *buffer_ptr ++;
        byte2binary(decimal, binary_data);
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
    reg = register_addressing(ins->reg);
    for(i = 0; i <= 2; i++)
    {
        ins->asem[i + 4] = reg[i];
    }
    ins->asem[7] = ',';
    ins->asem[8] = ' ';
    binary = atoi(ins->data0);
    hexadecimal = convertBinaryToHexadecimal(binary);
    printf("data: &d\n", hexadecimal);
    if(decimal < 16)
    {
        ins->asem[9] = 0;
        ins->asem[10] = hexadecimal;
    }
    else
    {
        ins->asem[9] = hexadecimal / 16;
        ins->asem[10] = hexadecimal % 16;
    }

    if(ins->w = '1')
    {
        binary = atoi(ins->data1);
        decimal = convertBinaryToHexadecimal(binary);
        printf("data: &d\n", decimal);
        if(decimal < 10)
        {
            ins->asem[11] = 0;
            ins->asem[12] = hexadecimal;
        }
        else
        {
            ins->asem[11] = hexadecimal / 16;
            ins->asem[12] = hexadecimal % 16;
        } 
    }

    node->ins = ins;
    list_add(node);
}

void int_TS_interpreter(instruction *ins, char* binary_data)
{
    int i, decimal;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    byte2binary(decimal, binary_data)
    for(i = 0; i <= 7; i++)
    {
        ins->type[i] = binary_data[i];
    }
    ins->length = 16;

    /*TODO: set the asem*/

    node->ins = ins;
    list_add(node);
}

void add_RMR2E_interpreter(instruction *ins, char* binary_data)
{
    int i, decimal;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    ins->d = ins->seg_1[2];
    ins->w = ins->seg_1[3];
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    byte2binary(decimal, binary_data);

    for(i = 0; i <= 2; i++)
    {
        ins->mod[i] = binary_data[i];
    }
    for(i = 3; i <= 5; i++)
    {
        ins->reg[i - 3] = binary_data[i];
    }
    for(i = 6; i <= 7; i++)
    {
        ins->rm[i - 6] = binary_data[i];
    }
    ins->length = 16;

    /*TODO: set the asem*/

    node->ins = ins;
    list_add(node);
}