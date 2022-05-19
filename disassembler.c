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
    char mod[2];
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
instructions_list* asem_result;     /*save result*/
int *buffer_ptr; 

void decimal2binary(int decimal, char* binary_text);
void read_header(exec* hdr, char* openfile);
char* convertBinaryToHexadecimal(char binary[8]);
char* register_addressing_8bit(char* reg)
char* register_addressing_16bit(char* reg);
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
    switch(*reg)
    {
    case "000":
        return "AL"
        break;
    case "001":
        return "CL";
        break;
    case "010":
        return "DL";
        break;
    case "011":
        return "BL";
        break;
    case "100":
        return "AH";
        break;
    case "101":
        return "CH";
        break;
    case "110":
        return "DH";
        break;
    case "111":
        return "BH";
        break;
    default:
        printf("register addressing error\n");
        return "";
        break;
    }
}

char* register_addressing_16bit(char reg[3])
{
    switch (*reg)
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

char* convertBinaryToHexadecimal(char binary[8])
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

    switch (*front)
    {
    case "0000":
        result[0] = '0';
        break;
    case "0001":
        result[0] = '1';
        break;
    case "0010":
        result[0] = '2';
        break;
    case "0011":
        result[0] = '3';
        break;
    case "0100":
        result[0] = '4';
        break;
    case "0101":
        result[0] = '5';
        break;
    case "0110":
        result[0] = '6';
        break;
    case "0111":
        result[0] = '7';
        break;
    case "1000":
        result[0] = '8';
        break;
    case "1001":
        result[0] = '9';
        break;
    case "1010":
        result[0] = 'A';
        break;
    case "1011":
        result[0] = 'B';
        break;
    case "1100":
        result[0] = 'C';
        break;
    case "1101":
        result[0] = 'D';
        break;
    case "1110":
        result[0] = 'E';
        break;
    case "1111":
        result[0] = 'F';
        break;
    }

    switch (*rear)
    {
    case "0000":
        result[1] = '0';
        break;
    case "0001":
        result[1] = '1';
        break;
    case "0010":
        result[1] = '2';
        break;
    case "0011":
        result[1] = '3';
        break;
    case "0100":
        result[1] = '4';
        break;
    case "0101":
        result[1] = '5';
        break;
    case "0110":
        result[1] = '6';
        break;
    case "0111":
        result[1] = '7';
        break;
    case "1000":
        result[1] = '8';
        break;
    case "1001":
        result[1] = '9';
        break;
    case "1010":
        result[1] = 'A';
        break;
    case "1011":
        result[1] = 'B';
        break;
    case "1100":
        result[1] = 'C';
        break;
    case "1101":
        result[1] = 'D';
        break;
    case "1110":
        result[1] = 'E';
        break;
    case "1111":
        result[1] = 'F';
        break;
    }
    return result;
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
        decimal2binary(decimal, binary_data);

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
                else if(ins->seg_1 == "1100")

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
    int i, decimal, n, k;
    instruction_node* node;
    char reg[3], *hexadecimal;
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

    /*second byte*/
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
        /*third byte*/
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

void int_TS_interpreter(instruction *ins, char* binary_data)
{
    int i, decimal, hexadecimal;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    decimal2binary(decimal, binary_data)
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
    hexadecimal = binary2hexadecimal(ins->type);
    printf("type: %s\n", hexadecimal);
    ins->asem[4] = hexadecimal[0];
    ins->asem[5] = hexadecimal[1];

    node->ins = ins;
    list_add(node);
}

void add_RMR2E_interpreter(instruction *ins, char* binary_data)
{
    int i, decimal;
    char *hexadecimal;
    instruction_node* node;
    node = malloc(sizeof(instruction_node));
    ins->d = ins->seg_1[2];
    ins->w = ins->seg_1[3];

    /*second byte*/
    decimal = (int)read_buffer[*buffer_ptr];
    *buffer_ptr ++;
    decimal2binary(decimal, binary_data);

    for(i = 0; i <= 1; i++)
    {
        ins->mod[i] = binary_data[i];
    }
    for(i = 2; i <= 4; i++)
    {
        ins->reg[i - 3] = binary_data[i];
    }
    for(i = 5; i <= 7; i++)
    {
        ins->rm[i - 6] = binary_data[i];
    }
    ins->length = 16;

    /*set the asem*/
    ins->asem[0] = 'A';
    ins->asem[1] = 'D';
    ins->asem[2] = 'D';
    ins->asem[3] = ' ';
    if(ins->d = '1')
    {
        if(ins->w = '0')
        {
            hexadecimal = register_addressing_8bit(ins->reg);
        }
        else
        {
            hexadecimal = register_addressing_16bit(ins->reg);
        }
        ins->asem[4] = hexadecimal[0];
        ins->asem[5] = hexadecimal[1];
        ins->asem[6] = ',';
        ins->asem[7] = ' ';
        
    }
    else
    {
        switch (ins->mod)
        {
        case "00":
            hexadecimal = register_addressing_8bit(ins->rm);
            
            break;
        case "11":
            break;
        default:
            break;
        })
    }

    node->ins = ins;
    list_add(node);
}