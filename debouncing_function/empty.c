//timer - preodic down- btn press cause debounce condition
//so removing debouncing
//create function to reduce code in main

#include "ti_msp_dl_config.h"

volatile uint8_t currentState1 = 0;
uint8_t previousState1 = 0;
uint8_t stableState1 = 0;

volatile uint8_t currentState2 = 0;
uint8_t previousState2 = 0;
uint8_t stableState2 = 0;

static void debounceKey(uint8_t *currentState, uint8_t *previousState,
                        uint8_t *stableState, GPIO_Regs *port, uint32_t pin);

static inline void toggle(GPIO_Regs *port, uint32_t pin);

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    DL_TimerA_startCounter(TIMER_0_INST);

    while (1)
    {
        __WFI();

//        key1
        debounceKey(&currentState1, &previousState1, &stableState1, GPIO_GRP_0_led22_PORT,
                    GPIO_GRP_0_led22_PIN);

//        key2
        debounceKey(&currentState2, &previousState2, &stableState2, GPIO_GRP_0_led22_PORT,
            GPIO_GRP_0_led26_PIN);
    }
}

void TIMER_0_INST_IRQHandler(void)
{
//        port A- k1- pin18
    if (DL_GPIO_readPins(GPIO_GRP_0_k1_18_PORT, GPIO_GRP_0_k1_18_PIN))
    {
        currentState1 = 1;
    }
    else
    {
        currentState1 = 0;
    }
//        port B- k2-pin21
    if (DL_GPIO_readPins(GPIO_GRP_0_k2_21_PORT, GPIO_GRP_0_k2_21_PIN))
    {
        currentState2 = 1;
    }
    else
    {
        currentState2 = 0;
    }
}

static void debounceKey(uint8_t *currentState, uint8_t *previousState,
                        uint8_t *stableState, GPIO_Regs *port, uint32_t pin)
{
   if (*currentState == 1 && *previousState == 1 && *stableState == 0)
    {
        toggle(port,pin);
        *stableState = 1;
    }
    else if (*currentState == 0 && *previousState == 0 && *stableState == 1)
    {
        *stableState = 0;
    }
    *previousState = *currentState;
}

static inline void toggle(GPIO_Regs *port,uint32_t pin){
    DL_GPIO_togglePins(port,pin);
}
