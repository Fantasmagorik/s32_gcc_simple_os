//#include <stm32f103xb.h>


#define TASK_BLOCKED    0x33333333
#define TASK_COUNT      20


enum osErrorsList{
    UNKNOWN_ERROR = 1,
    TASK_LIST_IS_FULL

};

typedef void (*callback) (void);

void TaskDispatch(void);
void TaskInit(void);
int TaskFind(void (*func)());


struct _task{
    callback func;
    uint16_t delay;
    uint16_t repeat;
    uint32_t timeWorked;
};
struct _statistics{
    uint16_t id;
    uint32_t timeWorked;
    };


int wait_for_event(uint16_t *byte_addr, uint16_t mask, uint8_t expected_value, uint32_t timeout);
void delay_ms(uint32_t timeout);
void statisticsPrepare();




