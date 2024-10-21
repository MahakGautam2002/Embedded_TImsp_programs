/*
 * PS2:
 * Automotive Interior Lightning Control System
 *  By Mahak Gautam - 220098
 */

#include "ti_msp_dl_config.h"
#include "stdio.h"

#define MaxBuffersize (256) //Maximum size of Buffer
#define UARTbufferSize (4) // Uart transfer data in chunks of 4

volatile uint8_t gLED1flag = 0; // flag to determine if led1 is ON
volatile uint8_t gLED2flag = 0; // flag to determine if led2 is ON
volatile uint8_t gReceiveData; // receive CMD from Docklight

uint8_t TransmitBuffer[MaxBuffersize]; // Buffer to store data to be send
uint8_t TXindex = 0; // index of Transmit Buffer
uint8_t RemainingBytes = 0; // bytes remaining to transfer

void AddDataTX(uint8_t *msg, uint8_t datasize); // function to add sendData into Transmit Buffer
void TransmitData(); // sending data to UART/docklight

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN); // Enable UART Interrupt

    while (1)
    {
        __WFI();  // Wait for interrupt

        // if Both LEDs is ON
        if (gLED1flag == 1 && gLED2flag == 1)
        {
            uint8_t sendData[] = "Mood Lighting Mode "; // if both LEDs is on then send data is MOOD LIGHTING MODE

            AddDataTX(&sendData, sizeof(sendData) - 1); // add sendData to TXbuffer

            // Transmit untill no byte is remaining
            if (RemainingBytes > 0)
            {
                TransmitData();  // Transmit the data
                printf("LED1 && LED2 \n");
            }

            gLED1flag = 0;  // Reset flag after processing
            gLED2flag = 0;  // Reset flag after processing


        }

        // if LED1 is ON
        if (gLED1flag == 1)
        {
            uint8_t sendData[] = "Ambient Lighting Mode "; // if LEDs1 is on then send data is Ambient LIGHTING MODE
            AddDataTX(&sendData, sizeof(sendData) - 1); // add sendData to TXbuffer
            // Transmit untill no byte is remaining
            if (RemainingBytes > 0)
            {
                TransmitData();  // Transmit the data
                printf("LED1 \n");
            }

            gLED1flag = 0;  // Reset flag after processing
            DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_LD2_PIN); // turn off led

        }

        //LED2 is ON
        if (gLED2flag == 1)
        {
            uint8_t sendData[] = "Reading Lighting Mode "; // if LEDs2 is on then send data is Reading LIGHTING MODE
            AddDataTX(&sendData, sizeof(sendData) - 1); // add sendData to TXbuffer
            // Transmit untill no byte is remaining
            if (RemainingBytes > 0)
            {
                TransmitData();  // Transmit the data
                printf("LED2 \n");
            }

            gLED2flag = 0;  // Reset flag after processing
            DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_LD1_PIN); // turn off led

        }

    }
}

// UART interrupt handler
void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_getPendingInterrupt(UART_0_INST))
    {
    case DL_UART_IIDX_RX: // interrupt occure when data is receive
        gReceiveData = DL_UART_receiveData(UART_0_INST); // store received data from UART
        if (gReceiveData == '3') // if receivedata is 3 the both LED must ON and respective flag is set
        {
            DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_LD1_PIN);
            DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_LD2_PIN);
            gLED1flag = 1;
            gLED2flag = 1;
        }
        if (gReceiveData == '1') // if receivedata is 1 the  LED1 must ON and respective flag is set
        {
            DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_LD1_PIN);

            gLED1flag = 1;
        }
        if (gReceiveData == '2') // if receivedata is 2 the  LED2 must ON and respective flag is set
        {
            DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_LD2_PIN);
            gLED2flag = 1;
        }

        break;
    default:
        break;
    }
}


// Add senddata to TXbuffer
void AddDataTX(uint8_t *msg, uint8_t datasize)
{
    if (datasize <= MaxBuffersize) // check if sendData size is within Limits
    {
        for (uint8_t i = 0; i < datasize; i++)
        {
            TransmitBuffer[i] = msg[i]; // store sendData content in TX Buffer
        }
        RemainingBytes = datasize; // assign remaining Byte to send as Data size
        TXindex = 0;
    }
}


// Transmit data function
void TransmitData()
{
    while (RemainingBytes > 0) // till no Byte remin to send
    {
        uint8_t byteTosend = (RemainingBytes > UARTbufferSize) ? UARTbufferSize :RemainingBytes; // calculate chunks to trasmit

        DL_UART_fillTXFIFO(UART_0_INST, &TransmitBuffer[TXindex], byteTosend); // Transmit data in chunks
        while (DL_UART_isBusy(UART_0_INST))
            ; // Wait until UART is no longer busy
        RemainingBytes -= byteTosend;  // decrease remain byte to send
        TXindex += byteTosend; // increase counter of TxBuffer to send
    }
    TXindex = 0; // reset index
}
