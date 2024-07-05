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
#include "encode.h"
#include "driver_systick.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

#define LED_BLUE_PIN	    0
#define LED_RED_PIN		    15
#define LED_GREEN_PIN	    16
#define SWITCH_2_PIN 	    12
#define SWITCH_3_PIN 	    13

#define DOUBLE_CLICK_TIME   500
#define ADC_RESOLUTION      4095

/******************************************************************************
 * Global variables
 ******************************************************************************/
volatile char temp;

volatile uint32_t tickCount = 0;

volatile uint8_t checkSW2 = 0;
volatile uint32_t firstSW2Press = 0;
volatile uint32_t pressSW2Count = 0;

volatile uint8_t checkSW3 = 0;
volatile uint32_t firstSW3Press = 0;
volatile uint32_t pressSW3Count = 0;

volatile uint32_t previous_adc_value = 0;
volatile uint32_t current_adc_value = 0;

volatile uint8_t vol_flag = 0;

typedef enum {
    NONE,
    SINGLE_CLICK,
    DOUBLE_CLICK,
    LONG_CLICK
} ButtonState;

ButtonState sw2State = NONE;
ButtonState sw3State = NONE;

/******************************************************************************
 * Functions
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

    TRGMUX_DRV_SetTriggerSource(TRGMUX, TRGMUX_ADC0_INDEX, TRGMUX_TriggerInput0, TRGMUX_Source_LPIT_CH0);
}

void initDMA()
{
    dma_channel_config_t config =
    {
        .srcAddr          = DMA_TCD_SADDR_SADDR(&(ADC0->R[0])),
        .srcTransferSize  = DMA_TRANSFER_SIZE_4B,
        .destAddr         = DMA_TCD_DADDR_DADDR(&current_adc_value),
        .destTransferSize = DMA_TRANSFER_SIZE_4B,
    };

    DMA_DRV_SetChannelConfig(DMA, 0, &config);

    CLOCK_DRV_EnableClock(CLOCK_DMAMUX);
    DMAMUX_DRV_ChannelDisable(DMAMUX, 0);
    DMAMUX_DRV_ChannelSourceSelect(DMAMUX, 0, DMAMUX_ADC0);
    DMAMUX_DRV_ChannelEnable(DMAMUX, 0);
}

/******************************************************************************
 * IRQ handlers
 ******************************************************************************/
void LPUART1_RxTx_IRQHandler(void)
{
    temp = LPUART_DRV_ReadByte(LPUART1);
    LPUART_DRV_WriteByte(LPUART1, current_adc_value);
}

void PORTC_IRQHandler(void)
{
    if (PORT_DRV_CheckPinInterruptFlags(PORTC, SWITCH_2_PIN)) {
        PORT_DRV_ClearPinsInterruptFlags(PORTC, (1u << SWITCH_2_PIN));
        pressSW2Count++;
        GPIO_DRV_PortToggle(PTD, LED_BLUE_PIN);
	} else if (PORT_DRV_CheckPinInterruptFlags(PORTC, SWITCH_3_PIN)) {
        PORT_DRV_ClearPinsInterruptFlags(PORTC, (1u << SWITCH_3_PIN));
        pressSW3Count++;
        GPIO_DRV_PortToggle(PTD, LED_RED_PIN);
	}
}

void SysTick_Handler() {
	tickCount++;
	if(tickCount % 200 == 0) {
		if(previous_adc_value != current_adc_value){
			vol_flag = 1;
			previous_adc_value = current_adc_value;
		}
	}
	// Check SW2
	if(pressSW2Count == 1 && checkSW2 == 0) {
		firstSW2Press = tickCount;
		checkSW2 = 1;
	}
	if(pressSW2Count == 2) {
		sw2State = DOUBLE_CLICK;
		pressSW2Count = 0;
		checkSW2 = 0;
	}
	else if((tickCount - firstSW2Press > 500) && pressSW2Count == 1){
		sw2State = SINGLE_CLICK;
		pressSW2Count = 0;
		checkSW2 = 0;
	}

	// Check SW3
	if(pressSW3Count == 1 && checkSW3 == 0) {
		firstSW3Press = tickCount;
		checkSW3 = 1;
	}
	if(pressSW3Count == 2) {
		sw3State = DOUBLE_CLICK;
		pressSW3Count = 0;
		checkSW3 = 0;
	}
	else if((tickCount - firstSW3Press > 500) && pressSW3Count == 1){
		sw3State = SINGLE_CLICK;
		pressSW3Count = 0;
		checkSW3 = 0;
	}
}

/******************************************************************************
 * Main
 ******************************************************************************/
int main(void)
{
    initClock();
    initGPIO();
    initUART();
    initADC();
    initLPIT();
    initDMA();
    initSIM();

    SysTick_Config(SystemCoreClock/1000);
    NVIC_EnableIRQ(SysTick_IRQn);

    char* data = "Start\n";

    LPUART_DRV_WriteBlocking(LPUART1, (uint8_t *)data, 6);

    while(1) {
    	switch (sw2State) {
    	case SINGLE_CLICK:
//    		UART1_SendChar('n');
    		push_message('5', '0');
    		sw2State = NONE;
    		break;
    	case DOUBLE_CLICK:
//    		UART1_SendChar('p');
    		push_message('6', '0');
    		sw2State = NONE;
    		break;
    	case LONG_CLICK:
    		break;
    	case NONE:
    		break;
    	}

    	switch (sw3State) {
    	case SINGLE_CLICK:
//    		UART1_SendChar('s');
    		push_message('2', '0');
    		sw3State = NONE;
    		break;
    	case DOUBLE_CLICK:
//    		UART1_SendChar('r');
    		push_message('3', '0');
    		sw3State = NONE;
    		break;
    	case LONG_CLICK:
    		break;
    	case NONE:
    		break;
    	}

    	if(vol_flag) {
			push_message('9', (uint8_t)current_adc_value);
    		vol_flag = 0;
    	}
    }

    return 0;
}

/******************************************************************************
 * EOF
 ******************************************************************************/
