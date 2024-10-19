//timer - preodic down- btn press cause debounce condition
//so removing debouncing

#include "ti_msp_dl_config.h"

volatile uint8_t currentState1 = 0;
uint8_t previousState1 = 0;
uint8_t stableStable1 = 0;

volatile uint8_t currentState2 = 0;
uint8_t previousState2 = 0;
uint8_t stableStable2 = 0;

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    DL_TimerA_startCounter(TIMER_0_INST);

    while (1)
    {
        __WFI();

//        key1
        if (currentState1 == 1 && previousState1 == 1 && stableStable1 == 0)
        {
            DL_GPIO_togglePins(GPIO_GRP_0_led22_PORT,
            GPIO_GRP_0_led22_PIN | GPIO_GRP_0_led26_PIN); // pink led toggle
            stableStable1 = 1;
        }
        else if (currentState1 == 0 && previousState1 == 0
                && stableStable1 == 1)
        {
            stableStable1 = 0;
        }
        previousState1 = currentState1;


//        key2
        if(currentState2==1 && previousState2==1 && stableStable2==0){
            DL_GPIO_togglePins(GPIO_GRP_0_led22_PORT,
                    GPIO_GRP_0_led26_PIN | GPIO_GRP_0_led27_PIN); // yellow led toggle
            stableStable2=1;
        }
        else if(currentState2==0 && previousState2==0 && stableStable2==1){
            stableStable2=0;
        }
        previousState2=currentState2;
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
