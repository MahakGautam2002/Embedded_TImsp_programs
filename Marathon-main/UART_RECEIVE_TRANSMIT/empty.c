#include "ti_msp_dl_config.h"
#include "stdio.h"
#define UART_BUFFER_SIZE (4)
#define TX_BUFFER_SIZE (256)
#define FALSE (0)
#define TRUE (1)

uint8_t TX_BUFFER[TX_BUFFER_SIZE];
uint16_t ind=0;
uint16_t remainingBytes =0;
volatile uint16_t flag = FALSE;
uint8_t response_buffer[TX_BUFFER_SIZE];
uint16_t bufferIndex =0;

void addData(uint8_t* data,uint16_t size){
    for(uint8_t i=0;i<size;i++){
        TX_BUFFER[i] = data[i];
    }
    remainingBytes =size;
}
void transmitData(){
    while(remainingBytes>0){
        uint8_t bytestosend = (remainingBytes > UART_BUFFER_SIZE) ? UART_BUFFER_SIZE : remainingBytes;
        DL_GPIO_setPins(GPIO_UART_TX_PORT, GPIO_UART_TX_PIN);
        DL_UART_Main_fillTXFIFO(UART_INST,&TX_BUFFER[ind] , bytestosend);

        while(DL_UART_Main_isBusy(UART_INST));

        DL_GPIO_clearPins(GPIO_UART_TX_PORT, GPIO_UART_TX_PIN);
        ind += bytestosend;
        remainingBytes -= bytestosend;

    }
    ind = 0;
}

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(GPIO_INT_IRQN);
    NVIC_EnableIRQ(UART_INST_INT_IRQN);
    uint8_t msg[] = "Hello this is Archita. Nice to meet you. Ajju has taught me this with a lot of efforts. Love ya. Bye, Signing off!!!";
    while (1) {

        if(FALSE == flag){
            __WFI();
        }

        addData(msg, sizeof(msg)-1);
        if(remainingBytes>0){
            transmitData();
        }
        flag = FALSE;

        for (int i = 0; i < 15; i++){
             printf("%c ", response_buffer[i]);
        }
        printf("\n");
    }
}


void GROUP1_IRQHandler(void){
   switch(DL_GPIO_getPendingInterrupt(GPIOA)){
   case DL_GPIO_IIDX_DIO18:
       flag = TRUE;
       break;
   default:
       break;
   }
}

void UART_INST_IRQHandler(){
    switch (DL_UART_Main_getPendingInterrupt(UART_INST)){
        case DL_UART_MAIN_IIDX_RX :
            while(!DL_UART_isRXFIFOEmpty(UART_INST)){
                   uint8_t one_byte = DL_UART_Main_receiveData(UART_INST);
                   if(bufferIndex < TX_BUFFER_SIZE){
                       response_buffer[bufferIndex] = one_byte;
                       (bufferIndex)++;
                   }
                }
            break;
//        case DL_UART_MAIN_INTERRUPT_NOISE_ERROR:
//            for(int i=0;i<TX_BUFFER_SIZE;i++){
//                TX_BUFFER[i] =0;
//            }
//            break;
//        case DL_UART_MAIN_INTERRUPT_RX_TIMEOUT_ERROR:
//            for(int i=0;i<TX_BUFFER_SIZE;i++){
//                            TX_BUFFER[i] =0;
//                        }
//            break;
//        case DL_UART_MAIN_INTERRUPT_FRAMING_ERROR:
//            for(int i=0;i<TX_BUFFER_SIZE;i++){
//                            TX_BUFFER[i] =0;
//                        }
//            break;
//        case DL_UART_MAIN_INTERRUPT_PARITY_ERROR:
//            for(int i=0;i<TX_BUFFER_SIZE;i++){
//                            TX_BUFFER[i] =0;
//                        }
//            break;
//        case DL_UART_MAIN_INTERRUPT_BREAK_ERROR:
//            for(int i=0;i<TX_BUFFER_SIZE;i++){
//                            TX_BUFFER[i] =0;
//                        }
//            break;
//        case DL_UART_MAIN_INTERRUPT_OVERRUN_ERROR:
//            for(int i=0;i<TX_BUFFER_SIZE;i++){
//                            TX_BUFFER[i] =0;
//                        }
//            break;
        default:
            break;
    }
}
