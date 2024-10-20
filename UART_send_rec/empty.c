/*
 * uart- 9600 baud rate
 * interrupt- receiver
 * pinmux - PA11 PA10 uart 0
 *           PA8 PA9 uart 1 ----- RX485 - rx-rx and tx-tx
 *                          ----Rx 232 rx--tx and tx---rx and jumpers of UART must be below
 *
 *   here btn 18th interrupt is used for sending data to docklight
 */
#include "ti_msp_dl_config.h"

#define Buffersize  (10)
#define MaxBuffersize (256)
#define sendPackt (4)

uint8_t TxBuff[MaxBuffersize];
volatile uint8_t RXBuff[Buffersize];

//volatile uint8_t RXBuff[]="mahak sending msg";

//uint8_t msg[Buffersize];
uint8_t TXindex = 0;
uint8_t remByte = 0;
volatile uint8_t RXindex = 0;
volatile uint8_t result;
volatile uint8_t flag = 0;

void transmitData()
{
    while (remByte > 0)
    {

        uint8_t byteTosend = (remByte > sendPackt) ? sendPackt : remByte;
        DL_UART_fillTXFIFO(UART_0_INST, &TxBuff[TXindex], byteTosend);  // transmit data in chunks
        while (DL_UART_isBusy(UART_0_INST))
            ; // uart is busy
        remByte -= byteTosend;
        TXindex += byteTosend;
    }
    TXindex = 0;
}

void addData(uint8_t *msg, uint8_t datasize)
{
    if (datasize <= MaxBuffersize)
    {
        for (uint8_t i = 0; i < datasize; i++)
        {
            TxBuff[i] = msg[i];
        }
        remByte = datasize;
        TXindex = 0;
    }
}

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(GPIO_GRP_0_INT_IRQN);

    while (1)
    {
        if (0 == flag)
        {
            __WFI();
        }
        addData(&RXBuff, sizeof(RXBuff));
        if (remByte > 0)
        {
            transmitData();
        }
        flag = 0;
    }
}

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_getPendingInterrupt(UART_0_INST))
    {
    case DL_UART_IIDX_RX:
        while (!DL_UART_isRXFIFOEmpty(UART_0_INST))  /// while RX is not empty
        {
            for (uint8_t i = 0; i < Buffersize; i++)
            {
                RXBuff[i] = DL_UART_receiveDataBlocking(UART_0_INST);
            }
            for (uint8_t i = 0; i < Buffersize; i++)
            {
                printf("%c", RXBuff[i]);
            }
            printf("\n");
        }
        break;
    default:
        break;
    }
}

void GROUP1_IRQHandler(void)
{
    switch (DL_GPIO_getPendingInterrupt(GPIO_GRP_0_PORT))
    {
    case DL_GPIO_IIDX_DIO18:
        flag = 1;
        break;
    default:
        break;

    }
}

//**********************************************************
/*
 * To edit the code according to your requirements, I will make the following modifications:

Ensure that the first character in the received buffer is 1. If so, the next character will define the length of the string to be sent.
Only send the string if the first character is 1.
The length of the string to be sent will be the second character in the buffer (in this case 5).
The string "hello" should be extracted and sent as per the specified length.
 */
//#include "ti_msp_dl_config.h"
//
//#define Buffersize  (10)
//#define MaxBuffersize (256)
//#define sendPackt (4)
//
//uint8_t TxBuff[MaxBuffersize];
//volatile uint8_t RXBuff[Buffersize];
//
////volatile uint8_t RXBuff[]="mahak sending msg";
//
////uint8_t msg[Buffersize];
//uint8_t TXindex = 0;
//uint8_t remByte = 0;
//volatile uint8_t RXindex = 0;
//volatile uint8_t result;
//volatile uint8_t flag = 0;
//
//void transmitData()
//{
//    while (remByte > 0)
//    {
//
//        uint8_t byteTosend = (remByte > sendPackt) ? sendPackt : remByte;
//        DL_UART_fillTXFIFO(UART_0_INST, &TxBuff[TXindex], byteTosend); // transmit data in chunks
//        while (DL_UART_isBusy(UART_0_INST))
//            ; // uart is busy
//        remByte -= byteTosend;
//        TXindex += byteTosend;
//    }
//    TXindex = 0;
//}
//
//void addData(uint8_t *msg, uint8_t datasize)
//{
//    if (datasize <= MaxBuffersize)
//    {
//        for (uint8_t i = 0; i < datasize; i++)
//        {
//            TxBuff[i] = msg[i];
//        }
//        remByte = datasize;
//        TXindex = 0;
//    }
//}
//
//int main(void)
//{
//    SYSCFG_DL_init();
//    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
//    NVIC_EnableIRQ(GPIO_GRP_0_INT_IRQN);
//
//    while (1)
//    {
//        if (0 == flag)
//        {
//            __WFI();
//        }
//        if (RXBuff[0] == '1')
//        {
//            uint8_t length = RXBuff[1] - '0';
//            if (length <= Buffersize - 2)
//            {
//                addData(&RXBuff[2], length);
//
//            }
//            if (remByte > 0)
//            {
//                transmitData();
//            }
//        }
//
//        flag = 0;
//    }
//}
//
//void UART_0_INST_IRQHandler(void)
//{
//    switch (DL_UART_getPendingInterrupt(UART_0_INST))
//    {
//    case DL_UART_IIDX_RX:
//        while (!DL_UART_isRXFIFOEmpty(UART_0_INST))  /// while RX is not empty
//        {
//            for (uint8_t i = 0; i < Buffersize; i++)
//            {
//                RXBuff[i] = DL_UART_receiveDataBlocking(UART_0_INST);
//            }
//            for (uint8_t i = 0; i < Buffersize; i++)
//            {
//                printf("%c", RXBuff[i]);
//            }
//            printf("\n");
//        }
//        break;
//    default:
//        break;
//    }
//}
//
//void GROUP1_IRQHandler(void)
//{
//    switch (DL_GPIO_getPendingInterrupt(GPIO_GRP_0_PORT))
//    {
//    case DL_GPIO_IIDX_DIO18:
//        flag = 1;
//        break;
//    default:
//        break;
//
//    }
//}
