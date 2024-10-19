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

/* Libraries included */
#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

/* Function declaration */
void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void DL_GPIO_setPins_Custom(GPIO_Regs* gpio, uint32_t pins);
void DL_GPIO_clearPins_Custom(GPIO_Regs* gpio, uint32_t pins);

/* Macro definition */
#define GPIOB_BASE_custom (0x400A2000U)
GPIO_Regs *GPIOB_cumstom = ((GPIO_Regs*) GPIOB_BASE_custom);
#define GPIO_GRP_0_PORT  (GPIOB_cumstom)

#define POWER_STARTUP_DELAY (16)
#define CPUCLK_FREQ 32000000

#define GPIO_GRP_0_LED_1_IOMUX (IOMUX_PINCM57)
#define GPIO_GRP_0_LED_2_IOMUX (IOMUX_PINCM50)

#define GPIO_GRP_0_LED_1_PIN (DL_GPIO_PIN_26)
#define DL_GPIO_PIN_26 (0x04000000)

#define GPIO_GRP_0_LED_2_PIN (DL_GPIO_PIN_22)
#define DL_GPIO_PIN_22 (0x00400000)

#define DELAY (10000000)


int main(void)
{
    SYSCFG_DL_init();
    uint8_t uint8_t_colour = 0;
    while (1)
    {
        switch (uint8_t_colour)
        {
        case 0:
            DL_GPIO_setPins_Custom(GPIO_GRP_0_PORT, GPIO_GRP_0_LED_1_PIN);
            DL_GPIO_clearPins_Custom(GPIO_GRP_0_PORT, GPIO_GRP_0_LED_2_PIN);
            break;
        case 1:
            DL_GPIO_setPins_Custom(GPIO_GRP_0_PORT, GPIO_GRP_0_LED_2_PIN);
            DL_GPIO_clearPins_Custom(GPIO_GRP_0_PORT, GPIO_GRP_0_LED_1_PIN);
            break;
        default:
            break;
        }
        uint8_t_colour = (uint8_t_colour + 1) % 2;
        delay_cycles(DELAY);
    }
}

/* Function definition */
void DL_GPIO_clearPins_Custom(GPIO_Regs* gpio, uint32_t pins)
{
    gpio->DOUTCLR31_0 = pins;
}
void DL_GPIO_setPins_Custom(GPIO_Regs* gpio, uint32_t pins)
{
    gpio->DOUTSET31_0 = pins;
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

    DL_GPIO_initDigitalOutput(GPIO_GRP_0_LED_1_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_GRP_0_LED_2_IOMUX);

    DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_LED_1_PIN |
        GPIO_GRP_0_LED_2_PIN);
    DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_LED_1_PIN |
        GPIO_GRP_0_LED_2_PIN);

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
