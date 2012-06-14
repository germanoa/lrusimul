/*
 * (Header)
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
#include <include/retcodes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Memory Actions
 */
#define MEMSIZE 0
#define PROCSIZE 1
#define READ 2
#define WRITE 3
#define ENDPROC 4

#define MEMSIZE_STR "MEMSIZE"
#define PROCSIZE_STR "PROCSIZE"
#define READ_STR "READ"
#define WRITE_STR "WRITE"
#define ENDPROC_STR "ENDPROC"

/*
 * Page Locals
 */
#define SWAP 0
#define MEM 1


/*
 * Memory
 */
typedef struct mem_struct mem_struct;
struct mem_struct {
    int size; /* number of frames */
    int used; /* number of frames used */
    struct page_struct *loaded_pages_first; /* The first(head) page in use in this memory */
    struct page_struct *loaded_pages_tail;
};

/*
 * Memory Actions
 */
typedef struct mem_actions_struct mem_actions_struct;
struct mem_actions_struct {
    int action;
    int parameter1;
    int parameter2; /* If not used, receive NULL. */
    mem_actions_struct *next;
};

/*
 * Process
 */
typedef struct proc_struct proc_struct;
struct proc_struct {
    int pid; /* process id */
    int size; /* number of pages */
    struct page_struct *page_table; /* The first(head) page of this process */
};

typedef struct proc_struct_list proc_struct_list;
struct proc_struct_list {
	proc_struct *proc;
	proc_struct_list *next;
};

/*
 * Page
 */
typedef struct page_struct page_struct;
struct page_struct {
    int page; /* page number */
    int local; /* present local */
    int RB; /* reference bit */
    int MB; /* modification bit */
    int read_access; /* stats */
    int write_access; /* stats */
    int page_faults; /* stats */
    int nro_subst; /* stats */
    struct page_struct *prev_mem_page; /* If local=MEM, the next page of the mem. If local=SWAP, NULL. */
    struct page_struct *next_mem_page; /* If local=MEM, the next page of the mem. If local=SWAP, NULL. */
};

/*
 * Receive input text file ; load mem_actions struct.
 */
mem_actions_struct* file_2_memaction ( FILE *system_config );

/*
 * Execute the system.
 */
void system_run(mem_actions_struct *system);

/*
 * Execute an action
 */
void execute_action(mem_actions_struct *action);

/*
 * LRU second choice
 */
void lru_2nd_choice(page_struct *page);

/*
 * Move a page to swap area
 */
void move_to_swap(page_struct *page);

/*
 * Move a page to end of FIFO mem.
 */
void move_to_mem_tail(page_struct *candidate);

/*
 * MEMSIZE action 
 */
void memsize_action(int size);

/*
 * PROCSIZE action 
 */
int procsize_action(int pid, int size);

/*
 * READ action 
 */
int read_action(int page, int pid);

/*
 * WRITE action 
 */
int write_action(int page, int pid);

/*
 * ENDPROC action 
 */
int endproc_action(int pid);

/*
 * Reset a page informations and set her page number.
 */
void reset_page(page_struct *page, int page_number);

/*
 * Print proc_mem stats
 */
void print_procs_stats();
void print_page_stats(page_struct page);
