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

#include "ti_msp_dl_config.h"
#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

/* Function Declaration */
void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void DL_GPIO_setPins_Custom(GPIO_Regs* gpio, uint32_t pins);
void DL_GPIO_clearPins_Custom(GPIO_Regs* gpio, uint32_t pins);
uint32_t DL_GPIO_readPins_Custom(GPIO_Regs* gpio, uint32_t pins);

/* Macro Definition */
#define POWER_STARTUP_DELAY (16)
#define CPUCLK_FREQ 32000000
DL_TimerA_backupConfig gTIMER_0Backup;
#define TIMER_0_INST_LOAD_VALUE (327U)

#define TIMA0_BASE (0x40860000U)
GPTIMER_Regs  * TIMA0_CUSTOM = ((GPTIMER_Regs *) TIMA0_BASE);
#define TIMER_0_INST (TIMA0_CUSTOM)

#define TIMER_0_INST_IRQHandler TIMA0_IRQHandler
#define TIMER_0_INST_INT_IRQN (TIMA0_INT_IRQn)

#define GPIOB_BASE (0x400A2000U)
GPIO_Regs * GPIOB_CUSTOM = ((GPIO_Regs *) GPIOB_BASE);
#define GPIO_GRP_0_LED_1_PORT (GPIOB_CUSTOM)

#define DL_GPIO_PIN_27  (0x08000000)
#define GPIO_GRP_0_LED_1_PIN (DL_GPIO_PIN_27)
#define GPIO_GRP_0_LED_1_IOMUX (IOMUX_PINCM58)

#define GPIOA_BASE (0x400A0000U)
GPIO_Regs * GPIOA_CUSTOM  = ((GPIO_Regs *) GPIOA_BASE);
#define GPIO_GRP_0_BTN_1_PORT (GPIOA_CUSTOM)

#define DL_GPIO_PIN_18 (0x00040000)
#define GPIO_GRP_0_BTN_1_PIN (DL_GPIO_PIN_18)
#define GPIO_GRP_0_BTN_1_IOMUX (IOMUX_PINCM40)
#define LED_ON 800

/* Variable declaration */
uint8_t uint_8_current_btn_state = 0;
uint8_t uint_8_stable_btn_state = 0;
uint8_t uint_8_previous_btn_state = 0;

uint32_t delay_counter =0;
uint32_t on_count =0;



int button_1_pressed(void);

int button_1_pressed(){
    if(uint_8_previous_btn_state==uint_8_current_btn_state){
        if(uint_8_current_btn_state !=uint_8_stable_btn_state){
            if(uint_8_current_btn_state){
                uint_8_stable_btn_state =1;
                return 1;
            }
            else{
                uint_8_stable_btn_state =0;
            }
        }
    }
    return 0;
}

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    DL_TimerA_startCounter(TIMER_0_INST);

    while (1) {
        __WFI();
        if(button_1_pressed()){
            on_count =0;
            DL_GPIO_setPins_Custom(GPIO_GRP_0_LED_1_PORT, GPIO_GRP_0_LED_1_PIN);
            on_count = delay_counter + LED_ON;
        }
        if(on_count == delay_counter){
            DL_GPIO_clearPins(GPIO_GRP_0_LED_1_PORT, GPIO_GRP_0_LED_1_PIN);
            on_count =0;
        }
    }
}

/* Timer Function */
void TIMER_0_INST_IRQHandler(void){
    delay_counter++;
    uint_8_previous_btn_state = uint_8_current_btn_state;
    if(DL_GPIO_readPins_Custom(GPIO_GRP_0_BTN_1_PORT,GPIO_GRP_0_BTN_1_PIN)){
        (uint_8_current_btn_state =1);
    }
    else{
        (uint_8_current_btn_state =0);
    }

}

/* Function Definition */

void DL_GPIO_clearPins_Custom(GPIO_Regs* gpio, uint32_t pins)
{
    gpio->DOUTCLR31_0 = pins;
}
void DL_GPIO_setPins_Custom(GPIO_Regs* gpio, uint32_t pins)
{
    gpio->DOUTSET31_0 = pins;
}
uint32_t DL_GPIO_readPins_Custom(GPIO_Regs* gpio, uint32_t pins)
{
    return (gpio->DIN31_0 & pins);
}


void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_TIMER_0_init();
    /* Ensure backup structures have no valid state */
    gTIMER_0Backup.backupRdy = false;

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

    DL_GPIO_initDigitalOutput(GPIO_GRP_0_LED_1_IOMUX);

    DL_GPIO_initDigitalInput(GPIO_GRP_0_BTN_1_IOMUX);

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
 * TIMER_0_INST_LOAD_VALUE = (10 ms * 32768 Hz) - 1
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

