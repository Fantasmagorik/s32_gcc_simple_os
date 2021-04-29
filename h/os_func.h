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
};

int wait_for_event(uint16_t *byte_addr, uint16_t mask, uint8_t expected_value, uint32_t timeout);
void delay_ms(uint32_t timeout);




