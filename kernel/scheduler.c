#include "scheduler.h"
#include "task.h"

extern void context_switch(void*, void*);

static int current = 0;

/*   Round-Robin padrão   */

static int round_robin()
{
    return (current + 1) % task_count;
}

/*   Algoritmo atual   */

static sched_algo_t current_algo = round_robin;

void scheduler_set_algorithm(sched_algo_t algo)
{
    if (algo)
        current_algo = algo;
}


void schedule_from_trap(uint64_t *frame)
{
    int prev = current;
    int next = current_algo();
/* TODO:
* Copiar frame -> tasks[prev].regs*/
    for(int i = 0; i < 32; i++){
        tasks[prev].regs[i] = frame[i];
}
/* TODO:
* Salvar sepc da task atual.*/
    uint64_t current_sepc;
    asm volatile("csrr %0, sepc" : "=r"(current_sepc));
    tasks[prev].sepc = current_sepc;
    
    current = next;
/* TODO:
* Copiar tasks[next].regs -> frame*/
    for(int i = 0; i < 32; i++){
        frame[i] = tasks[next].regs[i];
    }
/* TODO:
* Restaurar sepc da prxima task.*/
    asm volatile("csrw sepc, %0" :: "r"(tasks[next].sepc));    
}


/*   Yield   */

void yield()
{
    int prev = current;
    int next = current_algo();

    current = next;

    context_switch(tasks[prev].regs,
                   tasks[next].regs);
}
 
/*   Início   */

void scheduler_start()
{
    if (task_count == 0)
        return;

    tasks[0].entry();
}
