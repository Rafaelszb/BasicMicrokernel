#include "scheduler.h"
#include "task.h"

extern TCB tasks[];
extern int task_count;

extern void context_switch(void*, void*);

static int current = 0;

/* Round-Robin padrão   */
static int round_robin()
{
    if (task_count <= 1) return 0;
    return (current + 1) % task_count;
}

/* Algoritmo atual   */
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

    // Proteção: Se for a mesma tarefa, não há necessidade de trocar contexto
    if (prev == next) {
        return;
    }

    /* Copiar frame -> tasks[prev].regs (Limite corrigido para 31) */
    for(int i = 0; i < 31; i++){
        tasks[prev].regs[i] = frame[i];
    }

    /* Salvar sepc da task atual. */
    uint64_t current_sepc;
    asm volatile("csrr %0, sepc" : "=r"(current_sepc));
    tasks[prev].sepc = current_sepc;
    
    current = next;

    /* Copiar tasks[next].regs -> frame */
    for(int i = 0; i < 31; i++){
        frame[i] = tasks[next].regs[i];
    }

    /* Restaurar sepc da próxima task. */
    asm volatile("csrw sepc, %0" :: "r"(tasks[next].sepc));    
}

/* Yield   */
void yield()
{
    int prev = current;
    int next = current_algo();

    if (prev == next) return;

    current = next;

    context_switch(tasks[prev].regs,
                   tasks[next].regs);
}
 
/* Início   */
void scheduler_start()
{
    if (task_count == 0)
        return;

    current = 0;
    tasks[0].entry();
}
