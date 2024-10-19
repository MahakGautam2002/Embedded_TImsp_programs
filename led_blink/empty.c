//polling method of running code- without any interrupt
#include "ti_msp_dl_config.h"

int main(void)
{
    SYSCFG_DL_init();

    while (1)
    {
//        port B
//        DL_GPIO_setPins(GPIO_GRP_0_led22_PORT, GPIO_GRP_0_led22_PIN); // blue led set
//        delay_cycles(32000000); // 1sec softwre delay
//        DL_GPIO_clearPins(GPIO_GRP_0_led22_PORT, GPIO_GRP_0_led22_PIN); // blue led clear
//
//        DL_GPIO_setPins(GPIO_GRP_0_led26_PORT, GPIO_GRP_0_led26_PIN); // red led set
//        delay_cycles(32000000);
//        DL_GPIO_clearPins(GPIO_GRP_0_led26_PORT, GPIO_GRP_0_led26_PIN); // red led clear
//
//        DL_GPIO_setPins(GPIO_GRP_0_led27_PORT, GPIO_GRP_0_led27_PIN); // green led set
//        delay_cycles(32000000);
//        DL_GPIO_clearPins(GPIO_GRP_0_led27_PORT, GPIO_GRP_0_led27_PIN); // green led clear


//        port A
        DL_GPIO_togglePins(GPIO_GRP_0_led0_PORT, GPIO_GRP_0_led0_PIN); // red led toggle
        delay_cycles(32000000);

    }
}
