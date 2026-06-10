#include "trap.h"
#include "timer.h"
#include "scheduler.h"
#include "uart.h"

extern void uart_print(const char*);

void trap_handler(uint64_t *frame)
{
    uint64_t scause_val;
    
    // Lê o CSR scause
    asm volatile("csrr %0, scause" : "=r"(scause_val));

    // Isolamos o bit 63 (Interrupt) e os 4 bits inferiores (Exception Code)
    uint64_t is_interrupt = (scause_val >> 63) & 1ULL;
    uint64_t trap_code = scause_val & 0xFFFULL;

    // Se for uma interrupção (1) E o código for do Timer de Supervisor (5)
    if (is_interrupt && (trap_code == 5)) {
        
        // 1. Limpa a interrupção atual agendando o próximo tick
        timer_next();

        // 2. Transfere o controle para o escalonador rotacionar as tarefas
        schedule_from_trap(frame);
        
    } 
    
    uart_print("Unhandled trap\n");
    while (1);
}
