/*
 * spi mode - peripheral
 * chip select cs1
 * pinmux spi1 9,8,7,17
 */

#include "ti_msp_dl_config.h"

#define PacketSize (4)

volatile uint8_t TXpacket[PacketSize] = { 'a','b','c','d' };
volatile uint8_t RXpacket[PacketSize];

volatile uint8_t flag = 0;

int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(GPIO_GRP_0_INT_IRQN);
    NVIC_EnableIRQ(SPI_0_INST_INT_IRQN);

    while (1)
    {
        __WFI();
        if (flag == 1)
        {

            for (int i = 0; i < PacketSize; i++)
            {
                printf("R:%d \n", RXpacket[i]);
            }
            printf("\n");

            flag = 0;
        }
    }
}
void SPI_0_INST_IRQHandler(void)
{
    switch (DL_SPI_getPendingInterrupt(SPI_0_INST))
    {
    case DL_SPI_IIDX_RX:
        for (uint8_t i = 0; i < PacketSize; i++)
        {
            RXpacket[i] = DL_SPI_receiveDataBlocking8(SPI_0_INST);

        }
        flag = 1;

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
        DL_SPI_fillTXFIFO8(SPI_0_INST, &TXpacket[0], PacketSize);
        while (DL_SPI_isBusy(SPI_0_INST))
            ;
        break;
    default:
        break;
    }
}
