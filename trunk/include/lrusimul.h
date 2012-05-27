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

/*
 * Memory Actions
 */
#define MEMSIZE 0
#define PROCSIZE 1
#define READ 2
#define WRITE 3
#define ENDPROC 4

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
    struct page_struct *loaded_pages; /* The first(head) page in use in this memory */
};

/*
 * Memory Actions
 */
typedef struct mem_actions_struct mem_actions_struct;
struct mem_actions_struct {
    int action;
    int parameter1;
    int parameter2; /* If not used, receive NULL. */
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

/*
 * Page
 */
typedef struct page_struct page_struct;
struct page_struct {
    int page; /* page number */
    int local; /* present local */
    int RB; /* reference bit */
    int MB; /* modification bit */
    int read_acess; /* stats */
    int write_acess; /* stats */
    int page_faults; /* stats */
    int nro_subst; /* stats */
    struct list_head *next_proc_page; /* The next page of the process */
    struct list_head *next_mem_page; /* If local=MEM, the next page of the mem. If local=SWAP, NULL. */
};


/*
 * Receive input text file ; return mem_actions struct.
 */
mem_actions_struct file_2_memaction(char *file);

/*
 * LRU second choice
 */
void lru2(mem_struct *mem);

/*
 * MEMSIZE action 
 */
int memsize_action(int size);

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
 * Print proc_mem stats
 */
void print_proc_mem_stats(proc_struct *proc);

