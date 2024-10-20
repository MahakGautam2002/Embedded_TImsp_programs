//Btn as interrupt which remove debouncing
//config- interrupt enable - edge trigger

#include "ti_msp_dl_config.h"

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(GPIO_GRP_0_GPIOA_INT_IRQN);
    NVIC_EnableIRQ(GPIO_GRP_0_GPIOB_INT_IRQN);

    while (1)
    {
        __WFI();
    }
}

void GROUP1_IRQHandler(void)
{
    switch (DL_GPIO_getPendingInterrupt(GPIO_GRP_0_k1_18_PORT))
    {
    case DL_GPIO_IIDX_DIO18:
        DL_GPIO_togglePins(GPIO_GRP_0_led22_PORT,
        GPIO_GRP_0_led22_PIN | GPIO_GRP_0_led26_PIN);
        break;
    default:
        break;
    }

    switch (DL_GPIO_getPendingInterrupt(GPIO_GRP_0_k2_21_PORT))
    {
    case DL_GPIO_IIDX_DIO21:
        DL_GPIO_togglePins(GPIO_GRP_0_led22_PORT,
        GPIO_GRP_0_led26_PIN | GPIO_GRP_0_led27_PIN);
        break;
    default:
        break;
    }
}

