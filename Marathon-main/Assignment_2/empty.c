/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* #include "ti_msp_dl_config.h" */

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>
#include<stdio.h>

/* Function Declaration */
void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void DL_GPIO_clearPins_Custom(GPIO_Regs *gpio, uint32_t pins);
void DL_GPIO_setPins_Custom(GPIO_Regs *gpio, uint32_t pins);
uint32_t DL_GPIO_readPins_Custom(GPIO_Regs *gpio, uint32_t pins);

/* Macro Definition */
#define GPIOA_BASE (0x400A0000U)
GPIO_Regs *GPIOA_BTN_1 = ((GPIO_Regs*) GPIOA_BASE);
#define GPIO_GRP_0_BTN_1_PORT (GPIOA_BTN_1)

#define DL_GPIO_PIN_18 (0x00040000)
#define GPIO_GRP_0_BTN_1_PIN (DL_GPIO_PIN_18)
#define GPIO_GRP_0_BTN_1_IOMUX (IOMUX_PINCM40)

#define GPIOB_BASE (0x400A2000U)
GPIO_Regs *GPIOB_BTN_2 = ((GPIO_Regs*) GPIOB_BASE);
#define GPIO_GRP_0_BTN_2_PORT  (GPIOB_BTN_2)

#define DL_GPIO_PIN_21 (0x00200000)
#define GPIO_GRP_0_BTN_2_PIN  (DL_GPIO_PIN_21)
#define GPIO_GRP_0_BTN_2_IOMUX (IOMUX_PINCM49)

#define GPIOB_BASE (0x400A2000U)
GPIO_Regs *GPIOB_LED_1 = ((GPIO_Regs*) GPIOB_BASE);
#define GPIO_GRP_0_LED_1_PORT (GPIOB_LED_1)

#define DL_GPIO_PIN_27 (0x08000000)
#define GPIO_GRP_0_LED_1_PIN (DL_GPIO_PIN_27)
#define GPIO_GRP_0_LED_1_IOMUX (IOMUX_PINCM58)

#define GPIOA_BASE (0x400A0000U)
GPIO_Regs *GPIOA_LED_2 = ((GPIO_Regs*) GPIOA_BASE);
#define GPIO_GRP_0_LED_2_PORT (GPIOA_LED_2)

#define DL_GPIO_PIN_0 (0x00000001)
#define GPIO_GRP_0_LED_2_PIN (DL_GPIO_PIN_0)
#define GPIO_GRP_0_LED_2_IOMUX (IOMUX_PINCM1)

#define DELAY (9000000)
#define POWER_STARTUP_DELAY (16)
#define CPUCLK_FREQ 32000000

//int main(void)
//{
//    SYSCFG_DL_init();
//    int8_t int8_t_btn_1_pressed_count = 0;
//    int8_t int8_t_btn_2_pressed_count = 0;
//    int8_t int8_t_btn_1_pressed = 0;
//    int8_t int8_t_btn_2_pressed = 0;
//
//    while (1)
//    {
//        if (DL_GPIO_readPins(GPIO_GRP_0_BTN_1_PORT, GPIO_GRP_0_BTN_1_PIN))
//        {
//            DL_GPIO_setPins(GPIO_GRP_0_LED_1_PORT, GPIO_GRP_0_LED_1_PIN);
//            delay_cycles(DELAY);
//            DL_GPIO_clearPins(GPIO_GRP_0_LED_1_PORT, GPIO_GRP_0_LED_2_PIN);
//            DL_GPIO_clearPins(GPIO_GRP_0_LED_2_PORT, GPIO_GRP_0_LED_2_PIN);
//            delay_cycles(DELAY);
//            DL_GPIO_setPins(GPIO_GRP_0_LED_2_PORT, GPIO_GRP_0_LED_2_PIN);
//            int8_t_btn_1_pressed_count++;
//            printf("Button 1 pressed : %d \n", int8_t_btn_1_pressed_count);
//        }
//
//        if(DL_GPIO_readPins(GPIO_GRP_0_BTN_2_PORT,GPIO_GRP_0_BTN_2_PIN))
//        {
//            DL_GPIO_clearPins(GPIO_GRP_0_LED_2_PORT, GPIO_GRP_0_LED_2_PIN);
//            delay_cycles(DELAY);
//            DL_GPIO_setPins(GPIO_GRP_0_LED_2_PORT, GPIO_GRP_0_LED_2_PIN);
//            DL_GPIO_setPins(GPIO_GRP_0_LED_1_PORT, GPIO_GRP_0_LED_1_PIN);
//            delay_cycles(DELAY);
//            DL_GPIO_clearPins(GPIO_GRP_0_LED_1_PORT, GPIO_GRP_0_LED_1_PIN);
//            int8_t_btn_2_pressed_count++;
//            printf("Button 1 pressed : %d \n", int8_t_btn_2_pressed_count);
//        }
//    }
//}

