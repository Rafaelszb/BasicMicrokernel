#include "scheduler.h"
#include "task.h"

extern TCB tasks[];
extern int task_count;
extern void context_switch(void*, void*);

static int current = 0;

static int round_robin()
{
    if (task_count <= 1) return 0;
    return (current + 1) % task_count;
}

static sched_algo_t current_algo = round_robin;

void scheduler_set_algorithm(sched_algo_t algo)
{
    if (algo) current_algo = algo;
}

// ALTERAÇÃO: Agora retorna o ponteiro do novo contexto
uint64_t* schedule_from_trap(uint64_t *frame)
{
    int prev = current;
    int next = current_algo();

    // Se for a mesma tarefa, retorna o mesmo frame sem alterar nada
    if (prev == next) {
        return frame; 
    }

    /* 1. Salva o contexto da tarefa antiga no vetor dela */
    for(int i = 0; i < 32; i++){
        tasks[prev].regs[i] = frame[i];
    }

    /* Salva o sepc da task atual */
    uint64_t current_sepc;
    asm volatile("csrr %0, sepc" : "=r"(current_sepc));
    tasks[prev].sepc = current_sepc;
    
    // Atualiza o índice global da tarefa ativa
    current = next;

    /* Restaurar sepc da próxima task nos registradores de controle da CPU */
    asm volatile("csrw sepc, %0" :: "r"(tasks[next].sepc));    

    /* 2. RETORNO DO PULO DO GATO: 
       Em vez de escrever no frame antigo, passamos o endereço do vetor da nova tarefa.
       O Assembly vai ler diretamente daqui! */
    return (uint64_t*)(tasks[next].regs);
}

/* Yield */
void yield()
{
    int prev = current;
    int next = current_algo();

    if (prev == next) return;

    current = next;
    context_switch(tasks[prev].regs, tasks[next].regs);
}
 
/* Início */
void scheduler_start()
{
    if (task_count == 0) return;
    current = 0;
    tasks[0].entry();
}
