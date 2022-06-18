#include "tool_funcs.c"
#include "function_define.h"
#define BUFFER_SIZE 32 * 1024

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
        byte_data = (int)read_buffer[*buffer_ptr];
        instruction_func[byte_data](ins, read_buffer, buffer_ptr, 0);
    }    
}