int main(void) {
    SYSCFG_DL_init();
    int8_t int8_t_btn_1_pressed_count = 0;
    int8_t int8_t_btn_2_pressed_count = 0;
    int8_t int8_t_btn_1_previous_state = 0;
    int8_t int8_t_btn_2_previous_state = 0;
    int8_t int8_t_btn_1_current_state =0;
    int8_t int8_t_btn_2_current_state =0;
    int8_t int8_t_btn_1_stable_state =0;
    int8_t int8_t_btn_2_stable_state =0;

    while (1) {
        if(DL_GPIO_readPins(GPIO_GRP_0_BTN_1_PORT, GPIO_GRP_0_BTN_1_PIN)){
            int8_t_btn_1_current_state = 1;
        }
        else{
            int8_t_btn_1_current_state = 0;
        }
        if(DL_GPIO_readPins(GPIO_GRP_0_BTN_2_PORT, GPIO_GRP_0_BTN_2_PIN)){
            int8_t_btn_2_current_state = 1;
        }
        else{
            int8_t_btn_2_current_state =0;
        }

        printf("Button 1 state: %d, Button 2 state: %d\n", int8_t_btn_1_current_state, int8_t_btn_2_current_state);

        if (int8_t_btn_1_current_state == int8_t_btn_1_previous_state) {
            if(int8_t_btn_1_current_state != int8_t_btn_1_stable_state){
            if (int8_t_btn_1_current_state) {
                printf("Button 1 pressed\n");
                DL_GPIO_setPins(GPIO_GRP_0_LED_1_PORT, GPIO_GRP_0_LED_1_PIN);
                delay_cycles(DELAY);
                DL_GPIO_clearPins(GPIO_GRP_0_LED_1_PORT, GPIO_GRP_0_LED_1_PIN);
                DL_GPIO_clearPins(GPIO_GRP_0_LED_2_PORT, GPIO_GRP_0_LED_2_PIN);
                delay_cycles(DELAY);
                DL_GPIO_setPins(GPIO_GRP_0_LED_2_PORT, GPIO_GRP_0_LED_2_PIN);
                int8_t_btn_1_pressed_count++;
                printf("Button 1 pressed count: %d\n", int8_t_btn_1_pressed_count);
            }
            int8_t_btn_1_stable_state=int8_t_btn_1_current_state;
        }
    }

        if (int8_t_btn_2_current_state == int8_t_btn_2_previous_state) {
            if(int8_t_btn_2_current_state != int8_t_btn_2_stable_state ){
            if (int8_t_btn_2_current_state) {
                printf("Button 2 pressed\n");
                DL_GPIO_clearPins(GPIO_GRP_0_LED_2_PORT, GPIO_GRP_0_LED_2_PIN);
                delay_cycles(DELAY);
                DL_GPIO_setPins(GPIO_GRP_0_LED_2_PORT, GPIO_GRP_0_LED_2_PIN);
                DL_GPIO_setPins(GPIO_GRP_0_LED_1_PORT, GPIO_GRP_0_LED_1_PIN);
                delay_cycles(DELAY);
                DL_GPIO_clearPins(GPIO_GRP_0_LED_1_PORT, GPIO_GRP_0_LED_1_PIN);
                int8_t_btn_2_pressed_count++;
                printf("Button 2 pressed count: %d\n", int8_t_btn_2_pressed_count);
            }
            int8_t_btn_2_stable_state = int8_t_btn_2_current_state;
        }
        }
        int8_t_btn_1_previous_state = int8_t_btn_1_current_state;
        int8_t_btn_2_previous_state = int8_t_btn_2_current_state;
    }
}

/* Function Definition */

void DL_GPIO_clearPins_Custom(GPIO_Regs *gpio, uint32_t pins)
{
    gpio->DOUTCLR31_0 = pins;
}

void DL_GPIO_setPins_Custom(GPIO_Regs *gpio, uint32_t pins)
{
    gpio->DOUTSET31_0 = pins;
}
uint32_t DL_GPIO_readPins_Custom(GPIO_Regs *gpio, uint32_t pins)
{
    return (gpio->DIN31_0 & pins);
}

void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_SYSCTL_init();
}

void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    delay_cycles(POWER_STARTUP_DELAY);
}

void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initDigitalInput(GPIO_GRP_0_BTN_1_IOMUX);

    DL_GPIO_initDigitalInputFeatures(GPIO_GRP_0_BTN_2_IOMUX,
                                     DL_GPIO_INVERSION_ENABLE,
                                     DL_GPIO_RESISTOR_PULL_UP,
                                     DL_GPIO_HYSTERESIS_DISABLE,
                                     DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalOutput(GPIO_GRP_0_LED_1_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_GRP_0_LED_2_IOMUX);

    DL_GPIO_setPins(GPIOA, GPIO_GRP_0_LED_2_PIN);
    DL_GPIO_enableOutput(GPIOA, GPIO_GRP_0_LED_2_PIN);
    DL_GPIO_clearPins(GPIOB, GPIO_GRP_0_LED_1_PIN);
    DL_GPIO_enableOutput(GPIOB, GPIO_GRP_0_LED_1_PIN);

}

void SYSCFG_DL_SYSCTL_init(void)
{

    //Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    /* Set default configuration */
    DL_SYSCTL_disableHFXT();
    DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_1);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);

}

