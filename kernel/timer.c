#include "timer.h"
static uint64_t tick_interval = 100000;
/* TODO:
* Implementar chamada SBI set_timer.
*
* Prottipo sugerido:
* static inline void sbi_set_timer(uint64_t time);
*/
void timer_next(void)
{
uint64_t now;
/* TODO:
* Ler o CSR time.
*/
/* TODO:
* Programar now + tick_interval.
*/
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
