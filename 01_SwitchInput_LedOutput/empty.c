/*
 * PS1:
 * Switch Input and LED Output Interface
 *
 * By Mahak Gautam - 220098
 */

#include "ti_msp_dl_config.h"

/*
 * SwitchPress function is used to ON LED when switch is pressed
 * parameters- Switch Port & Pin , LED Port & Pin
 */
void SwitchPress(GPIO_Regs *Switch_Port, uint32_t Switch_Pin,
                 GPIO_Regs *LED_Port, uint32_t LED_Pin);


/*
 * SwitchReleased function is used to OFF LED when switch is released
 * parameters- Switch Port & Pin , LED Port & Pin
 */
void SwitchReleased(GPIO_Regs *Switch_Port, uint32_t Switch_Pin,
                    GPIO_Regs *LED_Port, uint32_t LED_Pin);


int main(void)

{
    SYSCFG_DL_init();

    while (1)
    {
//        switch1 press - LED1 ON
        SwitchPress(GPIO_GRP_0_SW1_PORT, GPIO_GRP_0_SW1_PIN,
        GPIO_GRP_0_LED1_PORT,
                    GPIO_GRP_0_LED1_PIN);
//        switch1 released - LED1 off
        SwitchReleased(GPIO_GRP_0_SW1_PORT, GPIO_GRP_0_SW1_PIN,
        GPIO_GRP_0_LED1_PORT,
                       GPIO_GRP_0_LED1_PIN);


//        switch2 press - LED2 ON
        SwitchPress(GPIO_GRP_0_SW2_PORT, GPIO_GRP_0_SW2_PIN,
        GPIO_GRP_0_LED2_PORT,
                    GPIO_GRP_0_LED2_PIN);
//        switch2 released - LED2 OFF
        SwitchReleased(GPIO_GRP_0_SW2_PORT, GPIO_GRP_0_SW2_PIN,
        GPIO_GRP_0_LED2_PORT,
                       GPIO_GRP_0_LED2_PIN);

    }
}


void SwitchPress(GPIO_Regs *Switch_Port, uint32_t Switch_Pin,
                 GPIO_Regs *LED_Port, uint32_t LED_Pin)
{
    if (DL_GPIO_readPins(Switch_Port, Switch_Pin)) // switch press is read
    {
        DL_GPIO_setPins(LED_Port, LED_Pin); // set LED
    }
}

void SwitchReleased(GPIO_Regs *Switch_Port, uint32_t Switch_Pin,
                    GPIO_Regs *LED_Port, uint32_t LED_Pin)
{
    if (!DL_GPIO_readPins(Switch_Port, Switch_Pin)) // switch press is not read (switch released)
    {
        DL_GPIO_clearPins(LED_Port, LED_Pin);  // clear LED
    }
}
