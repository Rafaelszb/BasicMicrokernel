#include "trap.h"
#include "timer.h"
#include "scheduler.h"
#include "uart.h"

#define SCAUSE_INTERRUPT_BIT (1ULL << 63)
#define SCAUSE_CODE_MASK (~(1ULL << 63))

#define IRQ_S_TIMER 5

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
        
    } else {
        // Se caiu aqui, vamos travar, mas avisando qual o número do problema
        uart_print("\n[Kernel] Trap nao tratado! scause de debug: ");
        
        // Se o código for 1, significa um Instruction Misaligned Fault
        // Se is_interrupt for 0 e trap_code for 9, é uma ecall vinda do U-mode
        if (is_interrupt == 0) {
            uart_print("Excecao sincrona codigo ");
            // Como dica visual simples, você pode deduzir se é falha de página, etc.
        }
        
        while(1); 
    }
}
