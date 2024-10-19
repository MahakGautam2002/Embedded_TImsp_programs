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

//#include "ti_msp_dl_config.h"
#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>
#include<stdio.h>

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_TIMER_0_init(void);

#define POWER_STARTUP_DELAY (16)
#define CPUCLK_FREQ 32000000

DL_TimerA_backupConfig gTIMER_0Backup;
#define TIMA0_BASE_CUSTOM (0x40860000U)
GPTIMER_Regs * TIMA0_CUSTOM = ((GPTIMER_Regs *) TIMA0_BASE_CUSTOM);
#define TIMER_0_INST (TIMA0_CUSTOM)

#define TIMER_0_INST_IRQHandler TIMA0_IRQHandler
#define TIMER_0_INST_INT_IRQN (TIMA0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE (32767U)

/* Defines for BTN_1: GPIOA.18 with pinCMx 40 on package pin 11 */
#define GPIOA_BASE_CUSTOM (0x400A0000U)
GPIO_Regs* GPIOA_CUSTOM = ((GPIO_Regs *) GPIOA_BASE_CUSTOM);
#define GPIO_GRP_0_BTN_1_PORT  (GPIOA_CUSTOM)
// pins affected by this interrupt request:["BTN_1"]
#define GPIO_GRP_0_GPIOA_INT_IRQN (GPIOA_INT_IRQn)
#define DL_INTERRUPT_GROUP1_IIDX_GPIOA (CPUSS_INT_GROUP_IIDX_STAT_INT0)
#define GPIO_GRP_0_GPIOA_INT_IIDX (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_GRP_0_BTN_1_IIDX (DL_GPIO_IIDX_DIO18)
#define DL_GPIO_PIN_18 (0x00040000)
#define GPIO_GRP_0_BTN_1_PIN  (DL_GPIO_PIN_18)
#define GPIO_GRP_0_BTN_1_IOMUX (IOMUX_PINCM40)
/* Defines for BTN_2: GPIOB.21 with pinCMx 49 on package pin 20 */
#define GPIOB_BASE_CUSTOM (0x400A2000U)
GPIO_Regs * GPIOB_CUSTOM = ((GPIO_Regs *) GPIOB_BASE_CUSTOM);
#define GPIO_GRP_0_BTN_2_PORT (GPIOB_CUSTOM)
// pins affected by this interrupt request:["BTN_2"]
#define GPIO_GRP_0_GPIOB_INT_IRQN  (GPIOB_INT_IRQn)
#define DL_INTERRUPT_GROUP1_IIDX_GPIOB (CPUSS_INT_GROUP_IIDX_STAT_INT1)
#define GPIO_GRP_0_GPIOB_INT_IIDX  (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define GPIO_GRP_0_BTN_2_IIDX (DL_GPIO_IIDX_DIO21)
#define DL_GPIO_PIN_21 (0x00200000)
#define GPIO_GRP_0_BTN_2_PIN (DL_GPIO_PIN_21)
#define GPIO_GRP_0_BTN_2_IOMUX (IOMUX_PINCM49)

// for timer and count
//uint32_t uint32_t_timerValue = 0;
uint32_t uint32_t_capturedValue = 0;
uint8_t uint8_t_button1Pressed = 0;
uint8_t uint8_t_button2Pressed = 0;

int main(void)
{
    // initial configuration
    SYSCFG_DL_init();
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    NVIC_EnableIRQ(GPIO_GRP_0_GPIOA_INT_IRQN);
    NVIC_EnableIRQ(GPIO_GRP_0_GPIOB_INT_IRQN);

    while (1)
    {
        __WFI();

        if (uint8_t_button2Pressed)
        {
            uint8_t_button2Pressed = 0;
            printf("Captured Timer Value: %d\n", uint32_t_capturedValue);
        }
    }
}

