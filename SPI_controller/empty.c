/*
 * spi mode - controller
 * bitrate- 80 ❌ 5 right
 * chip select cs1
 * pinmux spi1 9,8,7,17
 * grd to grd
 */

#include "ti_msp_dl_config.h"

#define PacketSize (4)

volatile uint8_t TXpacket[PacketSize] = { 1, 2, 3, 4 };
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
                printf("R:%c \n", RXpacket[i]);
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

//*************************************************************
//#include "ti_msp_dl_config.h"
//#include "stdio.h"
//
//#define SPI_PACKET_SIZE   (4)
//
//volatile uint8_t flag = 0;
//
//uint8_t txPacket[SPI_PACKET_SIZE] = { 1, 2, 3, 4 };
//uint8_t rxPacket[SPI_PACKET_SIZE];
//
//int main(void)
//{
//
//    SYSCFG_DL_init();
//    NVIC_EnableIRQ(GPIO_GRP_0_INT_IRQN);
//    NVIC_EnableIRQ(SPI_0_INST_INT_IRQN);
//
//    while (1)
//    {
//        __WFI();
//        if (flag == 1)
//        {
//            for (int i = 0; i < SPI_PACKET_SIZE; i++)
//            {
//                printf("%c", rxPacket[i]);
//            }
//            printf("\n");
//            flag = 0;
//        }
//
//    }
//
//}
//
//void GROUP1_IRQHandler(void)
//{
//    switch (DL_GPIO_getPendingInterrupt(GPIO_GRP_0_PORT))
//    {
//    case DL_GPIO_IIDX_DIO18:
//        DL_SPI_fillTXFIFO8(SPI_0_INST, &txPacket[0], SPI_PACKET_SIZE);
//        while (DL_SPI_isBusy(SPI_0_INST))
//        {
//            ;
//        }
//        break;
//    default:
//        break;
//    }
//}
//void SPI1_IRQHandler(void)
//{
//    switch (DL_SPI_getPendingInterrupt(SPI_0_INST))
//    {
//    case DL_SPI_IIDX_RX:
//        for (uint8_t i = 0; i < SPI_PACKET_SIZE; i++)
//        {
//            rxPacket[i] = DL_SPI_receiveDataBlocking8(SPI_0_INST);
//        }
//        flag = 1 ;
//        break;
//    default :
//        break ;
//    }
//}
