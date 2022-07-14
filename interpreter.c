#include "interpreter_define.h"

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

void int_TS_interp(instruction* ins, int type)
{
    int byte_data, i, source, func_type;
    int data_set[8];
    char* ptr;
    i = 0;
    if(type == 36)
    {
        kernel_main(register_status[0x03]);
    }
}