#include <stm32f103xb.h>
#include "os_func.h"
#include "hw_mcu.h"
#define F_TO_1uS    F_CPU/ 1000000

#define __disable_irq()  asm("CPSID I")
#define __enable_irq()   asm("CPSIE I")
extern uint32_t u_sec;
int         TASK_EN = 0;
struct      _task TASKS[TASK_COUNT];
int         currentTaskActive;
uint32_t    taskDispatchTimeWorked;
struct _statistics tasksInfo[TASK_COUNT + 1];
uint32_t    lastTimeStatisticsCollect, COUNT_OF_WAIT_PROCESSES, COUNT_OF_ALL_PROCESSES;
uint32_t statisticPeriod;

uint32_t millis()  {
  return (u_sec * 1000 + ((SysTick->LOAD - SysTick->VAL) / F_TO_1uS));
}
void TaskInit()
{
    int i;
    struct _task *task = TASKS;

    for(i = 0; i < TASK_COUNT; i++)
    {
        task->delay             = 0;
        task->func              = 0;
        task->repeat            = 0;
        task->timeWorked        = 0;
        tasksInfo[i].id         = 0;
        tasksInfo[i].timeWorked = 0;

    }
    taskDispatchTimeWorked      = 0;
    lastTimeStatisticsCollect   = 0;
    currentTaskActive = 0;
    TASK_EN = 1;
}

int TaskFind(callback func)
{
    int i;
    struct _task *task;
    for(i = 0, task = TASKS; i < TASK_COUNT; i++)
    {
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
    task->timeWorked= 0;
    __enable_irq();
    return i;
}

void statisticsPrepare()    {
    int i;
    struct _task *task;
    statisticPeriod = lastTimeStatisticsCollect - millis();
    lastTimeStatisticsCollect = millis();
    COUNT_OF_WAIT_PROCESSES = COUNT_OF_ALL_PROCESSES = 0;
    for(i = 0, task = TASKS; i < TASK_COUNT; i++, task++){
        if(task->func)  {
            if(task->delay == 0)
                COUNT_OF_WAIT_PROCESSES++;
            tasksInfo[COUNT_OF_ALL_PROCESSES].id = i;
            tasksInfo[COUNT_OF_ALL_PROCESSES].timeWorked = task->timeWorked;
            task->timeWorked = 0;
            COUNT_OF_ALL_PROCESSES++;
        }
    }
    tasksInfo[TASK_COUNT].id            = 199; //taskDispatch or may all services
    tasksInfo[TASK_COUNT].timeWorked    = taskDispatchTimeWorked;
    taskDispatchTimeWorked              = 0;
}
void TaskDispatch()
{
    void (*func)();
    uint16_t i, x;
    struct _task *task;
    uint32_t taskTime = millis();
    for(i = 0, x = currentTaskActive, x = (x + 1 >= TASK_COUNT)? 0: ++x; i < TASK_COUNT; i++)
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
            taskDispatchTimeWorked += (millis() - taskTime);
            taskTime = millis();
            func();     //now its a C function call, will improved to switch context asm paste
            task->timeWorked += (millis() - taskTime);
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
        if(task->delay)
            --task->delay;
}
