/*
 * (Source)
 * lrusimul
 * LRU Second Choice Memory Manager Simulator
 * 
 * Universidade Federal do Rio Grande do Sul - UFRGS
 * Sistemas Operacionais I
 *
 * Germano Andersson <germanoa@gmail.com>
 * Felipe Lahti <felipe.lahti@gmail.com>
 *
 * 2012-06-14
 */

#include <include/list.h>
#include <include/lrusimul.h>
#include <include/retcodes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

mem_struct *mem;
mem_actions_struct *mem_actions;


void file_2_memaction(FILE *system_config)
{
    printf ("not implemented yet.\n");
}

void system_run(mem_actions_struct *system)
{
    printf ("not implemented yet.\n");
}

void lru_2nd_choice(mem_struct *mem)
{
    printf ("not implemented yet.\n");
}

int memsize_action(int size)
{
    printf ("not implemented yet.\n");
    return 0;
}

int procsize_action(int pid, int size)
{
    printf ("not implemented yet.\n");
    return 0;
}

int read_action(int page, int pid)
{
    printf ("not implemented yet.\n");
    return 0;
}

int write_action(int page, int pid)
{
    printf ("not implemented yet.\n");
    return 0;
}

int endproc_action(int pid)
{
    printf ("not implemented yet.\n");
    return 0;
}

void print_proc_mem_stats(proc_struct *proc)
{
    printf ("not implemented yet.\n");
}


int main (int argc, char *argv[])
{
    FILE *system_config;
    //usar system_config = fopen

    file_2_memaction(system_config);
    system_run(mem_actions);

    return 0;
}

