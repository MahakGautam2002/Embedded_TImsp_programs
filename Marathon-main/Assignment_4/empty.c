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
#include<stdio.h>
int btn_1_isPressed(void);
int btn_2_isPressed(void);

#define DELAY (10)
#define MAX (50)
#define MIN (10)
uint16_t uint16_t_btn_1_current_state = 0;
uint16_t uint16_t_btn_1_previous_state = 0;
uint16_t uint16_t_btn_1_stable_state = 0;
bool bool_btn_1_pressed = 0;

uint16_t uint16_t_btn_2_current_state = 0;
uint16_t uint16_t_btn_2_previous_state = 0;
uint16_t uint16_t_btn_2_stable_state = 0;
bool bool_btn_2_pressed = 0;

uint16_t uint16_t_timer_count =10;
uint16_t uint16_t_timer_delay =10;

int btn_1_isPressed(){
    if(uint16_t_btn_1_current_state==uint16_t_btn_1_previous_state){
        if(uint16_t_btn_1_current_state !=uint16_t_btn_1_stable_state){
            if(uint16_t_btn_1_current_state){
                uint16_t_btn_1_stable_state =1;
                return 1;
            }
            else{
                uint16_t_btn_1_stable_state =0;
            }
        }
    }
    return 0;
}

int btn_2_isPressed(){
    if(uint16_t_btn_2_current_state==uint16_t_btn_2_previous_state){
        if(uint16_t_btn_2_current_state !=uint16_t_btn_2_stable_state){
            if(uint16_t_btn_2_current_state){
                uint16_t_btn_2_stable_state =1;
                return 1;
            }
            else{
                uint16_t_btn_2_stable_state =0;
            }
        }
    }
    return 0;
}


int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(PWM_0_INST_INT_IRQN);
    DL_TimerA_startCounter(PWM_0_INST);

    while (1) {
        if(uint16_t_timer_count==0){
            DL_GPIO_togglePins(GPIO_GRP_0_LED_1_PORT, GPIO_GRP_0_LED_1_PIN);
            uint16_t_timer_count = uint16_t_timer_delay;
        }
        if(btn_1_isPressed()){
            if(uint16_t_timer_delay<MAX){
                uint16_t_timer_delay += DELAY;
                printf("btn 1 : %d\n",uint16_t_timer_delay);
            }
        }
        if(btn_2_isPressed()){
           if(uint16_t_timer_delay>MIN){
               uint16_t_timer_delay -= DELAY;
               printf("btn 2 : %d\n",uint16_t_timer_delay);
           }
        }
    }
}

void PWM_0_INST_IRQHandler(void){
    uint16_t_btn_1_previous_state = uint16_t_btn_1_current_state;
    uint16_t_btn_2_previous_state = uint16_t_btn_2_current_state;

    if(DL_GPIO_readPins(GPIO_GRP_0_BTN_1_PORT,GPIO_GRP_0_BTN_1_PIN)){
            (uint16_t_btn_1_current_state =1);
     }
     else{
            (uint16_t_btn_1_current_state =0);
     }
    if(DL_GPIO_readPins(GPIO_GRP_0_BTN_2_PORT,GPIO_GRP_0_BTN_2_PIN)){
                (uint16_t_btn_2_current_state =1);
         }
         else{
                (uint16_t_btn_2_current_state =0);
         }
    if(uint16_t_timer_count>0){
        uint16_t_timer_count--;
    }
}
