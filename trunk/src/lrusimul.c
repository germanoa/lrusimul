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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>

mem_struct *mem;
mem_actions_struct *mem_actions;
proc_struct_list *procs;

mem_actions_struct* file_2_memaction ( FILE *system_config )
{
	int lineNumber = 0;
	
	char *line;

	int argsReads;
	char action_str[255];
	int p1, p2;
	
	mem_actions_struct *top = 0, *tail = 0, *current;	
	
	current = malloc(sizeof(mem_actions_struct));
    
    while(!feof(system_config))
    {
	    lineNumber++;
	    
		fgets(line, 255, system_config);		

		//Alloc mec_action
		current = malloc(sizeof(mem_actions_struct));
		
		if ( current == NULL )
		{
			printf("Problem on allocation memory!\n");
			return NULL;
		}
		
		//Read infos		
	    argsReads = sscanf(line, "%s %d %d", action_str, &p1, &p2); 
	    printf("%s %d %d\n", action_str, p1, p2);
	    if ( argsReads == 2 )
	    {
	    	if ( strcmp(action_str, MEMSIZE_STR) == 0 )
	    	{
	    		current->action = MEMSIZE;
	    		current->parameter1 = p1;
	    	}
	    	else if ( strcmp(action_str, ENDPROC_STR) == 0 )
	    	{
	    		current->action = ENDPROC;
	    		current->parameter1 = p1;
	    	}
	    	else
	    	{
	    		printf("Invalid command in line %d. Ignoring it. \n", lineNumber);
	    		free(current);
	    		current = NULL;
	    	}
 		}
 		else
 		{
 			if ( strcmp(action_str, PROCSIZE_STR) == 0 )
	    	{ 	
	    		current->action = PROCSIZE;
	    		current->parameter1 = p1;
	    		current->parameter2 = p2;	    			    		
	    	}
	    	else if ( strcmp(action_str, READ_STR) == 0 )
	    	{
	    		current->action = READ;
	    		current->parameter1 = p1;
	    		current->parameter2 = p2;	    			    		
	    	}
	    	else if ( strcmp(action_str, WRITE_STR) == 0 )
	    	{
	    		current->action = WRITE;
	    		current->parameter1 = p1;
	    		current->parameter2 = p2;	    		
	    	}
	    	else
	    	{
	    		printf("Invalid command in line %d. Ignoring it. \n", lineNumber);
	    		free(current);
	    		current = NULL;
	    	}
 		}	
 		
 		if ( top == NULL )
		{
			top = current;
			tail = current;
		}
		else
		{
			tail->next = current;
			tail = current;
		}
    }
    return top;
}

void system_run(mem_actions_struct *top)
{
	if ( top->action != MEMSIZE )
	{
		printf("The first action must be MEMSIZE number\n");
		exit(1);
	}
	
	procs = malloc(sizeof(proc_struct_list));
	for(;top->next; top = top->next)
	{
		printf("%d %d %d\n", top->action, top->parameter1, top->parameter2);
		execute_action(top);
	}
	
	print_procs_stats();
}

void print_procs_stats()
{
	int i;
	
	for(; procs; procs = procs->next)
	{
		printf("PROCESSO: %d\n", procs->proc->pid);
		printf("Página Acessos(R/W) NroPageFault NroSubst\n");
		for(i = 0; i < procs->proc->size; i++)
		{
			print_page_stats(procs->proc->page_table[i]);
		}
	}
}

void print_page_stats(page_struct page)
{
	printf("%d %d %d %d\n", page.page, (page.read_access + page.write_access), page.page_faults, page.nro_subst);
}

void execute_action(mem_actions_struct *action)
{
	switch(action->action)
	{
		case MEMSIZE:
			memsize_action(action->parameter1);
			break;
		case PROCSIZE:
			procsize_action(action->parameter1, action->parameter2);
			break;		
		case READ:
			read_action(action->parameter1, action->parameter2);
			break;		
		case WRITE:
			write_action(action->parameter1, action->parameter2);
			break;
		case ENDPROC:
			endproc_action(action->parameter1);
			break;		
	}
}

void lru_2nd_choice(mem_struct *mem)
{
    printf ("not implemented yet.\n");
}

void memsize_action(int size)
{
	mem = malloc(sizeof(mem_struct));
	mem->size = size;
}

void reset_page(page_struct *page, int page_number)
{
	if( page == NULL)
	{
		printf("Invalid page.\n");
	}

	page->page = page_number;
	//page->local = SWAP;
	//page->RB = 0;
//	page->MB = 0;
	//page->read_access = 0;
///	page->write_access = 0;
//	page->page_faults = 0;
//	page->nro_subst = 0;
}

int procsize_action(int pid, int size)
{
	int i;
    proc_struct *proc = malloc(sizeof(proc_struct));

    if (proc == NULL)
    {
    	printf("Problem on alloc memory to process %d with size %d\n", pid, size);
    	exit(1);
    }

    proc->pid = pid;
    proc->size = size;    
    proc->page_table = malloc((1+ size) * sizeof(proc_struct)); 
    
    if (proc->page_table == NULL)
    {
    	printf("Problem on alloc memory to process %d with size %d\n", pid, size);
    	exit(1);
    }
    
    for(i = 0; i < size; i++)
	{
		reset_page(&proc->page_table[i], i);
	}
    
    if ( procs->proc == NULL )
    {
    	procs->proc = proc;
	}
	else
	{
		proc_struct_list *l = procs;
		
		for(; l->next; l = l->next) {
		}
		
		l->next = malloc(sizeof(proc_struct_list));
		l->next->proc = proc;		
	}
    
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
	if ( argc != 2 )
	{
		printf("Incorrect number of parameters. You must pass a file with the commands. Use: lrusimul myconfig.txt\n");
		return 1;
	}
	
    FILE *system_config;
    system_config = fopen(argv[1], "r+");
    
    if ( system_config == NULL)
    {
    	printf("File not found\n");
    	return 2;
    }
    
    
    system_run( file_2_memaction(system_config) );

    fclose(system_config);

    return 0;
}

