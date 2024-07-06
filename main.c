/******************************************************************************
 * Includes
 ******************************************************************************/
#include "app_init.h"
#include "encode.h"
#include "queue.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
#define DOUBLE_CLICK_TIME   500
#define ADC_RESOLUTION      4095
#define ADC_UPDATE_DUR      200

typedef enum {
    NONE,
    SINGLE_CLICK,
    DOUBLE_CLICK,
    LONG_CLICK
} ButtonState;

ButtonState sw2State = NONE;
ButtonState sw3State = NONE;

/******************************************************************************
 * Global variables
 ******************************************************************************/
volatile char temp;

volatile uint32_t tickCount          = 0;

volatile uint8_t  checkSW2           = 0;
volatile uint32_t firstSW2Press      = 0;
volatile uint32_t pressSW2Count      = 0;

volatile uint8_t  checkSW3           = 0;
volatile uint32_t firstSW3Press      = 0;
volatile uint32_t pressSW3Count      = 0;

volatile uint8_t volume = 0;
volatile uint32_t current_adc_value  = 0;

volatile uint8_t  vol_flag           = 0;

/******************************************************************************
 * Prototypes
 ******************************************************************************/

/******************************************************************************
 * Functions
 ******************************************************************************/
static inline uint8_t adc_value_to_volume(uint32_t value)
{
    return (uint8_t)(value * 100 / ADC_RESOLUTION + 1);
}

static inline void Check_ADC()
{
    static uint32_t update_time = ADC_UPDATE_DUR;
    if(tickCount > update_time)
    {   
        update_time += ADC_UPDATE_DUR;
        if(volume != adc_value_to_volume(current_adc_value))
        {
            vol_flag = 1;
            volume = adc_value_to_volume(current_adc_value);
        }
    }
}

static inline void Check_SW2()
{
	static uint32_t preStateCount = 0;
	uint32_t temp = pressSW2Count;
	uint32_t diff = temp - preStateCount;
    if(diff == 1 && checkSW2 == 0)
    {
        firstSW2Press = tickCount;
        checkSW2 = 1;
    }
    else if(diff == 2)
    {
        sw2State = DOUBLE_CLICK;
        preStateCount = temp;
        checkSW2 = 0;
    }
    else if((tickCount - firstSW2Press > 500) && diff == 1)
    {
        sw2State = SINGLE_CLICK;
        preStateCount = temp;
        checkSW2 = 0;
    }
    else
    {
        /* Nothing */
    }
}

static inline void Check_SW3()
{
	static uint32_t preStateCount = 0;
	uint32_t temp = pressSW3Count;
	uint32_t diff = temp - preStateCount;
	if(diff == 1 && checkSW3 == 0)
	{
		firstSW3Press = tickCount;
		checkSW3 = 1;
	}
	else if(diff == 2)
	{
		sw3State = DOUBLE_CLICK;
		preStateCount = temp;
		checkSW3 = 0;
	}
	else if((tickCount - firstSW3Press > 500) && diff == 1)
	{
		sw3State = SINGLE_CLICK;
		preStateCount = temp;
		checkSW3 = 0;
	}
	else
	{
		/* Nothing */
	}
}

/******************************************************************************
 * IRQ handlers
 ******************************************************************************/
void LPUART1_RxTx_IRQHandler(void)
{
	if(LPUART1->STAT & LPUART_STAT_RDRF_MASK) {
		temp = LPUART_DRV_ReadByte(LPUART1);
        queue_put_data(temp);
		// if (temp == OPTION_PLAYING) {
		// 	GPIO_DRV_PinWrite(PTD, LED_RED_PIN, 0);
		// } else if (temp == OPTION_PAUSE) {
		// 	GPIO_DRV_PinWrite(PTD, LED_RED_PIN, 1);
		// }

	}
//    LPUART_DRV_WriteByte(LPUART1, current_adc_value);
}

void PORTC_IRQHandler(void)
{
    if (PORT_DRV_CheckPinInterruptFlags(PORTC, SWITCH_2_PIN))
    {
        PORT_DRV_ClearPinsInterruptFlags(PORTC, (1u << SWITCH_2_PIN));
        pressSW2Count++;
    }
    else if (PORT_DRV_CheckPinInterruptFlags(PORTC, SWITCH_3_PIN))
    {
        PORT_DRV_ClearPinsInterruptFlags(PORTC, (1u << SWITCH_3_PIN));
        pressSW3Count++;
    }
    else
    {
        /* Nothing */
    }
}

void SysTick_Handler()
{
    tickCount++;
}

/******************************************************************************
 * Main
 ******************************************************************************/
int main(void)
{
    initSCG();
    initGPIO();
    initUART();
    initADC();
    initLPIT();
    initDMA((uint32_t)&current_adc_value);
    initSIM();

    SysTick_Config(SystemCoreClock/1000);
    NVIC_EnableIRQ(SysTick_IRQn);

    while(1) {
        Check_ADC();
        Check_SW2();
        Check_SW3();

        switch (sw2State)
        {
            case SINGLE_CLICK:
                push_message(OPTION_UP, MESSAGE_DEFAULT_VALUE);
                sw2State = NONE;
                break;
            case DOUBLE_CLICK:
                push_message(OPTION_FORWARD, MESSAGE_DEFAULT_VALUE);
                sw2State = NONE;
                break;
            default:
                break;
        }

        switch (sw3State)
        {
            case SINGLE_CLICK:
                push_message(OPTION_CONFIRM, MESSAGE_DEFAULT_VALUE);
                sw3State = NONE;
                break;
            case DOUBLE_CLICK:
                push_message(OPTION_GO_BACK, MESSAGE_DEFAULT_VALUE);
                sw3State = NONE;
                break;
            default:
                break;
        }

        if(vol_flag)
        {
            push_message(OPTION_VOLTAGE, volume);
            vol_flag = 0;
        }

        if (!queue_empty()) {
            uint8_t* data = queue_get_data();
            if(checkReceiveCommandValid(data) == MESSAGE_CORRECT) {
                if (data[MEASSAGE_OPTION_BYTE] == OPTION_PLAYING) {
                    GPIO_DRV_PinWrite(PTD, LED_RED_PIN, 0);
                } else if (data[MEASSAGE_OPTION_BYTE] == OPTION_PAUSE){
                    GPIO_DRV_PinWrite(PTD, LED_RED_PIN, 1);
                }
            }
        }
    }
    return 0;
}

/******************************************************************************
 * EOF
 ******************************************************************************/
