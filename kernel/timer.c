#include "timer.h"

static uint64_t tick_interval = 100000;

/* Implementação da chamada SBI set_timer utilizando a convenção do RISC-V */
static inline void sbi_set_timer(uint64_t time)
{
    #if __riscv_xlen == 64
    // Em sistemas de 64 bits, o argumento vai direto no registrador a0
    register unsigned long a0 __asm__("a0") = time;
    register unsigned long a7 __asm__("a7") = 0; // EID para Set Timer (SBI Extension ID 0x00)

    __asm__ volatile (
        "ecall"
        : "+r" (a0)
        : "r" (a7)
        : "memory"
    );
    #else
    // Caso esteja em um ambiente 32-bits, seriam necessários a0 e a1 para os 64-bits de 'time'
    #error "Suporte apenas para RISC-V 64-bits configurado."
    #endif
}

void timer_next(void)
{
    uint64_t now;

    /* Lendo o CSR 'time' */
    __asm__ volatile ("csrr %0, time" : "=r" (now));

    /* Programando o próximo disparo para: agora + intervalo */
    sbi_set_timer(now + tick_interval);
}

void timer_init(uint64_t interval)
{
    if (interval != 0)
        tick_interval = interval;

    // Configura o primeiro disparo do timer
    timer_next();

    /* * Habilitar STIE (Supervisor Timer Interrupt Enable) no CSR sie (bit 5)
     * Habilitar SIE (Supervisor Interrupt Enable) global no CSR sstatus (bit 1)
     */
    unsigned long sie_mask = (1 << 5);   // STIE bit
    unsigned long sstatus_mask = (1 << 1); // SIE bit

    __asm__ volatile (
        "csrs sie, %0" 
        : 
        : "r" (sie_mask) 
        : "memory"
    );
    
    __asm__ volatile (
        "csrs sstatus, %0" 
        : 
        : "r" (sstatus_mask) 
        : "memory"
    );
}
