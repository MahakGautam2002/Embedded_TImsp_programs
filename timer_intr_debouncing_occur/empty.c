//timer - preodic down- btn press cause debounce condition
#include "ti_msp_dl_config.h"

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    DL_TimerA_startCounter(TIMER_0_INST);

    while (1)
    {
        __WFI();
    }
}

void TIMER_0_INST_IRQHandler(void)
{
    //        port A- k1- pin18
    if (DL_GPIO_readPins(GPIO_GRP_0_k1_18_PORT, GPIO_GRP_0_k1_18_PIN))
    {
        DL_GPIO_togglePins(GPIO_GRP_0_led22_PORT,
        GPIO_GRP_0_led22_PIN | GPIO_GRP_0_led26_PIN); // pink led toggle
    }
    //        port B- k2-pin21
    if (DL_GPIO_readPins(GPIO_GRP_0_k2_21_PORT, GPIO_GRP_0_k2_21_PIN))
    {
        DL_GPIO_togglePins(GPIO_GRP_0_led22_PORT,
        GPIO_GRP_0_led26_PIN | GPIO_GRP_0_led27_PIN); // yellow led toggle
    }
}
