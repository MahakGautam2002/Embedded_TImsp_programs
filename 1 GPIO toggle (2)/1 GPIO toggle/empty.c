#include "ti_msp_dl_config.h"
#include "stdio.h"

volatile uint16_t counter = 0;
volatile uint16_t delay = 500;
const uint16_t MIN_DELAY = 100;  // Prevent delay from going too low

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(TIMER_10ms_INST_INT_IRQN);
    DL_Timer_startCounter(TIMER_10ms_INST);
    NVIC_EnableIRQ(GPIO_GRP_0_GPIOB_INT_IRQN);
    NVIC_EnableIRQ(GPIO_GRP_0_GPIOA_INT_IRQN);

    while (1) {
        __WFI();  // Wait for interrupt

        if (delay == counter) {
            DL_GPIO_togglePins(GPIOB, GPIO_GRP_0_LED_PIN);
            printf("%d \n", delay);
            counter = 0;  // Reset the counter after toggle
        }
    }
}

// Timer interrupt: increments counter by 10 every 10ms
void TIMER_10ms_INST_IRQHandler(void) {
    counter += 10;
}

// GPIO interrupt handler
void GROUP1_IRQHandler(void) {
    // Handle GPIOB interrupt
    switch (DL_GPIO_getPendingInterrupt(GPIOB)) {
        case DL_GPIO_IIDX_DIO21:
            delay += 100;  // Increase delay by 100
            break;
        default:
            break;
    }

    // Handle GPIOA interrupt
    switch (DL_GPIO_getPendingInterrupt(GPIOA)) {
        case DL_GPIO_IIDX_DIO18:
            if (delay > MIN_DELAY) {  // Prevent delay from going below the minimum threshold
                delay -= 100;  // Decrease delay by 100
            }
            break;
        default:
            break;
    }
}
