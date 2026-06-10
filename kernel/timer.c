#include "timer.h"

static uint64_t tick_interval = 100000;

static inline void sbi_set_timer(uint64_t time)
{
    register unsigned long a0 __asm__("a0") = time;
    register unsigned long a6 __asm__("a6") = 0;          // FID para set_timer
    register unsigned long a7 __asm__("a7") = 0x54494D45; // EID oficial da extensão "TIME"

    __asm__ volatile (
        "ecall"
        : "+r" (a0)
        : "r" (a6), "r" (a7)
        : "memory"
    );
}

void timer_next(void)
{
    uint64_t now;

    /* TODO: Ler o CSR time.    */
    __asm__ volatile ("csrr %0, time" : "=r" (now));

    /* TODO: Programar now + tick_interval.*/
    sbi_set_timer(now + tick_interval);
}

void timer_init(uint64_t interval)
{
    if (interval != 0)
        tick_interval = interval;

    timer_next();

    /* TODO:
    * Habilitar STIE no CSR sie.
    * Habilitar SIE global em sstatus.
    */

    unsigned long sie_mask = (1ULL << 5);   // STIE bit
    unsigned long sstatus_mask = (1ULL << 1); // SIE bit

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
