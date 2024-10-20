//#include "ti_msp_dl_config.h"
//
//int main(void)
//{
//    SYSCFG_DL_init();
//    uint8_t choice = 0;
//
//    while (1)
//    {
//        switch (choice)
//        {
//        case 0:
//            DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_blue_PIN);
//            DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_red_PIN);
//            break;
//        case 1:
//            DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_blue_PIN);
//            DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_red_PIN);
//            break;
//        default:
//            break;
//        }
//        choice=(choice+1)%2;
//        delay_cycles(32000000);
//    }
//}
//******************************************

//uisng timer police light
#include "ti_msp_dl_config.h"

volatile uint32_t timerCounter = 0;

#define LED1_ON (50)
#define LED2_ON (50)

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    DL_TimerA_startCounter(TIMER_0_INST);

    while (1) {
        if(timerCounter % (LED1_ON + LED2_ON) < LED1_ON) {
            DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_blue_PIN);
            DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_red_PIN);
        }
        else
        {
            DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_blue_PIN);
            DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_red_PIN);
        }
    }
}

void TIMER_0_INST_IRQHandler(void) {
    timerCounter++;
}
