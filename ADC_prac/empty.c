//divide by 8
//intr- mem0 result load
//pinmux PA27 and one on 3.3v =4096 & GRD=0

#include "ti_msp_dl_config.h"
#include "stdio.h"
volatile uint8_t flag = 0;
uint32_t result = 0;

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    while (1)
    {
        DL_ADC12_startConversion(ADC12_0_INST);
        if (flag == 0)
        {
            __WFI();
        }
        result = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);
        printf("Digital:%d \n", result);
        DL_ADC12_disableConversions(ADC12_0_INST);
        flag = 0;
        DL_ADC12_enableConversions(ADC12_0_INST);
    }
}

void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST))
    {
    case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
        flag = 1;
        break;
    default:
        break;
    }
}
