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
  /* TODO:
  * Ler CSR scause.*/
  asm volatile("csrr %0, scause" : "=r"(scause));
  /* TODO:
  * Verificar se interrupo. */
  if(scause & SCAUSE_INTERRUPT_BIT){
    uint64_t code = scause & SCAUSE_CODE_MASK;
  /* TODO:
  * Identificar Supervisor Timer Interrupt.  */
    if(code == IRQ_S_TIMER){
    /* TODO:
    * Chamar timer_next().
    * Chamar schedule_from_trap(frame).*/      
      timer_next();
      schedule_from_trap(frame);
      return;
    }
  }
  
  uart_print("Unhandled trap\n");
  while (1);
}
