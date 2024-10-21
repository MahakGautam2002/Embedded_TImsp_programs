/*
 * PS4:
 * Automotive Parking Assist System
 *  By Mahak Gautam - 220098
 */

#include "ti_msp_dl_config.h"
#include "stdio.h"

#define MaxBuffersize (256) // Maximum size of the transmission buffer
#define UARTbufferSize (4)  // UART transmits data in chunks of 4 bytes

// Global variables
volatile uint32_t gCounter = 0;           // Timer counter
volatile uint32_t gIncrementValue = 1;    // Increment value for the counter
volatile uint32_t gLargestDistance = 10;  // Initial largest measured distance (in meters)


uint8_t TransmitBuffer[MaxBuffersize];    // Buffer to store data to be sent via UART
uint8_t TXindex = 0;                      // Index for tracking position in TransmitBuffer
uint8_t RemainingBytes = 0;               // Tracks the number of bytes left to transmit


void AddDataTX(uint8_t *msg, uint8_t datasize); // Function to add data into the transmission buffer
void TransmitData();                            // Function to transmit data via UART

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);  // Enable Timer interrupt
    DL_TimerA_startCounter(TIMER_0_INST);   // Start timer


    while (1)
    {
        __WFI();

        // If distance is between 5 and 10 meters
        if (gLargestDistance <= 10 && gLargestDistance >= 5)
        {
            if (gCounter == gLargestDistance)  // Check if counter matches distance
            {
                DL_GPIO_togglePins(GPIO_GRP_0_PORT, GPIO_GRP_0_LD1_PIN);  // Toggle LED
                delay_cycles(64000000);  // Introduce a delay of 2sec

                // Prepare caution message
                uint8_t sendData[] = "Caution: Distance less than 10 meters \n";
                AddDataTX(&sendData, sizeof(sendData) - 1);  // Add to TX buffer

                // Transmit data if there are remaining bytes to send
                if (RemainingBytes > 0)
                {
                    TransmitData();  // Transmit the data via UART
                    printf("caution \n");
                }

                gCounter = 0;  // Reset counter after transmission
            }
        }
        // If distance is between 2 and 5 meters
        else if (gLargestDistance < 5 && gLargestDistance > 2)
        {
            if (gCounter == gLargestDistance)  // Check if counter matches distance
            {
                DL_GPIO_togglePins(GPIO_GRP_0_PORT, GPIO_GRP_0_LD1_PIN);  // Toggle LED
                delay_cycles(32000000);  // Introduce a delay of 1sec

                // Prepare warning message
                uint8_t sendData[] = "Warning: Distance less than 5 meters \n";
                AddDataTX(&sendData, sizeof(sendData) - 1);

                // Transmit data if there are remaining bytes to send
                if (RemainingBytes > 0)
                {
                    TransmitData();  // Transmit the data via UART
                    printf("warning \n");
                }

                gCounter = 0;  // Reset counter after transmission
            }
        }
        // If distance is less than or equal to 2 meters
        else if (gLargestDistance <= 2 && gLargestDistance >= 0)
        {
            if (gCounter == gLargestDistance)  // Check if counter matches distance
            {
                DL_GPIO_togglePins(GPIO_GRP_0_PORT, GPIO_GRP_0_LD1_PIN);  // Toggle LED
                delay_cycles(1600000);  // Introduce a delay of 0.5sec

                // Prepare danger message
                uint8_t sendData[] = "Danger: Distance less than 2 meters \n";
                AddDataTX(&sendData, sizeof(sendData) - 1);
                // Transmit data if there are remaining bytes to send
                if (RemainingBytes > 0)
                {
                    TransmitData();  // Transmit the data via UART
                    printf("Danger\n");
                }

                gCounter = 0;  // Reset counter after transmission
            }
        }

        // check boarder conditions
        if (gLargestDistance > 10)  // Cap the largest distance at 10 meters
            gLargestDistance = 10;
        if (gLargestDistance < 0)   // Ensure distance doesn't go below 0 meters
            gLargestDistance = 0;
    }
}

// Timer interrupt handler
void TIMER_0_INST_IRQHandler(void)
{
    // Check for repeat count interrupt
    switch (DL_TimerA_getPendingInterrupt(TIMER_0_INST))
    {
    case DL_TIMERA_IIDX_REPEAT_COUNT:
        gCounter += 1;  // Increment counter on each interrupt

        // If a button is pressed, decrease the distance
        if (DL_GPIO_readPins(GPIO_GRP_0_PORT, GPIO_GRP_0_BTN_PIN))
        {
            printf("k2 maxd:%u\n", gLargestDistance);  // Print the largest distance
            gLargestDistance -= 1;  // Decrease distance
            gCounter = 0;  // Reset counter
        }
        break;

    default:
        break;
    }
}

// Function to add data to the transmission buffer
void AddDataTX(uint8_t *msg, uint8_t datasize)
{
    // Check if the data size is within buffer limits
    if (datasize <= MaxBuffersize)
    {
        // Copy message into the transmission buffer
        for (uint8_t i = 0; i < datasize; i++)
        {
            TransmitBuffer[i] = msg[i];
        }
        RemainingBytes = datasize;  // Set the number of bytes remaining to transmit
        TXindex = 0;  // Reset the buffer index
    }
}

// Function to transmit data via UART
void TransmitData()
{
    // Transmit data until all bytes are sent
    while (RemainingBytes > 0)
    {
        // Determine how many bytes to send in this chunk
        uint8_t byteTosend = (RemainingBytes > UARTbufferSize) ? UARTbufferSize : RemainingBytes;

        // Transmit the chunk of data via UART
        DL_UART_fillTXFIFO(UART_0_INST, &TransmitBuffer[TXindex], byteTosend);

        // Wait until UART transmission is complete
        while (DL_UART_isBusy(UART_0_INST))
            ;

        // Update remaining bytes and buffer index
        RemainingBytes -= byteTosend;
        TXindex += byteTosend;
    }

    TXindex = 0;  // Reset index after transmission
}
