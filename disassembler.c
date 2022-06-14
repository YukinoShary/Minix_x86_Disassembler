#include "function_define.h"
#define BUFFER_SIZE 32 * 1024

unsigned char* read_buffer;         
instructions_list* asem_result;     /*save result*/
int *buffer_ptr; 
char hexadecimal_table[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

/* when the flag == 1, it means interpreter mode*/
void (*instruction_func[256])(instruction*, char*, int*, int) =
{
    /*this is the line 1 */add_RMR2E_oper,
    /*this is the line 1 */add_RMR2E_oper,
    /*this is the line 2 */add_RMR2E_oper,
    /*this is the line 3 */NULL,
    /*this is the line 4 */NULL,
    /*this is the line 5 */NULL,
    /*this is the line 6 */NULL,
    /*this is the line 7 */NULL,
    /*this is the line 8 */NULL,
    /*this is the line 9 */NULL,
    /*this is the line a */NULL,
    /*this is the line b */NULL,
    /*this is the line c */NULL,
    /*this is the line d */NULL,
    /*this is the line e */NULL,
    /*this is the line f */NULL,
    /*this is the line 10 */NULL,
    /*this is the line 11 */NULL,
    /*this is the line 12 */NULL,
    /*this is the line 13 */NULL,
    /*this is the line 14 */NULL,
    /*this is the line 15 */NULL,
    /*this is the line 16 */NULL,
    /*this is the line 17 */NULL,
    /*this is the line 18 */NULL,
    /*this is the line 19 */NULL,
    /*this is the line 1a */NULL,
    /*this is the line 1b */NULL,
    /*this is the line 1c */NULL,
    /*this is the line 1d */NULL,
    /*this is the line 1e */NULL,
    /*this is the line 1f */NULL,
    /*this is the line 20 */NULL,
    /*this is the line 21 */NULL,
    /*this is the line 22 */NULL,
    /*this is the line 23 */NULL,
    /*this is the line 24 */NULL,
    /*this is the line 25 */NULL,
    /*this is the line 26 */NULL,
    /*this is the line 27 */NULL,
    /*this is the line 28 */NULL,
    /*this is the line 29 */NULL,
    /*this is the line 2a */NULL,
    /*this is the line 2b */NULL,
    /*this is the line 2c */NULL,
    /*this is the line 2d */NULL,
    /*this is the line 2e */NULL,
    /*this is the line 2f */NULL,
    /*this is the line 30 */xor_RMRE_oper,
    /*this is the line 31 */xor_RMRE_oper,
    /*this is the line 32 */xor_RMRE_oper,
    /*this is the line 33 */xor_RMRE_oper,
    /*this is the line 34 */NULL,
    /*this is the line 35 */NULL,
    /*this is the line 36 */NULL,
    /*this is the line 37 */NULL,
    /*this is the line 38 */NULL,
    /*this is the line 39 */NULL,
    /*this is the line 3a */NULL,
    /*this is the line 3b */NULL,
    /*this is the line 3c */NULL,
    /*this is the line 3d */NULL,
    /*this is the line 3e */NULL,
    /*this is the line 3f */NULL,
    /*this is the line 40 */NULL,
    /*this is the line 41 */NULL,
    /*this is the line 42 */NULL,
    /*this is the line 43 */NULL,
    /*this is the line 44 */NULL,
    /*this is the line 45 */NULL,
    /*this is the line 46 */NULL,
    /*this is the line 47 */NULL,
    /*this is the line 48 */NULL,
    /*this is the line 49 */NULL,
    /*this is the line 4a */NULL,
    /*this is the line 4b */NULL,
    /*this is the line 4c */NULL,
    /*this is the line 4d */NULL,
    /*this is the line 4e */NULL,
    /*this is the line 4f */NULL,
    /*this is the line 50 */push_R_oper,
    /*this is the line 51 */push_R_oper,
    /*this is the line 52 */push_R_oper,
    /*this is the line 53 */push_R_oper,
    /*this is the line 54 */push_R_oper,
    /*this is the line 55 */push_R_oper,
    /*this is the line 56 */push_R_oper,
    /*this is the line 57 */push_R_oper,
    /*this is the line 58 */NULL,
    /*this is the line 59 */NULL,
    /*this is the line 5a */NULL,
    /*this is the line 5b */NULL,
    /*this is the line 5c */NULL,
    /*this is the line 5d */NULL,
    /*this is the line 5e */NULL,
    /*this is the line 5f */NULL,
    /*this is the line 60 */NULL,
    /*this is the line 61 */NULL,
    /*this is the line 62 */NULL,
    /*this is the line 63 */NULL,
    /*this is the line 64 */NULL,
    /*this is the line 65 */NULL,
    /*this is the line 66 */NULL,
    /*this is the line 67 */NULL,
    /*this is the line 68 */NULL,
    /*this is the line 69 */NULL,
    /*this is the line 6a */NULL,
    /*this is the line 6b */NULL,
    /*this is the line 6c */NULL,
    /*this is the line 6d */NULL,
    /*this is the line 6e */NULL,
    /*this is the line 6f */NULL,
    /*this is the line 70 */NULL,
    /*this is the line 71 */NULL,
    /*this is the line 72 */NULL,
    /*this is the line 73 */jnb_JNBAE_oper,
    /*this is the line 74 */NULL,
    /*this is the line 75 */jne_oper,
    /*this is the line 76 */NULL,
    /*this is the line 77 */NULL,
    /*this is the line 78 */NULL,
    /*this is the line 79 */NULL,
    /*this is the line 7a */NULL,
    /*this is the line 7b */NULL,
    /*this is the line 7c */NULL,
    /*this is the line 7d */NULL,
    /*this is the line 7e */NULL,
    /*this is the line 7f */NULL,
    /*this is the line 80 */IRM_2_oper,
    /*this is the line 81 */IRM_2_oper,
    /*this is the line 82 */IRM_2_oper,
    /*this is the line 83 */IRM_2_oper,
    /*this is the line 84 */NULL,
    /*this is the line 85 */NULL,
    /*this is the line 86 */NULL,
    /*this is the line 87 */NULL,
    /*this is the line 88 */mov_RMR_oper,
    /*this is the line 89 */mov_RMR_oper,
    /*this is the line 8a */mov_RMR_oper,
    /*this is the line 8b */mov_RMR_oper,
    /*this is the line 8c */lea_LEAR_oper,
    /*this is the line 8d */lea_LEAR_oper,
    /*this is the line 8e */lea_LEAR_oper,
    /*this is the line 8f */lea_LEAR_oper,
    /*this is the line 90 */NULL,
    /*this is the line 91 */NULL,
    /*this is the line 92 */NULL,
    /*this is the line 93 */NULL,
    /*this is the line 94 */NULL,
    /*this is the line 95 */NULL,
    /*this is the line 96 */NULL,
    /*this is the line 97 */NULL,
    /*this is the line 98 */cbw_oper,
    /*this is the line 99 */NULL,
    /*this is the line 9a */NULL,
    /*this is the line 9b */NULL,
    /*this is the line 9c */NULL,
    /*this is the line 9d */NULL,
    /*this is the line 9e */NULL,
    /*this is the line 9f */NULL,
    /*this is the line a0 */NULL,
    /*this is the line a1 */NULL,
    /*this is the line a2 */NULL,
    /*this is the line a3 */NULL,
    /*this is the line a4 */NULL,
    /*this is the line a5 */NULL,
    /*this is the line a6 */NULL,
    /*this is the line a7 */NULL,
    /*this is the line a8 */NULL,
    /*this is the line a9 */NULL,
    /*this is the line aa */NULL,
    /*this is the line ab */NULL,
    /*this is the line ac */NULL,
    /*this is the line ad */NULL,
    /*this is the line ae */NULL,
    /*this is the line af */NULL,
    /*this is the line b0 */mov_I2R_oper,
    /*this is the line b1 */mov_I2R_oper,
    /*this is the line b2 */mov_I2R_oper,
    /*this is the line b3 */mov_I2R_oper,
    /*this is the line b4 */mov_I2R_oper,
    /*this is the line b5 */mov_I2R_oper,
    /*this is the line b6 */mov_I2R_oper,
    /*this is the line b7 */mov_I2R_oper,
    /*this is the line b8 */mov_I2R_oper,
    /*this is the line b9 */mov_I2R_oper,
    /*this is the line ba */mov_I2R_oper,
    /*this is the line bb */mov_I2R_oper,
    /*this is the line bc */mov_I2R_oper,
    /*this is the line bd */mov_I2R_oper,
    /*this is the line be */mov_I2R_oper,
    /*this is the line bf */mov_I2R_oper,
    /*this is the line c0 */NULL,
    /*this is the line c1 */NULL,
    /*this is the line c2 */NULL,
    /*this is the line c3 */NULL,
    /*this is the line c4 */NULL,
    /*this is the line c5 */NULL,
    /*this is the line c6 */NULL,
    /*this is the line c7 */NULL,
    /*this is the line c8 */NULL,
    /*this is the line c9 */NULL,
    /*this is the line ca */NULL,
    /*this is the line cb */NULL,
    /*this is the line cc */NULL,
    /*this is the line cd */int_TS_oper,
    /*this is the line ce */NULL,
    /*this is the line cf */NULL,
    /*this is the line d0 */NULL,
    /*this is the line d1 */NULL,
    /*this is the line d2 */NULL,
    /*this is the line d3 */NULL,
    /*this is the line d4 */NULL,
    /*this is the line d5 */NULL,
    /*this is the line d6 */NULL,
    /*this is the line d7 */NULL,
    /*this is the line d8 */NULL,
    /*this is the line d9 */NULL,
    /*this is the line da */NULL,
    /*this is the line db */NULL,
    /*this is the line dc */NULL,
    /*this is the line dd */NULL,
    /*this is the line de */NULL,
    /*this is the line df */NULL,
    /*this is the line e0 */NULL,
    /*this is the line e1 */NULL,
    /*this is the line e2 */NULL,
    /*this is the line e3 */NULL,
    /*this is the line e4 */NULL,
    /*this is the line e5 */NULL,
    /*this is the line e6 */NULL,
    /*this is the line e7 */NULL,
    /*this is the line e8 */call_DS_oper,
    /*this is the line e9 */jmp_DS_oper,
    /*this is the line ea */NULL,
    /*this is the line eb */NULL,
    /*this is the line ec */NULL,
    /*this is the line ed */NULL,
    /*this is the line ee */NULL,
    /*this is the line ef */NULL,
    /*this is the line f0 */NULL,
    /*this is the line f1 */NULL,
    /*this is the line f2 */NULL,
    /*this is the line f3 */NULL,
    /*this is the line f4 */hlt_oper,
    /*this is the line f5 */NULL,
    /*this is the line f6 */IDRM_4_oper,
    /*this is the line f7 */IDRM_4_oper,
    /*this is the line f8 */NULL,
    /*this is the line f9 */NULL,
    /*this is the line fa */NULL,
    /*this is the line fb */NULL,
    /*this is the line fc */NULL,
    /*this is the line fd */NULL,
    /*this is the line fe */NULL,
    /*this is the line ff */NULL
};

int main(int argc, char* argv[])
{
    exec* hd;
    read_buffer = malloc(BUFFER_SIZE);
    asem_result = malloc(sizeof(instructions_list));
    asem_result->length = 0;
    asem_result->front = NULL;
    buffer_ptr = malloc(sizeof(int));
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
    long binary2long[4];
    fp = fopen(openfile, "rb");
    fread(&hdr->a_magic[0], sizeof(unsigned char), 1, fp);
    fread(&hdr->a_magic[1], sizeof(unsigned char), 1, fp);
    fread(&hdr->a_flags, sizeof(unsigned char), 1, fp);
    fread(&hdr->a_cpu, sizeof(unsigned char), 1, fp);
    fread(&(hdr->a_hdrlen), sizeof(unsigned char), 1, fp);
    fread(&hdr->a_unused, sizeof(unsigned char), 1, fp);
    fread(&hdr->a_version, sizeof(short), 2, fp);

    /*read a.out and put into buffer*/
    fseek(fp, 0L, SEEK_END);
    file_size = (size_t)ftell(fp);
    rewind(fp);
    printf("size:%lu\n", file_size);
    if(file_size < BUFFER_SIZE)
        fread(read_buffer, 1, file_size, fp);
    else
        printf("file size is larger than the buffer size\n");

    /*here is the the posision that a new byte should be written here which means [0]~[7] has been used*/
    *buffer_ptr = 8; 

    /*deal with big endian*/
    for(i = 0; i <= 3; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        (*buffer_ptr) ++;
    }
    hdr->a_text = (binary2long[3] << 24 ) +
        (binary2long[2] << 16) + (binary2long[1] << 8) + binary2long[0];

    for(i = 0; i <= 3; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        (*buffer_ptr) ++;
    }
    hdr->a_data = (binary2long[3] << 24 ) +
        (binary2long[2] << 16) + (binary2long[1] << 8) + binary2long[0];

    for(i = 0; i <= 3; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        (*buffer_ptr) ++;
    }
    hdr->a_bss = (binary2long[3] << 24 ) +
        (binary2long[2] << 16) + (binary2long[1] << 8) + binary2long[0];

    for(i = 0; i <= 3; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        (*buffer_ptr) ++;
    }
    hdr->a_entry = (binary2long[3] << 24 ) +
        (binary2long[2] << 16) + (binary2long[1] << 8) + binary2long[0];

    for(i = 0; i <= 3; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        (*buffer_ptr) ++;
    }
    hdr->a_total = (binary2long[3] << 24 ) +
        (binary2long[2] << 16) + (binary2long[1] << 8) + binary2long[0];

    for(i = 0; i <= 3; i++)
    {
        binary2long[i] = read_buffer[*buffer_ptr];
        (*buffer_ptr) ++;
    }
    hdr->a_syms = (binary2long[3] << 24 ) +
        (binary2long[2] << 16) + (binary2long[1] << 8) + binary2long[0];

    /**buffer_ptr = (int)hdr->a_hdrlen;*/
    printf("a_hrdlen:%hhu\n", hdr->a_hdrlen);
    printf("buffer pointer position:%d\n", *buffer_ptr);
    printf("text length:%ld\n", hdr->a_text);
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
    if(ins->d == 1)
    {
        if(ins->w == 0)
        {
            register_addressing_8bit(ins->reg, reg);
        }
        else
        {
            register_addressing_16bit(ins->reg, reg);
        }

        /*reg*/
        strcpy(ins->asem[offset], reg);
        offset += 2;
        strcpy(ins->asem[offset], ", ");
        offset += 2;
        
        /*r/m & disp*/
        if(ins->mod == 0x00)
        {
            if(ins->rm == 0x00)
            {
                strcpy(ins->asem[offset], "BX+SI");
                offset += 5;
            }
            else if(ins->rm == 0x01)
            {
                strcpy(ins->asem[offset], "BX+DI");
                offset += 5;
            }
            else if(ins->rm == 0x02)
            {
                strcpy(ins->asem[offset], "BP+SI");
                offset += 5;
            }
            else if(ins->rm == 0x03)
            {
                strcpy(ins->asem[offset], "BP+DI");
                offset += 5;
            }
            else if(ins->rm == 0x04)
            {
                strcpy(ins->asem[offset], "SI");
                offset += 2;
            }
            else if(ins->rm == 0x05)
            {
                strcpy(ins->asem[offset], "DI");
                offset += 2;
            }
            /*here is an exception*/
            else if(ins->rm == 0x06)
            {
                offset = read_disp(ins, offset, 1);
            }
            else if(ins->rm == 0x07)
            {
                strcpy(ins->asem[offset], "BX");
                offset += 2;
            }
            
        }
        
        else if(ins->mod == 0x01)
        {
            if(ins->rm = 0x00)
            {
                strcpy(ins->asem[offset], "[BX+SI");
                offset += 6;
                offset = read_disp(ins, offset, 0);
                ins->asem[offset] = ']';
                ins->asem[offset + 1] = '\0';
                offset += 2;
            }
            else if(ins->rm = 0x01)
            {
                strcpy(ins->asem[offset], "[BX+DI");
                offset += 6;
                offset = read_disp(ins, offset, 0);
                ins->asem[offset] = ']';
                ins->asem[offset + 1] = '\0';
                offset += 2;
            }
            else if(ins->rm = 0x02)
            {
                strcpy(ins->asem[offset], "[BP+SI");
                offset += 6;
                offset = read_disp(ins, offset, 0);
                ins->asem[offset] = ']';
                ins->asem[offset + 1] = '\0';
                offset += 2;
            }
            else if(ins->rm = 0x03)
            {
                strcpy(ins->asem[offset], "[BP+DI");
                offset += 6;
                offset = read_disp(ins, offset, 0);
                ins->asem[offset] = ']';
                ins->asem[offset + 1] = '\0';
                offset += 2;
            }
            else if(ins->rm = 0x04)
            {
                strcpy(ins->asem[offset], "[SI");
                offset += 3;
                offset = read_disp(ins, offset, 0);
                ins->asem[offset] = ']';
                ins->asem[offset + 1] = '\0';
                offset += 2;
            }
            else if(ins->rm = 0x05)
            {
                strcpy(ins->asem[offset], "[DI");
                offset += 3;
                offset = read_disp(ins, offset, 0);
                ins->asem[offset] = ']';
                ins->asem[offset + 1] = '\0';
                offset += 2;
            }
            else if(ins->rm = 0x06)
            {
                strcpy(ins->asem[offset], "[BP");
                offset += 3;
                offset = read_disp(ins, offset, 0);
                ins->asem[offset] = ']';
                ins->asem[offset + 1] = '\0';
                offset += 2;
            }
            else if(ins->rm = 0x07)
            {
                strcpy(ins->asem[offset], "[BX");
                offset += 3;
                offset = read_disp(ins, offset, 0);
                ins->asem[offset] = ']';
                ins->asem[offset + 1] = '\0';
                offset += 2;
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
                hexadecimal = decimalToHexadecimal(rear);
                ins->asem[offset + 10] = hexadecimal[0];
                ins->asem[offset + 11] = hexadecimal[1];
                hexadecimal = decimalToHexadecimal(front);
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
                hexadecimal = decimalToHexadecimal(rear);
                ins->asem[offset + 10] = hexadecimal[0];
                ins->asem[offset + 11] = hexadecimal[1];
                hexadecimal = decimalToHexadecimal(front);
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
                hexadecimal = decimalToHexadecimal(rear);
                ins->asem[offset + 10] = hexadecimal[0];
                ins->asem[offset + 11] = hexadecimal[1];
                hexadecimal = decimalToHexadecimal(front);
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
                hexadecimal = decimalToHexadecimal(rear);
                ins->asem[offset + 10] = hexadecimal[0];
                ins->asem[offset + 11] = hexadecimal[1];
                hexadecimal = decimalToHexadecimal(front);
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
                hexadecimal = decimalToHexadecimal(rear);
                ins->asem[offset + 8] = hexadecimal[0];
                ins->asem[offset + 9] = hexadecimal[1];
                hexadecimal = decimalToHexadecimal(front);
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
                hexadecimal = decimalToHexadecimal(rear);
                ins->asem[offset + 8] = hexadecimal[0];
                ins->asem[offset + 9] = hexadecimal[1];
                hexadecimal = decimalToHexadecimal(front);
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
                hexadecimal = decimalToHexadecimal(rear);
                ins->asem[offset + 8] = hexadecimal[0];
                ins->asem[offset + 9] = hexadecimal[1];
                hexadecimal = decimalToHexadecimal(front);
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
                hexadecimal = decimalToHexadecimal(rear);
                ins->asem[offset + 8] = hexadecimal[0];
                ins->asem[offset + 9] = hexadecimal[1];
                hexadecimal = decimalToHexadecimal(front);
                ins->asem[offset + 10] = hexadecimal[0];
                ins->asem[offset + 11] = hexadecimal[1];
                ins->asem[offset + 12] = ']';
                ins->asem[offset + 13] = '\0';
            }
            free(front);
            free(rear);
        }
        else if(strcmp(ins->mod, "11") == 0)
        {
            if(ins->w == 1)
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
        if(ins->w = 0)
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
                hexadecimal = decimalToHexadecimal(rear);
                ins->asem[offset + 1] = hexadecimal[0];
                ins->asem[offset + 2] = hexadecimal[1];
                hexadecimal = decimalToHexadecimal(front);
                ins->asem[offset + 3] = hexadecimal[0];
                ins->asem[offset + 4] = hexadecimal[1];
                ins->asem[offset + 5] = ']';
                ins->asem[offset + 6] = ',';
                ins->asem[offset + 7] = ' ';
                ins->asem[offset + 8] = reg[0];
                ins->asem[offset + 9] = reg[1];
                ins->asem[offset + 10] = '\0';
                free(front);
                free(rear);
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
            char front[8];
            /*low disp*/
            decimal = (int)read_buffer[*buffer_ptr];
            *buffer_ptr ++;
            decimal2binary(decimal, binary);
            ins->length += 8;
            for (i = 0; i <= 7; i++)
            {
                front[i] = binary[i];
            }
            /*minus*/
            if(front[0] == '1')
            {
                char* completement;
                completement = byte_complement(front);
                hexadecimal = decimalToHexadecimal(completement);
            }
            /*plus*/
            else
            {
                hexadecimal = decimalToHexadecimal(front);    
            }

            if(strcmp(ins->rm, "000") == 0)
            {
                ins->asem[offset + 1] = 'B';
                ins->asem[offset + 2] = 'X';
                ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = 'S';
                ins->asem[offset + 5] = 'I';
                if(front[0] == '1')
                    ins->asem[offset + 6] = '-';
                else
                    ins->asem[offset + 6] = '+';
                ins->asem[offset + 7] = '0';
                ins->asem[offset + 8] = '0';
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
                if(front[0] == '1')
                    ins->asem[offset + 6] = '-';
                else
                    ins->asem[offset + 6] = '+';
                ins->asem[offset + 7] = '0';
                ins->asem[offset + 8] = '0';
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
                if(front[0] == '1')
                    ins->asem[offset + 6] = '-';
                else
                    ins->asem[offset + 6] = '+';
                ins->asem[offset + 7] = '0';
                ins->asem[offset + 8] = '0';
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
                if(front[0] == '1')
                    ins->asem[offset + 6] = '-';
                else
                    ins->asem[offset + 6] = '+';
                ins->asem[offset + 7] = '0';
                ins->asem[offset + 8] = '0';
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
                if(front[0] == '1')
                    ins->asem[offset + 3] = '-';
                else
                    ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = '0';
                ins->asem[offset + 5] = '0';
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
                if(front[0] == '1')
                    ins->asem[offset + 3] = '-';
                else
                    ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = '0';
                ins->asem[offset + 5] = '0';
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
                if(front[0] == '1')
                    ins->asem[offset + 3] = '-';
                else
                    ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = '0';
                ins->asem[offset + 5] = '0';
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
                if(front[0] == '1')
                    ins->asem[offset + 3] = '-';
                else
                    ins->asem[offset + 3] = '+';
                ins->asem[offset + 4] = '0';
                ins->asem[offset + 5] = '0';
                ins->asem[offset + 6] = hexadecimal[0];
                ins->asem[offset + 7] = hexadecimal[1];
                ins->asem[offset + 8] = ']';
                ins->asem[offset + 9] = ',';
                ins->asem[offset + 10] = ' ';
                ins->asem[offset + 11] = reg[0];
                ins->asem[offset + 12] = reg[1];
                ins->asem[offset + 13] = '\0';
            }
            free(front);
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
                hexadecimal = decimalToHexadecimal(rear);
                ins->asem[offset + 7] = hexadecimal[0];
                ins->asem[offset + 8] = hexadecimal[1];
                hexadecimal = decimalToHexadecimal(front);
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
                hexadecimal = decimalToHexadecimal(rear);
                ins->asem[offset + 7] = hexadecimal[0];
                ins->asem[offset + 8] = hexadecimal[1];
                hexadecimal = decimalToHexadecimal(front);
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
                hexadecimal = decimalToHexadecimal(rear);
                ins->asem[offset + 7] = hexadecimal[0];
                ins->asem[offset + 8] = hexadecimal[1];
                hexadecimal = decimalToHexadecimal(front);
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
                hexadecimal = decimalToHexadecimal(rear);
                ins->asem[offset + 7] = hexadecimal[0];
                ins->asem[offset + 8] = hexadecimal[1];
                hexadecimal = decimalToHexadecimal(front);
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
                hexadecimal = decimalToHexadecimal(rear);
                ins->asem[offset + 4] = hexadecimal[0];
                ins->asem[offset + 5] = hexadecimal[1];
                hexadecimal = decimalToHexadecimal(front);
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
                hexadecimal = decimalToHexadecimal(rear);
                ins->asem[offset + 4] = hexadecimal[0];
                ins->asem[offset + 5] = hexadecimal[1];
                hexadecimal = decimalToHexadecimal(front);
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
                hexadecimal = decimalToHexadecimal(rear);
                ins->asem[offset + 4] = hexadecimal[0];
                ins->asem[offset + 5] = hexadecimal[1];
                hexadecimal = decimalToHexadecimal(front);
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
                hexadecimal = decimalToHexadecimal(rear);
                ins->asem[offset + 4] = hexadecimal[0];
                ins->asem[offset + 5] = hexadecimal[1];
                hexadecimal = decimalToHexadecimal(front);
                ins->asem[offset + 6] = hexadecimal[0];
                ins->asem[offset + 7] = hexadecimal[1];
                ins->asem[offset + 8] = ']';
                ins->asem[offset + 9] = ',';
                ins->asem[offset + 10] = ' ';
                ins->asem[offset + 11] = reg[0];
                ins->asem[offset + 12] = reg[1];
                ins->asem[offset + 13] = '\0';
            }
            free(front);
            free(rear);
        }
        else if(strcmp(ins->mod, "11") == 0)
        {
            if(ins->w == 1)
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

/*when flag == 1, read data*/
void MOD_RM_process(instruction* ins, int offset, int flag)
{
    int i, decimal;
    char *hexadecimal, *reg;
    if(strcmp(ins->mod, "00") == 0)
    {
        if(strcmp(ins->rm, "000") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'B';
            ins->asem[offset + 2] = 'X';
            ins->asem[offset + 3] = '+';
            ins->asem[offset + 4] = 'S';
            ins->asem[offset + 5] = 'I';
            ins->asem[offset + 6] = ']';
            ins->asem[offset + 7] = ',';
            ins->asem[offset + 8] = ' ';
            offset += 9;

            if(ins->s == 1 && ins->w == 1 && flag == 1)
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
        else if(strcmp(ins->rm, "001") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'B';
            ins->asem[offset + 2] = 'X';
            ins->asem[offset + 3] = '+';
            ins->asem[offset + 4] = 'D';
            ins->asem[offset + 5] = 'I';
            ins->asem[offset + 6] = ']';
            ins->asem[offset + 7] = ',';
            ins->asem[offset + 8] = ' ';
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
        else if(strcmp(ins->rm, "010") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'B';
            ins->asem[offset + 2] = 'P';
            ins->asem[offset + 3] = '+';
            ins->asem[offset + 4] = 'S';
            ins->asem[offset + 5] = 'I';
            ins->asem[offset + 6] = ']';
            ins->asem[offset + 7] = ',';
            ins->asem[offset + 8] = ' ';
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
        else if(strcmp(ins->rm, "011") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'B';
            ins->asem[offset + 2] = 'P';
            ins->asem[offset + 3] = '+';
            ins->asem[offset + 4] = 'D';
            ins->asem[offset + 5] = 'I';
            ins->asem[offset + 6] = ']';
            ins->asem[offset + 7] = ',';
            ins->asem[offset + 8] = ' ';
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
        else if(strcmp(ins->rm, "100") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'S';
            ins->asem[offset + 2] = 'I';
            ins->asem[offset + 3] = ']';
            ins->asem[offset + 4] = ',';
            ins->asem[offset + 5] = ' ';
            offset += 6;

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
        else if(strcmp(ins->rm, "101") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'D';
            ins->asem[offset + 2] = 'I';
            ins->asem[offset + 3] = ']';
            ins->asem[offset + 4] = ',';
            ins->asem[offset + 5] = ' ';
            offset += 6;

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
        else if(strcmp(ins->rm, "110") == 0)
        {          
            /*disp*/
            offset = read_disp(ins, offset, 1);
            offset ++;

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
        else if(strcmp(ins->rm, "111") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'B';
            ins->asem[offset + 2] = 'X';
            ins->asem[offset + 3] = ']';
            ins->asem[offset + 4] = ',';
            ins->asem[offset + 5] = ' ';
            offset += 6;

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
    else if(strcmp(ins->mod, "01") == 0)
    {
        /*TODO*/
        if(strcmp(ins->rm, "000") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'B';
            ins->asem[offset + 2] = 'X';
            ins->asem[offset + 3] = '+';
            ins->asem[offset + 4] = 'S';
            ins->asem[offset + 5] = 'I';
            ins->asem[offset + 6] = '+';
            offset += 7;
            offset = read_disp(ins, offset, 0);
            ins->asem[offset] = ']';
            ins->asem[offset + 1] = ',';
            ins->asem[offset + 2] = ' ';
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
        if(strcmp(ins->rm, "001") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'B';
            ins->asem[offset + 2] = 'X';
            ins->asem[offset + 3] = '+';
            ins->asem[offset + 4] = 'S';
            ins->asem[offset + 5] = 'I';
            ins->asem[offset + 6] = '+';
            offset += 7;
            offset = read_disp(ins, offset, 0);
            ins->asem[offset] = ']';
            ins->asem[offset + 1] = ',';
            ins->asem[offset + 2] = ' ';
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
        if(strcmp(ins->rm, "010") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'B';
            ins->asem[offset + 2] = 'X';
            ins->asem[offset + 3] = '+';
            ins->asem[offset + 4] = 'S';
            ins->asem[offset + 5] = 'I';
            ins->asem[offset + 6] = '+';
            offset += 7;
            offset = read_disp(ins, offset, 0);
            ins->asem[offset] = ']';
            ins->asem[offset + 1] = ',';
            ins->asem[offset + 2] = ' ';
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
        if(strcmp(ins->rm, "011") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'B';
            ins->asem[offset + 2] = 'X';
            ins->asem[offset + 3] = '+';
            ins->asem[offset + 4] = 'S';
            ins->asem[offset + 5] = 'I';
            ins->asem[offset + 6] = '+';
            offset += 7;
            offset = read_disp(ins, offset, 0);
            ins->asem[offset] = ']';
            ins->asem[offset + 1] = ',';
            ins->asem[offset + 2] = ' ';
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
        if(strcmp(ins->rm, "100") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'S';
            ins->asem[offset + 2] = 'I';
            ins->asem[offset + 3] = '+';
            offset += 4;
            offset = read_disp(ins, offset, 0);
            ins->asem[offset] = ']';
            ins->asem[offset + 1] = ',';
            ins->asem[offset + 2] = ' ';
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
        if(strcmp(ins->rm, "101") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'D';
            ins->asem[offset + 2] = 'I';
            ins->asem[offset + 3] = '+';
            offset += 4;
            offset = read_disp(ins, offset, 0);
            ins->asem[offset] = ']';
            ins->asem[offset + 1] = ',';
            ins->asem[offset + 2] = ' ';
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
        if(strcmp(ins->rm, "110") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'B';
            ins->asem[offset + 2] = 'P';
            ins->asem[offset + 3] = '+';
            offset += 4;
            offset = read_disp(ins, offset, 0);
            ins->asem[offset] = ']';
            ins->asem[offset + 1] = ',';
            ins->asem[offset + 2] = ' ';
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
        if(strcmp(ins->rm, "111") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'B';
            ins->asem[offset + 2] = 'X';
            ins->asem[offset + 3] = '+';
            offset += 4;
            offset = read_disp(ins, offset, 0);
            ins->asem[offset] = ']';
            ins->asem[offset + 1] = ',';
            ins->asem[offset + 2] = ' ';
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
    else if(strcmp(ins->mod, "10") == 0)
    {
        if(strcmp(ins->rm, "000") == 0)
        {   
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'B';
            ins->asem[offset + 2] = 'X';
            ins->asem[offset + 3] = '+';
            ins->asem[offset + 4] = 'S';
            ins->asem[offset + 5] = 'I';
            ins->asem[offset + 6] = '+';
            offset += 7;

            offset = read_disp(ins, offset, 1);
 
            ins->asem[offset] = ']';
            ins->asem[offset + 1] = ',';
            ins->asem[offset + 2] = ' ';
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
        else if(strcmp(ins->rm, "001") == 0)
        {         
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'B';
            ins->asem[offset + 2] = 'X';
            ins->asem[offset + 3] = '+';
            ins->asem[offset + 4] = 'D';
            ins->asem[offset + 5] = 'I';
            ins->asem[offset + 6] = '+';
            offset += 7;

            offset = read_disp(ins, offset, 1);
 
            ins->asem[offset] = ']';
            ins->asem[offset + 1] = ',';
            ins->asem[offset + 2] = ' ';
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
        else if(strcmp(ins->rm, "010") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'B';
            ins->asem[offset + 2] = 'P';
            ins->asem[offset + 3] = '+';
            ins->asem[offset + 4] = 'S';
            ins->asem[offset + 5] = 'I';
            ins->asem[offset + 6] = '+';
            offset += 7;

            offset = read_disp(ins, offset, 1);
 
            ins->asem[offset] = ']';
            ins->asem[offset + 1] = ',';
            ins->asem[offset + 2] = ' ';
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
        else if(strcmp(ins->rm, "011") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'B';
            ins->asem[offset + 2] = 'P';
            ins->asem[offset + 3] = '+';
            ins->asem[offset + 4] = 'D';
            ins->asem[offset + 5] = 'I';
            ins->asem[offset + 6] = '+';
            offset += 7;

            offset = read_disp(ins, offset, 1);
 
            ins->asem[offset] = ']';
            ins->asem[offset + 1] = ',';
            ins->asem[offset + 2] = ' ';
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
        else if(strcmp(ins->rm, "100") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'S';
            ins->asem[offset + 2] = 'I';
            ins->asem[offset + 3] = '+';
            offset += 4;

            offset = read_disp(ins, offset, 1);
 
            ins->asem[offset] = ']';
            ins->asem[offset + 1] = ',';
            ins->asem[offset + 2] = ' ';
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
        else if(strcmp(ins->rm, "101") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'D';
            ins->asem[offset + 2] = 'I';
            ins->asem[offset + 3] = '+';
            offset += 4;

            offset = read_disp(ins, offset, 1);
 
            ins->asem[offset] = ']';
            ins->asem[offset + 1] = ',';
            ins->asem[offset + 2] = ' ';
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
        else if(strcmp(ins->rm, "110") == 0)
        {
            ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'S';
            ins->asem[offset + 2] = 'I';
            ins->asem[offset + 3] = '+';
            offset += 4;

            offset = read_disp(ins, offset, 1);
 
            ins->asem[offset] = ']';
            ins->asem[offset + 1] = ',';
            ins->asem[offset + 2] = ' ';
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
        else if(strcmp(ins->rm, "111") == 0)
        {
           ins->asem[offset] = '[';
            ins->asem[offset + 1] = 'S';
            ins->asem[offset + 2] = 'I';
            ins->asem[offset + 3] = '+';
            offset += 4;

            offset = read_disp(ins, offset, 1);
 
            ins->asem[offset] = ']';
            ins->asem[offset + 1] = ',';
            ins->asem[offset + 2] = ' ';
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
    else if(strcmp(ins->mod, "11") == 0)
    {
        if(ins->w == '1')
            reg = register_addressing_16bit(ins->rm);
        else
            reg = register_addressing_8bit(ins->rm);
        ins->asem[offset] = reg[0];
        ins->asem[offset + 1] = reg[1];
        ins->asem[offset + 2] = ',';
        ins->asem[offset + 3] = ' ';
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

/* when flag == 1, read 2 byte*/
int read_disp(instruction* ins, int offset, int flag)
{
    char *hexadecimal, *complement;
    int i;
    complement = malloc(8);
    if(flag)
    {
        /*2 byte*/
        ins->low_disp = (int)read_buffer[*buffer_ptr] - '0';
        *buffer_ptr ++;
        ins->length += 8;
        ins->high_disp = (int)read_buffer[*buffer_ptr] - '0';
        *buffer_ptr ++;
        ins->length += 8;

        decimalToHexadecimal(ins->high_disp, hexadecimal);
        ins->asem[offset] = hexadecimal[0];
        ins->asem[offset + 1] = hexadecimal[1];
        decimalToHexadecimal(ins->low_disp, hexadecimal);
        ins->asem[offset + 2] = hexadecimal[0];
        ins->asem[offset + 3] = hexadecimal[1];
        offset += 4;
    }
    else
    {
        /*1 byte*/
        ins->low_disp = (int)read_buffer[*buffer_ptr] - '0';
        *buffer_ptr ++;
        ins->length += 8;

        if((ins->low_disp & 0x80) >> 7)
        {
            /*minus*/
            complement = ~ins->low_disp;
            ins->asem[offset] = '-';
            ins->asem[offset + 1] = '0';
            ins->asem[offset + 2] = '0';
            ins->asem[offset + 3] = complement[0];
            ins->asem[offset + 4] = complement[1];
            offset += 5;
        }
        else
        {
            decimalToHexadecimal(ins->low_disp, hexadecimal);
            ins->asem[offset] = '0';
            ins->asem[offset + 1] = '0';
            ins->asem[offset + 2] = hexadecimal[0];
            ins->asem[offset + 3] = hexadecimal[1];
            offset += 4;
        }
    }
    return offset;
}

/* when flag == 1, read 2 byte*/
int read_data(instruction* ins, int offset, int flag)
{
    char *hexadecimal, *complement;
    if(flag)
    {
        /*2 byte*/
        ins->data0 = (int)read_buffer[*buffer_ptr] - '0';
        *buffer_ptr ++;
        ins->length += 8;
        ins->data1 = (int)read_buffer[*buffer_ptr] - '0';
        *buffer_ptr ++;
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
        ins->data0 = (int)read_buffer[*buffer_ptr] - '0';
        *buffer_ptr ++;
        ins->length += 8;

        if((ins->data0 & 0x80) >> 7)
        {
            /*minus*/
            complement = ~ins->data0;
            ins->asem[offset] = '-';
            ins->asem[offset + 1] = '0';
            ins->asem[offset + 2] = '0';
            ins->asem[offset + 3] = complement[0];
            ins->asem[offset + 4] = complement[1];
            offset += 5;
        }
        else
        {
            decimalToHexadecimal(ins->data0, hexadecimal);
            ins->asem[offset + 0] = '0';
            ins->asem[offset + 1] = '0';
            ins->asem[offset + 2] = hexadecimal[0];
            ins->asem[offset + 3] = hexadecimal[1];
            offset += 4;
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
        asem_result->rear->next = node;
        asem_result->rear = node;
        asem_result->length ++;
    }
}

void register_addressing_8bit(int data, char* reg)
{
    switch(data)
    {
        case 0x00:
            strcpy(reg, "AL\0");
            break;
        case 0x01:
            strcpy(reg, "CL\0");
            break;
        case 0x02:
            strcpy(reg, "DL\0");
            break;
        case 0x03:
            strcpy(reg, "BL\0");
            break;
        case 0x04:
            strcpy(reg, "AH\0");
            break;
        case 0x05:
            strcpy(reg, "CH\0");
            break;
        case 0x06:
            strcpy(reg, "DH\0");
            break;
        case 0x07:
            strcpy(reg, "BH\0");
            break;
    }
}

char* register_addressing_16bit(int data, char* reg)
{
    switch(data)
    {
        case 0x00:
            strcpy(reg, "AX\0");
            break;
        case 0x01:
            strcpy(reg, "CX\0");
            break;
        case 0x02:
            strcpy(reg, "DX\0");
            break;
        case 0x03:
            strcpy(reg, "BX\0");
            break;
        case 0x04:
            strcpy(reg, "SP\0");
            break;
        case 0x05:
            strcpy(reg, "BP\0");
            break;
        case 0x06:
            strcpy(reg, "SI\0");
            break;
        case 0x07:
            strcpy(reg, "DI\0");
            break;
    }
}

void decimalToHexadecimal(int decimal, char* result)
{
    int i, x;
    x = decimal & 0xf0;
    result[0] = hexadecimal_table[x];
    x = decimal & 0x0f;
    result[1] = hexadecimal_table[x];
    result[2] = '\0';
}

void byte_complement(int byte_data, char* result)
{
    int complement;
    complement = ~byte_data;
    result[0] = (complement & 0xf0) >> 4;
    result[1] = (complement & 0x0f);
    result[2] = '\0';
}

char* text_to_instruction(exec* hdr)
{
    int i, text_end, byte_data;
    text_end = buffer_ptr + (int)(hdr->a_text);
    /*loop until the end of text*/
    while(text_end - *buffer_ptr > 0)
    {
        /*new memory allocation for every instruction*/
        instruction *ins;
        ins = malloc(sizeof(instruction));
        /*read the first byte*/
        byte_data = (int)read_buffer[*buffer_ptr] - '0';
        instruction_func[byte_data](ins, read_buffer, buffer_ptr, 0);
    }    
}

