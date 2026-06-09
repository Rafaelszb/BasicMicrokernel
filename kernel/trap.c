#include "trap.h"
#include "timer.h"
#include "scheduler.h"
#include "uart.h"

#define SCAUSE_INTERRUPT_BIT (1ULL << 63)
#define SCAUSE_CODE_MASK (~(1ULL << 63))

#define IRQ_S_TIMER 5

void trap_handler(uint64_t *frame)
{
    uint64_t scause;
    // Lê o motivo do trap
    asm volatile("csrr %0, scause" : "=r"(scause));

    // Verifica se o bit mais significativo é 1 (Interrupção) 
    // e se o código é 5 (Supervisor Timer Interrupt)
    if ((scause & (1ULL << 63)) && ((scause & 0xFFF) == 5)) {
        
        // 1. Agenda o próximo tick do relógio para limpar a flag de interrupção do hardware
        timer_next();

        // 2. Chama o escalonador para trocar as pilhas e o sepc dentro do frame
        schedule_from_trap(frame);
        
    } else {
        // Se caiu aqui, foi um erro de instrução, página ou ecall não esperado
        uart_print("Trap nao tratado! scause: ");
        // (Opcional) Print do valor de scause em hexa para debug
        while(1); 
    }
}
