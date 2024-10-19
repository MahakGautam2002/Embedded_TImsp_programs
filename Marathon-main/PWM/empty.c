#include "ti_msp_dl_config.h"

int main(void)
{
    SYSCFG_DL_init();
    DL_TimerG_startCounter(PWM_0_INST);
    while (1) {
        for(int i=200000;i>=0;i-=10000){
            DL_TimerG_setCaptureCompareValue(PWM_0_INST,i,DL_TIMER_CC_0_INDEX);

            delay_cycles(16000000);
        }
        for(int i=0;i<=200000;i+=10000){
            DL_TimerG_setCaptureCompareValue(PWM_0_INST,i,DL_TIMER_CC_0_INDEX);

            delay_cycles(16000000);
        }

    }
}
