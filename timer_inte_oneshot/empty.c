//timer- as interrupt

//LCKL, 20000ms== 2sec

//one shot down- zero intr
//one shot up- load intr

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

//void TIMER_0_INST_IRQHandler(void)
//{
//    DL_GPIO_togglePins(GPIO_GRP_0_led22_PORT, GPIO_GRP_0_led22_PIN); // blue led set
//
//}

//one shot up
void TIMER_0_INST_IRQHandler(void)
{
    switch (DL_TimerA_getPendingInterrupt(TIMER_0_INST))
    {
    case DL_TIMER_IIDX_LOAD:
        DL_GPIO_togglePins(GPIO_GRP_0_led22_PORT, GPIO_GRP_0_led22_PIN); // blue led set
        TIMER_0_INST->COUNTERREGS.CTR = TIMER_0_INST_LOAD_VALUE;
        DL_TimerA_startCounter(TIMER_0_INST);
        break;
    default:
        break;
    }
}

////one shot down
//void TIMER_0_INST_IRQHandler(void)
//{
//    switch(DL_TimerA_getPendingInterrupt(TIMER_0_INST)){
//    case DL_TIMER_IIDX_ZERO:
//    DL_GPIO_togglePins(GPIO_GRP_0_led22_PORT, GPIO_GRP_0_led22_PIN); // blue led set
//    DL_TimerA_stopCounter(TIMER_0_INST);
//    DL_TimerA_startCounter(TIMER_0_INST);
//        break;
//    default:
//        break;
//    }
//}

