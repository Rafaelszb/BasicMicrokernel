#include "timer.h"
static uint64_t tick_interval = 100000;
/* TODO:
* Implementar chamada SBI set_timer.
*
* Prottipo sugerido:
* static inline void sbi_set_timer(uint64_t time);*/
static inline void sbi_set_timer(uint64_t time)
{
  register uint64_t a0 asm("a0") = time;
  register uint64_t a6 asm("a6") = 0;
  register uint64_t a7 asm("a7") = 0;

  asm volatile( "ecall"      : "+r"(a0)      : "r"(a6), "r"(a7)      : "memory"  );
}

void timer_next(void)
{
  uint64_t now;
  /* TODO:
  * Ler o CSR time.*/
  asm volatile("csrr %0, time" : "=r"(now));
  
  /* TODO:
  * Programar now + tick_interval.  */
  sbi_set_timer(now + tick_interval);
}

void timer_init(uint64_t interval)
{
  if (interval != 0)
  tick_interval = interval;
  timer_next();
  /* TODO:
  * Habilitar STIE no CSR sie.*/
  uint64_t sie_bit = (1 << 5);
  asm volatile("csrs sie, %0" :: "r"(sie_bit));

  /* Habilitar SIE global em sstatus. */
  uint64_t sstatus_bit = (1 << 1);
  asm volatile("csrs sstatus, %0" :: "r"(sstatus_bit));
}
