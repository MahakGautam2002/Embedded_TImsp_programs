/*
 * PS 5: Can Trasmission TX
 *  By Mahak Gautam - 220098
 */

#include "ti_msp_dl_config.h"

#define LED_STATUS_ON ((uint8_t) 0x01)
#define LED_STATUS_OFF ((uint8_t) 0x00)

volatile bool gTXMsg;
volatile bool error;

int main(void)
{
    DL_MCAN_TxBufElement txMsg;

    SYSCFG_DL_init();

    txMsg.id = ((uint32_t)(0x4)) << 18U;
    txMsg.rtr = 0U;
    txMsg.xtd = 0U;
    txMsg.esi = 0U;
    txMsg.dlc = 1U;
    txMsg.brs = 1U;
    txMsg.fdf = 00;
    txMsg.efc = 1U;
    txMsg.mm = 0xAAU;
    /* Data bytes. */0
    txMsg.data[0] = LED_STATUS_ON;

    NVIC_EnableIRQ(GPIO_SWITCHES_INT_IRQN);

    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(MCAN0_INST))
        ;

    while (1) {
        gTXMsg = false;

        /* Waits until button is pressed to send the message*/
        while (gTXMsg == false) {
            __WFE();
        }

        if (txMsg.data[0] == LED_STATUS_ON) {
            txMsg.data[0] = LED_STATUS_OFF;
        } else {
            txMsg.data[0] = LED_STATUS_ON;
        }

        /* Write Tx Message to the Message RAM. */
        DL_MCAN_writeMsgRam(MCAN0_INST, DL_MCAN_MEM_TYPE_BUF, 0U, &txMsg);

        /* Add request for transmission. */
        DL_MCAN_TXBufAddReq(MCAN0_INST, 0U);
    }
}

void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case GPIO_SWITCHES_INT_IIDX:
            switch (DL_GPIO_getPendingInterrupt(GPIO_SWITCHES_PORT)) {
                case DL_GPIO_IIDX_DIO21:
                    gTXMsg = true;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}


