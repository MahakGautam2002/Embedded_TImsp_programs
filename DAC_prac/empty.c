//dac enable
//vref+, vref-
//enable dac output
//advance- output amplify on
//others - sysctl= enable clock tree
//       - mpfclk = enable
//vref - internal voltage 2.5
//press on clock tree- mpfclk- enable


//join red of multimeter to PA15 and black to grd , to se voltage output , and dial on 2 volt
#include "ti_msp_dl_config.h"
#include "stdio.h"

//#define refVolt (2500)  4096
//#define digitalInput  ?  (4095)

#define refVolt (2500)
#define digitalInput (2500)


uint32_t OutputVolt;

int main(void)
{
    SYSCFG_DL_init();

    OutputVolt=(digitalInput* refVolt) /4095;
    printf("analog volt in mv:%d \n",OutputVolt);

    DL_DAC12_output12(DAC0, digitalInput);
    DL_DAC12_enable(DAC0);

    while (1) {
        __WFI();
    }
}
