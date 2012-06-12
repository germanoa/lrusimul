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
#include <stdbool.h>
#include <malloc.h>

mem_struct *mem;
mem_actions_struct *mem_actions;

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
	for(;top->next; top = top->next)
	{
		printf("%d %d %d\n", top->action, top->parameter1, top->parameter2);
		execute_action(top);
	}
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

