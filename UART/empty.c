/*
 * uart- 9600 baud rate
 * interrupt- receiver
 * pinmux - PA11 PA10 uart 0
 *           PA8 PA9 uart 1 ----- RX485 - rx-rx and tx-tx
 *                          ----Rx 232 rx--tx and tx---rx and jumpers of UART must be below
 */
#include "ti_msp_dl_config.h"

volatile uint16_t result=0;

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);

    while (1) {
        __WFI();
    }
}

void UART_0_INST_IRQHandler(void){
    switch(DL_UART_getPendingInterrupt(UART_0_INST)){
    case DL_UART_IIDX_RX:
        result=DL_UART_receiveData(UART_0_INST);
        DL_UART_transmitData(UART_0_INST, result);
        printf("%c \n",result);
        break;
    default:
        break;
    }
}
