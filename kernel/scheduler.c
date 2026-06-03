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
* Copiar frame -> tasks[prev].regs
*/
/* TODO:
* Salvar sepc da task atual.
*/
current = next;
/* TODO:
* Copiar tasks[next].regs -> frame
*/
/* TODO:
* Restaurar sepc da prxima task.
*/
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
