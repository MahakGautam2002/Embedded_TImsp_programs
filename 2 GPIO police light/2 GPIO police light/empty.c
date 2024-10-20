// police light and left to right

#include "ti_msp_dl_config.h"
#include "stdio.h"

int main(void){
    SYSCFG_DL_init();
    while(1){
        //DL_GPIO_togglePins(GPIO_GRP_0_LED1_PORT, GPIO_GRP_0_LED1_PIN);
        GPIOB->DOUTTGL31_0 = DL_GPIO_PIN_22;
        delay_cycles(3000000);
        // if more swipe required increase the first delay that is the delay after toggling the first led
        //DL_GPIO_togglePins(GPIO_GRP_0_LED2_PORT, GPIO_GRP_0_LED2_PIN) ;
        GPIOA->DOUTTGL31_0 = DL_GPIO_PIN_0;
        delay_cycles(3000000);

    }
}
