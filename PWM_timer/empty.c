//period count - 200000
//pinmux - timer12 = PB13 PA31
//no interrupt
#include "ti_msp_dl_config.h"

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(PWM_0_INST_INT_IRQN);
    DL_TimerG_startCounter(PWM_0_INST);

    while (1) {
//        if(DL_GPIO_readPins(GPIO_GRP_0_PORT,GPIO_GRP_0_k1_PIN)){
//            DL_TimerG_setCaptureCompareValue(PWM_0_INST,5000,DL_TIMER_CC_0_INDEX);
//            delay_cycles(16000000);
//        }
        for(uint32_t i=200000;i>=0;i-=10000){
               DL_TimerG_setCaptureCompareValue(PWM_0_INST,i,DL_TIMER_CC_0_INDEX);
               delay_cycles(16000000);
        }

        for(uint32_t i=0;i<=200000;i+=10000){
               DL_TimerG_setCaptureCompareValue(PWM_0_INST,i,DL_TIMER_CC_0_INDEX);
               delay_cycles(16000000);
        }

    }
}