void GROUP1_IRQHandler(void)
{
    uint32_t uint32_t_gpioA = DL_GPIO_getEnabledInterruptStatus(GPIOA, GPIO_GRP_0_BTN_1_PIN);
    uint32_t uint32_t_gpioB = DL_GPIO_getEnabledInterruptStatus(GPIOB, GPIO_GRP_0_BTN_2_PIN);

    if (uint32_t_gpioA)
    {
//        uint32_t_timerValue = 0;
        uint8_t_button1Pressed = 1;
        printf("Timer Started\n");
        DL_TimerA_startCounter(TIMER_0_INST);
        DL_GPIO_clearInterruptStatus(GPIOA, GPIO_GRP_0_BTN_1_PIN);
    }

    if (uint32_t_gpioB)
    {
//        capturedValue = timerValue;
        uint32_t_capturedValue = DL_TimerA_getTimerCount(TIMER_0_INST);
        uint8_t_button2Pressed = 1;
        DL_GPIO_clearInterruptStatus(GPIOB, GPIO_GRP_0_BTN_2_PIN);
    }
}

void TIMER_0_INST_IRQHandler(void)
{
    switch (DL_TimerA_getPendingInterrupt(TIMER_0_INST))
    {
    case DL_TIMERA_IIDX_ZERO:
        if (uint8_t_button1Pressed)
        {
//            uint32_t_timerValue++;
            uint8_t_button1Pressed =0;
        }
        break;
    default:
        break;
    }
}





void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_TIMER_0_init();
    /* Ensure backup structures have no valid state */
    gTIMER_0Backup.backupRdy    = false;

}
void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_TimerA_reset(TIMER_0_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_TimerA_enablePower(TIMER_0_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initDigitalInput(GPIO_GRP_0_BTN_1_IOMUX);

    DL_GPIO_initDigitalInputFeatures(GPIO_GRP_0_BTN_2_IOMUX,
         DL_GPIO_INVERSION_ENABLE, DL_GPIO_RESISTOR_PULL_UP,
         DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_setUpperPinsPolarity(GPIOA, DL_GPIO_PIN_18_EDGE_RISE);
    DL_GPIO_clearInterruptStatus(GPIOA, GPIO_GRP_0_BTN_1_PIN);
    DL_GPIO_enableInterrupt(GPIOA, GPIO_GRP_0_BTN_1_PIN);
    DL_GPIO_setUpperPinsPolarity(GPIOB, DL_GPIO_PIN_21_EDGE_RISE);
    DL_GPIO_clearInterruptStatus(GPIOB, GPIO_GRP_0_BTN_2_PIN);
    DL_GPIO_enableInterrupt(GPIOB, GPIO_GRP_0_BTN_2_PIN);

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
    /* INT_GROUP1 Priority */
    NVIC_SetPriority(GPIOB_INT_IRQn, 0);

}



/*
 * Timer clock configuration to be sourced by LFCLK /  (32768 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   32768 Hz = 32768 Hz / (1 * (0 + 1))
 */
static const DL_TimerA_ClockConfig gTIMER_0ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_LFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 0U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_0_INST_LOAD_VALUE = (1000 ms * 32768 Hz) - 1
 */
static const DL_TimerA_TimerConfig gTIMER_0TimerConfig = {
    .period     = TIMER_0_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC_UP,
    .startTimer = DL_TIMER_STOP,
};

void SYSCFG_DL_TIMER_0_init(void) {

    DL_TimerA_setClockConfig(TIMER_0_INST,
        (DL_TimerA_ClockConfig *) &gTIMER_0ClockConfig);

    DL_TimerA_initTimerMode(TIMER_0_INST,
        (DL_TimerA_TimerConfig *) &gTIMER_0TimerConfig);
    DL_TimerA_enableInterrupt(TIMER_0_INST , DL_TIMERA_INTERRUPT_ZERO_EVENT);
    DL_TimerA_enableClock(TIMER_0_INST);





}

