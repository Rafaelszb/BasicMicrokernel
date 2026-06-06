#include "task.h"
#include "scheduler.h"
#include "memory.h"
#include "timer.h"
#include "trap.h"

extern void uart_print(const char*);
extern void trap_entry(void);


/*   Tasks   */

void task1()
{
    while (1)
    {
        uart_print("Task 1 running\n");
    }
}

void task2()
{
    while (1)
    {
        uart_print("Task 2 running\n");
    }
}

/*   Kernel   */

void kernel_main()
{
    memory_init();
    
    uart_print("\n=== Kernel ===\n");
    
    xTaskCreate(task1, 2048, 1);
    xTaskCreate(task2, 2048, 1);
    
    asm volatile("csrw stvec, %0" :: "r"(trap_entry));
    
    timer_init(100000);
    
    scheduler_start();
    
    while (1);
}
