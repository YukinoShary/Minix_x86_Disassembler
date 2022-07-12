#ifndef _STRUCTURE_H
#define _STRUCTURE_H
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
    /*function code*/
    int reg;
    int w;
    int d;
    int s;
    int v;
    int mod;
    int rm;
    int low_disp;
    int high_disp;
    int data0;
    int data1;
    int type;
    int port;
    char asem[64];
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

typedef struct message
{
    int m_source;
    int m_type;
    union 
    {
        /* data */
        int m1;
        int m2;
    } m_u;
} message;

#endif