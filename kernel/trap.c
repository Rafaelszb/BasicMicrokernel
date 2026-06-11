#include "trap.h"
#include "timer.h"
#include "scheduler.h"
#include "uart.h"

extern void uart_print(const char*);

void trap_handler(uint64_t *frame)
{
    uint64_t scause;
    
/* TODO: Ler CSR scause.*/

    asm volatile(
        "csrr %0, scause" 
        : "=r"(scause)
    );

    // Isolamos o bit 63 (Interrupt) e os 4 bits inferiores (Exception Code)
    uint64_t is_interrupt = (scause >> 63) & 1ULL;
    uint64_t trap_code = scause & 0xFFFULL;

    /* TODO: Verificar se interrupo.*/
    if (is_interrupt && (trap_code == 5)) { /* TODO: Identificar Supervisor Timer Interrupt.*/
        
     /* TODO: Chamar timer_next().*/
        timer_next();

    /*Chamar schedule_from_trap(frame).*/
        schedule_from_trap(frame);
        return;
    } 
    
    uart_print("Unhandled trap\n");
    while (1);
}
