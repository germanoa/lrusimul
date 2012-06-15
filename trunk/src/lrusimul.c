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
    line = malloc(255);

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
	    //printf("%s %d %d\n", action_str, p1, p2);
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
	for(;top; top = top->next)
	{
		//printf("%d %d %d\n", top->action, top->parameter1, top->parameter2);
		execute_action(top);
	}
	
	print_procs_stats();
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

void lru_2nd_choice(page_struct *page)
{
    if ( mem->used < mem->size)
    {
    	//just insert in FIFO
    	if ( mem->loaded_pages_first == NULL )
    	{
    		mem->loaded_pages_first = page;
    		mem->loaded_pages_tail = page;
    		page->prev_mem_page = page->next_mem_page = NULL;
    	}
    	else
    	{    		
    		mem->loaded_pages_tail->next_mem_page = page;
    		page->prev_mem_page = mem->loaded_pages_tail;
    		mem->loaded_pages_tail = page;
    		page->next_mem_page = NULL;
    	}

    	page->local = MEM;
    	mem->used++;
    }
    else
    {
    	//LRU 2nd choice
    	page_struct *candidate = mem->loaded_pages_first;

    	while ( !(mem->loaded_pages_first->RB == 0 && mem->loaded_pages_first->MB == 0) )
    	{
    		if ( candidate->RB == 0 && candidate->MB == 1)
	 		{
	 			candidate->MB = 0;
	 			move_to_mem_tail(candidate);
	 			//printf("RB = 0 && MB = 1\n");
	 		}
	    	else if ( candidate->RB == 1 && candidate->MB == 0)
	    	{
	    		candidate->RB = 0;
	    		move_to_mem_tail(candidate);
	    		//printf("RB = 1 && MB = 0\n");
	    	}
	    	else if ( candidate->RB == 1 && candidate->MB == 1)
	    	{
	    		candidate->RB = 0;
	    		move_to_mem_tail(candidate);
	    		//printf("RB = 1 && MB = 1\n");
	    	}		

	    	candidate = mem->loaded_pages_first;	
    	}   

    	move_to_swap(mem->loaded_pages_first);//*/ 	
   		mem->used--;
    	lru_2nd_choice(page);
    }
}

void move_to_mem_tail(page_struct *candidate)
{
	//move to tail
	mem->loaded_pages_tail->next_mem_page = candidate;
	candidate->prev_mem_page = mem->loaded_pages_tail;
	mem->loaded_pages_tail = candidate;
	mem->loaded_pages_first = candidate->next_mem_page;
	mem->loaded_pages_first->prev_mem_page = NULL;
	candidate->next_mem_page = NULL;
}

void move_to_swap(page_struct *page)
{
	page->local = SWAP;
	page->RB = 0;
	page->MB = 0;
	page->nro_subst++;

	//TODO: Verify if exists a bug when the frame size is 1.

	if ( page->next_mem_page == NULL)
	{
		printf("NEXT IS NULL\n");
	}
	else
	{
		mem->loaded_pages_first = mem->loaded_pages_first->next_mem_page;
		mem->loaded_pages_first->prev_mem_page = NULL;
		page->next_mem_page = NULL;
	}
}

void memsize_action(int size)
{
	mem = malloc(sizeof(mem_struct));
	mem->size = size;
	mem->used = 0;
}

void reset_page(page_struct *page, int page_number)
{
	if( page == NULL)
	{
		printf("Invalid page.\n");
	}

	page->page = page_number;
	page->local = SWAP;
	page->RB = 0;
	page->MB = 0;
	page->read_access = 0;
	page->write_access = 0;
	page->page_faults = 0;
	page->nro_subst = 0;
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
    proc->isGC = 0;
    proc->page_table = malloc((size + 1) * sizeof(page_struct)); 
    
    if (proc->page_table == NULL)
    {
    	printf("Problem on alloc memory to process %d with size %d\n", pid, size);
    	exit(1);
    }
    
    for(i = 0; i <= size; i++)
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

proc_struct* find_proc(int pid)
{
	proc_struct_list *aux;

	aux = procs;

	for(; aux; aux = aux->next)
	{
		if ( aux->proc->pid == pid)
		{
			return aux->proc;
		}
	}

	return NULL;
}

int read_action(int pageNumber, int pid)
{
    proc_struct *proc = find_proc(pid);

    if ( proc == NULL )
    {
    	printf("Process not exists. PID: %d \n", pid);
    	exit(1);    	
    }

    if ( !proc->isGC )
    {
    	if( pageNumber > proc->size )
	    {
	    	endproc_action(pid);
	    }
    }
    else 
    {
    	return 0;
    }

    page_struct *page = &proc->page_table[pageNumber];

    if ( page->local == SWAP )
    {
    	lru_2nd_choice(page);
    	page->page_faults++;
    }

    page->read_access++;
    page->RB = 1;

    return 0;
}

int write_action(int pageNumber, int pid)
{
    proc_struct *proc = find_proc(pid);

    if ( proc == NULL )
    {
    	printf("Process not exists. PID: %d \n", pid);
    	exit(1);
    }

    if ( !proc->isGC )
    {
    	if( pageNumber > proc->size )
	    {
	    	endproc_action(pid);
	    }
    }
    else 
    {
    	return 0;
    }

    page_struct *page = &proc->page_table[pageNumber];

    if ( page->local == SWAP )
    {
    	lru_2nd_choice(page);
    	page->page_faults++;
    }

    page->write_access++;
    page->MB = 1;

    return 0;
}

int endproc_action(int pid)
{
	int i;

    proc_struct *proc = find_proc(pid);

    if ( proc == NULL )
    {
    	printf("Process %d not finded to destroy.\n", proc->pid);
    	exit(1);
    }

    page_struct *page;

    for(i = 0; i <= proc->size; i++)
    {
		page = &proc->page_table[i];

		if ( page == NULL) 
		{
			return 0;
		}


		if ( page->local == MEM )
		{
			if ( page->prev_mem_page == 0)
			{
				mem->loaded_pages_first = page->next_mem_page;

				if ( mem->loaded_pages_first )
					mem->loaded_pages_first->prev_mem_page = NULL;
			}
			else
			{
				page->prev_mem_page->next_mem_page = page->next_mem_page;

				if (page->next_mem_page )
					page->next_mem_page->prev_mem_page = page->prev_mem_page;	
			}
			//Free page on memory
			mem->used--;
		}
    }

    return 0;
}

void print_procs_stats()
{
	int i;

	FILE *f = fopen("perf/log.txt", "w+");

	if ( f == NULL)
	{
		printf("Error opening perf/log.txt\n");
		exit(1);
	}
	
	for(; procs; procs = procs->next)
	{
		fprintf(f, "PROCESSO: %d\n", procs->proc->pid);
		fprintf(f, "Página Acessos(R/W) NroPageFault NroSubst\n");
		for(i = 0; i <= procs->proc->size; i++)
		{
			print_page_stats(procs->proc->page_table[i], f);
		}
	}

	for(; mem->loaded_pages_first; mem->loaded_pages_first = mem->loaded_pages_first->next_mem_page)
	{
		//printf("PAGE %d \n", mem->loaded_pages_first->page, f);
	}
}

void print_page_stats(page_struct page, FILE *f)
{
	fprintf(f, "%d %d %d %d\n", page.page, page.read_access + page.write_access, page.page_faults, page.nro_subst);
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
