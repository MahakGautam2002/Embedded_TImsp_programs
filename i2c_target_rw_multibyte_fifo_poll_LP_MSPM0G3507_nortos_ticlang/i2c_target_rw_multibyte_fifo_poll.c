//target****************************************************
#include "ti_msp_dl_config.h"

/*
 * Number of bytes to send from Target to Controller.
 *  The packet will be sent to the Controller during a Read request.
 *  This example uses FIFO with polling, and the maximum FIFO size is 8.
 *  Refer to interrupt examples to handle larger packets.
 */
#define I2C_TX_PACKET_SIZE (5)

/*
 * Number of bytes to receive from Controller.
 *  This example uses FIFO with polling, and the maximum FIFO size is 8.
 *  The example will wait until it receives all bytes.
 *  Refer to interrupt examples to handle larger packets
 */
#define I2C_RX_PACKET_SIZE (8)

/* Data sent to Controller in response to Read transfer */
uint8_t gTxPacket[I2C_TX_PACKET_SIZE] = { 'M', 'S', 'P', 'A', '!' };

/* Data received from Controller during a Write transfer */
uint8_t gRxPacket[I2C_RX_PACKET_SIZE];

int main(void)
{
    SYSCFG_DL_init();

    /* Set LED to indicate start of transfer */

    /* If write and read were successful, toggle LED */
    while (1)
    {
        DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

        /*
         * Fill FIFO with data.
         * Note that transactions are initiated by the Controller, so this function
         * only fills the buffer and the Target device will send this data when
         * requested by the Controller.
         */
        DL_I2C_fillTargetTXFIFO(I2C_INST, &gTxPacket[0], I2C_TX_PACKET_SIZE);

        /*
         * Wait to receive data from the Controller.
         * This loop expects I2C_RX_PACKET_SIZE bytes.
         */
        for (uint8_t i = 0; i < I2C_RX_PACKET_SIZE; i++)
        {
            while (DL_I2C_isTargetRXFIFOEmpty(I2C_INST))
                ;
            gRxPacket[i] = DL_I2C_receiveTargetData(I2C_INST);
            printf("%u \n", gRxPacket[i]);
        }

        /*
         * Wait until all bytes written to TX FIFO are sent after a successful
         * request.
         */
        while (!DL_I2C_isTargetTXFIFOEmpty(I2C_INST))
            ;

        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        delay_cycles(16000000);
    }
}
