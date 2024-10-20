/*
 * uart- 9600 baud rate
 * interrupt- receiver
 * pinmux - PA11 PA10 uart 0
 *           PA8 PA9 uart 1 ----- RX485 - rx-rx and tx-tx
 *                          ----Rx 232 rx--tx and tx---rx and jumpers of UART must be below
 *
 *   here array is used to store receive data and fillTXfillo to send data
 */
#include "ti_msp_dl_config.h"

#define Buffersize  (10)
volatile uint8_t result[Buffersize];

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);

    while (1)
    {
        __WFI();
    }
}

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_getPendingInterrupt(UART_0_INST))
    {
    case DL_UART_IIDX_RX:
        while (!DL_UART_isRXFIFOEmpty(UART_0_INST))
        {

            for (int i = 0; i < Buffersize; i++)
            {
                result[i] = DL_UART_receiveDataBlocking(UART_0_INST);
            }

            for (int i = 0; i < Buffersize; i++)
            {
                printf("%c ", result[i]);
            }
            printf("\n");
            DL_UART_fillTXFIFO(UART_0_INST, &result[0], Buffersize);
            while (DL_UART_isBusy(UART_0_INST))
                ;
        }
        break;
    default:
        break;
    }
}
