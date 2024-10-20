//timer - preodic down- btn press cause debounce condition
//so removing debouncing

#include "ti_msp_dl_config.h"

volatile uint8_t count = 0;
volatile uint8_t delay = 500;

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    DL_TimerA_startCounter(TIMER_0_INST);
    NVIC_EnableIRQ(GPIO_GRP_0_GPIOA_INT_IRQN);
    NVIC_EnableIRQ(GPIO_GRP_0_GPIOB_INT_IRQN);

    while (1)
    {
        __WFI();

        if (delay == count)
        {
            DL_GPIO_togglePins(GPIO_GRP_0_led22_PORT,
            GPIO_GRP_0_led22_PIN | GPIO_GRP_0_led26_PIN);
            printf("count: %d \n", delay);
            count = 0;
        }
        if (count <= 100 || count >= 1000)
        {
            count = 500;
        }

    }
}
    void TIMER_0_INST_IRQHandler(void)
    {
        count += 10;

    }

    void GROUP1_IRQHandler(void)
    {
        switch (DL_GPIO_getPendingInterrupt(GPIOB))
        {
        case DL_GPIO_IIDX_DIO21:
            if (delay < 1000)
            {

                delay += 100;
            }
            break;
        default:
            break;
        }
        switch (DL_GPIO_getPendingInterrupt(GPIOB))
        {
        case DL_GPIO_IIDX_DIO18:
            if (delay > 100)
            {

                delay -= 100;
            }
            break;
        default:
            break;
        }
    }

