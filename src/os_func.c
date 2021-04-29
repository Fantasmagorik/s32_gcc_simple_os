#include <stm32f103xb.h>
#include "os_func.h"

#define __disable_irq()  asm("CPSID I")
#define __enable_irq()   asm("CPSIE I")


int TASK_EN = 0;
struct _task TASKS[TASK_COUNT];
int currentTaskActive;

void TaskInit()
{
    int i;
    struct _task *task = TASKS;

    for(i = 0; i < TASK_COUNT; i++)
    {
        task->delay  = 0;
        task->func   = 0;
        task->repeat = 0;
    }
    currentTaskActive = 0;
    TASK_EN = 1;
}

int TaskFind(callback func) {
    int i;
    struct _task *task;
    for(i = 0, task = TASKS; i < TASK_COUNT; i++) {
        if(task->func == func)
            return i + 1;
        }
    return 0;
}


int addTask(callback func, uint16_t delay, uint16_t repeat)
{
    int i;
    struct _task *task;
    //! critical section
    __disable_irq();
    for(i = 0, task = TASKS; i < TASK_COUNT; i++, task++)
        if(task->func == 0)
            break;
    if(task->func)
    {
        __enable_irq();
        return -1;
    }
    task->func      = func;
    task->delay     = delay;
    task->repeat    = repeat;
    __enable_irq();
    return i;
}

void TaskDispatch()
{
    void (*func)();
    uint16_t i, x;
    struct _task *task;
    for(i = 0, x = (currentTaskActive + 1 >= TASK_COUNT)? 0: ++x; i < TASK_COUNT; i++)
    {
        task = (struct _task*) TASKS + x;

        if((task->delay == 0) && (task->func))
        {
            func = task->func;
            currentTaskActive = x;
            if(task->repeat)
                task->delay = task->repeat;
            else
                task->func = 0;

            func();     //now its a C function call, will improved to switch context asm paste
            break;
        }

        x = (++x >= TASK_COUNT)? 0: x;
    }
}

void TaskTick()
{
    int i;
    struct _task *task;
    for(i = 0, task = TASKS; i < TASK_COUNT; i++, task++)
    {
        if(task->delay)
        {
            --task->delay;
            if(task->delay == 0)
            {
                //launch the task
                asm("nop");
            }
        }
    }
}
