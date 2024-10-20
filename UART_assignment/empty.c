#include "ti_msp_dl_config.h"

#define Buffersize  (10)
#define MaxBuffersize (256)
#define sendPackt (4)

uint8_t TxBuff[MaxBuffersize];
volatile uint8_t RXBuff[Buffersize];
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
        DL_UART_fillTXFIFO(UART_0_INST, &TxBuff[TXindex], byteTosend);  // Transmit data in chunks
        while (DL_UART_isBusy(UART_0_INST)); // Wait until UART is no longer busy
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
            __WFI();  // Wait for interrupt
        }
        printf("after \n");
        // Check if the first character in the received buffer is '1'
        if (RXBuff[0] == '1')
        {
            printf("here \n");
            uint8_t length = RXBuff[1]-'0';  // Get the length (second character in the buffer)

            // Check if length is valid and does not exceed the buffer size
            if (length <= Buffersize - 2)
            {
                addData(&RXBuff[2], length);  // Add the message to be transmitted starting from index 2
                printf ("h \n ");
            }

            if (remByte > 0)
            {
                transmitData();  // Transmit the data
                printf("ok \n");
            }
        }

        flag = 0;  // Reset flag after processing
    }
}

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_getPendingInterrupt(UART_0_INST))
    {
    case DL_UART_IIDX_RX:
        while (!DL_UART_isRXFIFOEmpty(UART_0_INST))  // While RX FIFO is not empty
        {
            // Receive data into RX buffer
            for (uint8_t i = 0; i < Buffersize; i++)
            {
                RXBuff[i] = DL_UART_receiveDataBlocking(UART_0_INST);
            }

            // Display received data (optional for debugging)
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
        flag = 1;  // Set flag when interrupt occurs on pin 18
        break;
    default:
        break;
    }
}
