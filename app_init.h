#ifndef APP_INIT_H_
#define APP_INIT_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_common.h"
#include "driver_uart.h"
#include "driver_clock.h"
#include "driver_port.h"
#include "driver_nvic.h"
#include "driver_adc.h"
#include "driver_lpit.h"
#include "driver_gpio.h"
#include "driver_sim.h"
#include "driver_trgmux.h"
#include "driver_dma.h"
#include "driver_dmamux.h"
#include "driver_systick.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
#define LED_BLUE_PIN         0
#define LED_RED_PIN          15
#define LED_GREEN_PIN        16
#define SWITCH_2_PIN         12
#define SWITCH_3_PIN         13

#define DOUBLE_CLICK_TIME   500
#define ADC_RESOLUTION      4095

/******************************************************************************
 * API
 ******************************************************************************/
void initClock()
{
    /* Enable Fast IRC Div2 */
    CLOCK_DRV_SetFircAsyncClkDiv(SCG_AsyncDiv2Clk, SCG_AsyncClkDivBy1);

    /* Enable ADC0 clock */
    CLOCK_DRV_SetIpSrc(CLOCK_ADC0, CLOCK_IpSrcFircAsync);
    CLOCK_DRV_EnableClock(CLOCK_ADC0);

    /* Enable LPIT clock */
    CLOCK_DRV_SetIpSrc(CLOCK_LPIT, CLOCK_IpSrcFircAsync);
    CLOCK_DRV_EnableClock(CLOCK_LPIT);

    /* Enable LPUART1 clock */
    CLOCK_DRV_SetIpSrc(CLOCK_LPUART1, CLOCK_IpSrcFircAsync);
    CLOCK_DRV_EnableClock(CLOCK_LPUART1);

    /* Enable PORTs clock */
    CLOCK_DRV_EnableClock(CLOCK_PORTC);
    CLOCK_DRV_EnableClock(CLOCK_PORTD);
}

void initGPIO()
{
    gpio_pin_config_t LEDs_config =
    {
        .pinDirection = GPIO_DigitalOutput,
        .outputLogic = 1,
    };
    gpio_pin_config_t SWITCHs_config =
    {
        .pinDirection = GPIO_DigitalInput,
    };

    /* LEDs init */
    PORT_DRV_SetPinMux(PORTD, 15, 1);
    GPIO_DRV_PinInit(PTD, 15, &LEDs_config);
    PORT_DRV_SetPinMux(PORTD, 16, 1);
    GPIO_DRV_PinInit(PTD, 16, &LEDs_config);
    PORT_DRV_SetPinMux(PORTD, 0, 1);
    GPIO_DRV_PinInit(PTD, 0, &LEDs_config);

    /* SWITCHs init */
    PORT_DRV_SetPinMux(PORTC, 12, 1);
    PORT_DRV_SetPinInterruptConfig(PORTC, 12, PORT_InterruptRisingEdge);
    GPIO_DRV_PinInit(PTC, 12, &SWITCHs_config);
    PORT_DRV_SetPinMux(PORTC, 13, 1);
    PORT_DRV_SetPinInterruptConfig(PORTC, 13, PORT_InterruptRisingEdge);
    GPIO_DRV_PinInit(PTC, 13, &SWITCHs_config);
    NVIC_EnableIRQ(PORTC_IRQn);
}

void initADC()
{
    adc_config_t         init_config =
    {
        .referenceVoltageSource     = ADC_ReferenceVoltageSourceVref,
        .clockSource                = ADC_ClockSourceAlt0,
        .clockDivider               = ADC_ClockDivider1,
        .resolution                 = ADC_Resolution12Bit,
        .sampleClockCount           = 13U,
        .enableContinuousConversion = false,
        .triggerType                = ADC_TriggerTypeHardware,
        .dmaEnable                  = true,
    };
    adc_channel_config_t channel_config =
    {
        .channelNumber                        = 12U,
        .enableInterruptOnConversionCompleted = false,
    };

    ADC_DRV_Init(ADC0, &init_config);
    ADC_DRV_SetChannelConfig(ADC0, 0, &channel_config);
}

void initUART()
{
    lpuart_config_t init_config =
    {
        .baudRate_Bps  = 115200u,
        .parityMode    = LPUART_ParityDisabled,
        .dataBitsCount = LPUART_EightDataBits,
        .isMsb         = false,
        .stopBitCount  = LPUART_OneStopBit,
        .enableTx      = true,
        .enableRx      = true,
    };

    PORT_DRV_SetPinMux(PORTC, 6, PORT_MuxAlt2);
    PORT_DRV_SetPinMux(PORTC, 7, PORT_MuxAlt2);

    LPUART_DRV_Init(LPUART1, &init_config, SystemCoreClock);

    LPUART1->CTRL |= LPUART_CTRL_RIE(1);
    NVIC_EnableIRQ(LPUART1_RxTx_IRQn);
}

void initLPIT()
{
    lpit_config_t      init_config =
    {
        .enableRunInDebug = false,
        .enableRunInDoze  = false,
    };
    lpit_chnl_params_t chnlSetup =
    {
        .chainChannel          = false,
        .timerMode             = LPIT_PeriodicCounter,
        .enableReloadOnTrigger = false,
        .enableStartOnTrigger  = false,
        .enableStopOnTimeout   = false,
    };

    LPIT_DRV_Init(LPIT0, &init_config);
    LPIT_DRV_DisableInterrupts(LPIT0, (LPIT_MIER_TIE0_MASK | LPIT_MIER_TIE1_MASK |
                                       LPIT_MIER_TIE2_MASK | LPIT_MIER_TIE3_MASK));
    LPIT_DRV_SetupChannel(LPIT0, LPIT_Chnl_0, &chnlSetup);

    LPIT_DRV_SetTimerPeriod(LPIT0, LPIT_Chnl_0, SystemCoreClock/10 - 1);
    LPIT_DRV_StartTimer(LPIT0, LPIT_Chnl_0);
}

void initSIM()
{
    sim_adc0_opt_t option =
    {
        .ADC_Trigger_source    = SIM_ADC_TriggerSource_TRGMUX,
        .ADC_PreTrigger_source = SIM_ADC_PreTriggerSource_TRGMUX,
    };
    SIM_DRV_ADC0option(&option);

    TRGMUX_DRV_SetTriggerSource(TRGMUX, TRGMUX_ADC0_INDEX, TRGMUX_TriggerInput0,
                                                        TRGMUX_Source_LPIT_CH0);
}

void initDMA(uint32_t storing_address)
{
    dma_channel_config_t config =
    {
        .srcAddr          = (uint32_t)&(ADC0->R[0]),
        .srcTransferSize  = DMA_TRANSFER_SIZE_4B,
        .destAddr         = storing_address,
        .destTransferSize = DMA_TRANSFER_SIZE_4B,
    };

    DMA_DRV_SetChannelConfig(DMA, 0, &config);

    CLOCK_DRV_EnableClock(CLOCK_DMAMUX);
    DMAMUX_DRV_ChannelDisable(DMAMUX, 0);
    DMAMUX_DRV_ChannelSourceSelect(DMAMUX, 0, DMAMUX_ADC0);
    DMAMUX_DRV_ChannelEnable(DMAMUX, 0);
}

#endif /* APP_INIT_H_ */

/******************************************************************************
 * EOF
 ******************************************************************************/
