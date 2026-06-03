#include "trap.h"
#include "timer.h"
#include "scheduler.h"
#include "uart.h"
void trap_handler(uint64_t *frame)
{
uint64_t scause;
/* TODO:
* Ler CSR scause.
*/
/* TODO:
* Verificar se interrupo.
*/
/* TODO:
* Identificar Supervisor Timer Interrupt.
*/
/* TODO:
* Chamar timer_next().
* Chamar schedule_from_trap(frame).
*/
uart_print("Unhandled trap\n");
while (1)
;
}
