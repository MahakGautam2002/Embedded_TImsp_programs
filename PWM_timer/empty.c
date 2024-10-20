//period count - 200000
//pinmux - timer12 = PB13 PA31
//no interrupt

//  less i value - more brightness

#include "ti_msp_dl_config.h"

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(PWM_0_INST_INT_IRQN);
    DL_TimerG_startCounter(PWM_0_INST);

    int i = 50000;
    while (1)
    {
        if (i < 200000 && i > 20000)

        {
            if (DL_GPIO_readPins(GPIO_GRP_0_k1_PORT, GPIO_GRP_0_k1_PIN))
            {
                i += 10000;
                DL_TimerG_setCaptureCompareValue(PWM_0_INST, i,
                                                 DL_TIMER_CC_0_INDEX);
                delay_cycles(16000000);
                printf("i 1: %d \n", i);
            }

            if (DL_GPIO_readPins(GPIO_GRP_0_k2_PORT, GPIO_GRP_0_k2_PIN))
            {
                i -= 10000;
                DL_TimerG_setCaptureCompareValue(PWM_0_INST, i,
                                                 DL_TIMER_CC_0_INDEX);
                delay_cycles(16000000);
                printf("i 2: %d \n", i);
            }

        }
        else
        {
            printf("value beyond limit \n");
            i = 50000;
        }
        //        for(uint32_t i=200000;i>=0;i-=10000){
//               DL_TimerG_setCaptureCompareValue(PWM_0_INST,i,DL_TIMER_CC_0_INDEX);
//               delay_cycles(16000000);
//        }
//
//        for(uint32_t i=0;i<=200000;i+=10000){
//               DL_TimerG_setCaptureCompareValue(PWM_0_INST,i,DL_TIMER_CC_0_INDEX);
//               delay_cycles(16000000);
//        }

    }
}

//******************************

//period count - 200000
//pinmux - timer12 = PB13 PA31
//no interrupt

//  less i value - more brightness

//#include "ti_msp_dl_config.h"
//
//int main(void)
//{
//    SYSCFG_DL_init();
//    NVIC_EnableIRQ(PWM_0_INST_INT_IRQN);
//    DL_TimerG_startCounter(PWM_0_INST);
//
//    while (1)
//    {
//        for (uint32_t i = 200000; i >= 0; i -= 10000)
//        {
//            DL_TimerG_setCaptureCompareValue(PWM_0_INST, i,
//                                             DL_TIMER_CC_0_INDEX);
//            delay_cycles(16000000);
//        }
//
//        for (uint32_t i = 0; i <= 200000; i += 10000)
//        {
//            DL_TimerG_setCaptureCompareValue(PWM_0_INST, i,
//                                             DL_TIMER_CC_0_INDEX);
//            delay_cycles(16000000);
//        }
//
//    }
//}
//
