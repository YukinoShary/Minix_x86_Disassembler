#include "tool_func_define.h"

int (*call_vec[79])(message* msgptr) = 
{
   /*this is the line 1 */NULL,
    /*this is the line 1 */NULL,
    /*this is the line 2 */NULL,
    /*this is the line 3 */NULL,
    /*this is the line 4 */do_write,
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
    /*this is the line 30 */NULL,
    /*this is the line 31 */NULL,
    /*this is the line 32 */NULL,
    /*this is the line 33 */NULL,
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
    /*this is the line 4f */NULL
};

void mov_I2R_interp(instruction* ins)
{
    if(ins->w == 1)
    {
        register_status[ins->reg] == (ins->data1 >> 8) + ins->data0;
    }
    else 
    {
        register_status[ins->reg] = ins->data0;
    }   
}

void int_TS_interp(instruction* ins, int type, unsigned char* read_buffer, int* buffer_ptr)
{
    int byte_data, i;
    int data_set[8];
    int buffer[8];
    message* msgptr;
    i = 0;
    if(type == 36)
    {
        mem_ptr = register_status[0x03];
        while(i < 8)
        {
            /*big endian low*/
            byte_data = (int)virtual_memory[*mem_ptr];
            data_set[i] = byte_data;
            *mem_ptr += 1;
            /*big endian high*/
            byte_data = (int)virtual_memory[*mem_ptr];
            data_set[i] += byte_data << 8;
            *mem_ptr += 1;
            i ++;
        }
        
        call_vec[msgptr->m_type](msgptr);
    }
}