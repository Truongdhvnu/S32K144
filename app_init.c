/******************************************************************************
 * Includes
 ******************************************************************************/
#include "app_init.h"

/******************************************************************************
 * Code
 ******************************************************************************/
void initSCG()
{
    /**
     * Changing FIRCDIV should be done when FAST IRC is disabled to prevent
     * glitches to output divided clock
     *
     * Step 1: Disable Fast IRC clock
     * Step 2: Set the Fast IRC asynchronous clock divider
     * Step 3: Re-enable Fast IRC clock
     */
    CLOCK_DRV_DisableFirc();
    CLOCK_DRV_SetFircAsyncClkDiv(SCG_AsyncDiv2Clk, SCG_AsyncClkDivBy1);
    CLOCK_DRV_EnableFirc();
    /* Wait for FIRC clock to be valid */
    while (0UL == CLOCK_DRV_GetFircValidStatus());
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


    /* Enable PORTs clock */
    CLOCK_DRV_EnableClock(CLOCK_PORTC);
    CLOCK_DRV_EnableClock(CLOCK_PORTD);

    /* LEDs init */
    PORT_DRV_SetPinMux(PORTD, 15, 2);   /* FTM0CH0 */
    PORT_DRV_SetPinMux(PORTD, 16, 2);   /* FTM0CH1 */
    PORT_DRV_SetPinMux(PORTD, 0, 2);    /* FTM0CH2 */

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

    /* ADC0 clock config */
    CLOCK_DRV_DisableClock(CLOCK_ADC0);
    CLOCK_DRV_SetIpSrc(CLOCK_ADC0, CLOCK_IpSrcFircAsync);
    CLOCK_DRV_EnableClock(CLOCK_ADC0);

    /* ADC0 init */
    ADC_DRV_Init(ADC0, &init_config);
    /* ADC0 channel 0 config */
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

    /* LPUART1 clock config */
    CLOCK_DRV_DisableClock(CLOCK_LPUART1);
    CLOCK_DRV_SetIpSrc(CLOCK_LPUART1, CLOCK_IpSrcFircAsync);
    CLOCK_DRV_EnableClock(CLOCK_LPUART1);

    /* Set PIN MUX for LPUART Tx Rx */
    PORT_DRV_SetPinMux(PORTC, 6, PORT_MuxAlt2);
    PORT_DRV_SetPinMux(PORTC, 7, PORT_MuxAlt2);

    /* LPUART init */
    LPUART_DRV_Init(LPUART1, &init_config, SystemCoreClock);

    /* Enable Rx interrupt */
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

    /* Enable LPIT clock */
    CLOCK_DRV_SetIpSrc(CLOCK_LPIT, CLOCK_IpSrcFircAsync);
    CLOCK_DRV_EnableClock(CLOCK_LPIT);

    /* LPIT init */
    LPIT_DRV_Init(LPIT0, &init_config);
    /* LPIT disable all interrupt */
    LPIT_DRV_DisableInterrupts(LPIT0, (LPIT_MIER_TIE0_MASK | LPIT_MIER_TIE1_MASK |
                                       LPIT_MIER_TIE2_MASK | LPIT_MIER_TIE3_MASK));
    /* LPIT channel 0 setup config */
    LPIT_DRV_SetupChannel(LPIT0, LPIT_Chnl_0, &chnlSetup);
    /* LPIT channel 0 timer config */
    LPIT_DRV_SetTimerPeriod(LPIT0, LPIT_Chnl_0, SystemCoreClock/10 - 1);
    /* LPIT channel 0 start timer */
    LPIT_DRV_StartTimer(LPIT0, LPIT_Chnl_0);
}

void initSIM()
{
    sim_adc0_opt_t option =
    {
        .ADC_Trigger_source    = SIM_ADC_TriggerSource_TRGMUX,
        .ADC_PreTrigger_source = SIM_ADC_PreTriggerSource_TRGMUX,
    };
    /* ADC0 option setting */
    SIM_DRV_ADC0option(&option);
    /* Setup ADC0 trigger source to LPIT_CH0 via TRGMUX */
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
    /* DMA channel 0 config */
    DMA_DRV_SetChannelConfig(DMA, 0, &config);
    /* Enable DMAMUX clock */
    CLOCK_DRV_EnableClock(CLOCK_DMAMUX);
    /* Config trigger source for DMA channel 0 to ADC0 */
    DMAMUX_DRV_ChannelDisable(DMAMUX, 0);
    DMAMUX_DRV_ChannelSourceSelect(DMAMUX, 0, DMAMUX_ADC0);
    DMAMUX_DRV_ChannelEnable(DMAMUX, 0);
}

void initFTM() {
    SCG->FIRCCSR &= ~SCG_FIRCCSR_FIRCEN_MASK;
    SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV1(3);      // Divided by 4
    SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN_MASK;
	
    PCC->PCCn[PCC_FTM0_INDEX] &= ~PCC_PCCn_CGC_MASK;
    PCC->PCCn[PCC_FTM0_INDEX] |= PCC_PCCn_PCS(3);
    PCC->PCCn[PCC_FTM0_INDEX] |= PCC_PCCn_CGC_MASK;
	
    ftm_config_t config = {
        .clk_src = EXT_CLK,
        .pre_scale = FTM_PRESCALE_1,
        .modulo = 6000,
        .freq = 0,
    };
    ftm_chnl_prarams_t chnlSetup = {
        .mode = EDGE_ALIGNED_LOW_TRUE,
        .duty = 10,
        .intEnable = 0
    };

    FTM_DRV_Init(FTM0, &config, SystemCoreClock/4);

    FTM_DRV_SetupChannel(FTM0, FTM_Chnl_0, &config, &chnlSetup);
    FTM_DRV_SetupChannel(FTM0, FTM_Chnl_1, &config, &chnlSetup);
    FTM_DRV_SetupChannel(FTM0, FTM_Chnl_2, &config, &chnlSetup);

    FTM_DRV_StartCounters(FTM0, &config);
}
/******************************************************************************
 * EOF
 ******************************************************************************/
