//bare metal
//#include "ti_msp_dl_config.h"
#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#define POWER_STARTUP_DELAY                                                (16)

#define CPUCLK_FREQ                                                     32000000

/* Defines for led22: GPIOB.22 with pinCMx 50 on package pin 21 */
#define GPIO_GRP_0_led22_PORT                                            (GPIOB)
#define GPIO_GRP_0_led22_PIN                                    (DL_GPIO_PIN_22)
#define GPIO_GRP_0_led22_IOMUX                                   (IOMUX_PINCM50)
/* Defines for led26: GPIOB.26 with pinCMx 57 on package pin 28 */
#define GPIO_GRP_0_led26_PORT                                            (GPIOB)
#define GPIO_GRP_0_led26_PIN                                    (DL_GPIO_PIN_26)
#define GPIO_GRP_0_led26_IOMUX                                   (IOMUX_PINCM57)
/* Defines for led27: GPIOB.27 with pinCMx 58 on package pin 29 */
#define GPIO_GRP_0_led27_PORT                                            (GPIOB)
#define GPIO_GRP_0_led27_PIN                                    (DL_GPIO_PIN_27)
#define GPIO_GRP_0_led27_IOMUX                                   (IOMUX_PINCM58)
/* Defines for led0: GPIOA.0 with pinCMx 1 on package pin 33 */
#define GPIO_GRP_0_led0_PORT                                             (GPIOA)
#define GPIO_GRP_0_led0_PIN                                      (DL_GPIO_PIN_0)
#define GPIO_GRP_0_led0_IOMUX                                     (IOMUX_PINCM1)
/* Defines for k1_18: GPIOA.18 with pinCMx 40 on package pin 11 */
#define GPIO_GRP_0_k1_18_PORT                                            (GPIOA)
#define GPIO_GRP_0_k1_18_PIN                                    (DL_GPIO_PIN_18)
#define GPIO_GRP_0_k1_18_IOMUX                                   (IOMUX_PINCM40)
/* Defines for k2_21: GPIOB.21 with pinCMx 49 on package pin 20 */
#define GPIO_GRP_0_k2_21_PORT                                            (GPIOB)
#define GPIO_GRP_0_k2_21_PIN                                    (DL_GPIO_PIN_21)
#define GPIO_GRP_0_k2_21_IOMUX                                   (IOMUX_PINCM49)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);

int main(void)
{
    SYSCFG_DL_init();

    while (1)
    {

        if (GPIOA->DIN31_0 & 0x00040000)
        {
            GPIOB->DOUTTGL31_0 = 0x00400000;
//            delay_cycles(32000000);
        }

//        if (DL_GPIO_readPins(GPIO_GRP_0_k1_18_PORT, GPIO_GRP_0_k1_18_PIN))
//                {
//                    DL_GPIO_togglePins(GPIO_GRP_0_led22_PORT,
//                                       GPIO_GRP_0_led22_PIN | GPIO_GRP_0_led26_PIN); // pink led toggle
//                }

    }
}

void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_SYSCTL_init();
}

void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    delay_cycles(POWER_STARTUP_DELAY);
}

void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initDigitalOutput(GPIO_GRP_0_led22_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_GRP_0_led26_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_GRP_0_led27_IOMUX);

    DL_GPIO_initDigitalOutputFeatures(GPIO_GRP_0_led0_IOMUX,
                                      DL_GPIO_INVERSION_ENABLE,
                                      DL_GPIO_RESISTOR_PULL_UP,
                                      DL_GPIO_DRIVE_STRENGTH_LOW,
                                      DL_GPIO_HIZ_DISABLE);

    DL_GPIO_initDigitalInputFeatures(GPIO_GRP_0_k1_18_IOMUX,
                                     DL_GPIO_INVERSION_DISABLE,
                                     DL_GPIO_RESISTOR_NONE,
                                     DL_GPIO_HYSTERESIS_DISABLE,
                                     DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(GPIO_GRP_0_k2_21_IOMUX,
                                     DL_GPIO_INVERSION_ENABLE,
                                     DL_GPIO_RESISTOR_PULL_UP,
                                     DL_GPIO_HYSTERESIS_DISABLE,
                                     DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_clearPins(GPIOA, GPIO_GRP_0_led0_PIN);
    DL_GPIO_enableOutput(GPIOA, GPIO_GRP_0_led0_PIN);
    DL_GPIO_clearPins(GPIOB, GPIO_GRP_0_led22_PIN |
    GPIO_GRP_0_led26_PIN |
    GPIO_GRP_0_led27_PIN);
    DL_GPIO_enableOutput(GPIOB, GPIO_GRP_0_led22_PIN |
    GPIO_GRP_0_led26_PIN |
    GPIO_GRP_0_led27_PIN);

}

void SYSCFG_DL_SYSCTL_init(void)
{

    //Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    /* Set default configuration */
    DL_SYSCTL_disableHFXT();
    DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_1);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);

}

