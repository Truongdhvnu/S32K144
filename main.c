/******************************************************************************
 * Includes
 ******************************************************************************/
#include "app_init.h"
#include "encode.h"
#include "queue.h"
#include "driver_ftm.h"
/******************************************************************************
 * Definitions
 ******************************************************************************/
#define DOUBLE_CLICK_TIME   500
#define ADC_RESOLUTION      4095
#define ADC_UPDATE_DUR      200
#define COLOUR_NUMBERS 		24
#define LED_CHANGE_DUR		200

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

volatile uint8_t  playing_flag       = 0;


uint8_t colors[COLOUR_NUMBERS][3] = {
    {255, 0, 0},
    {255, 64, 0},
    {255, 127, 0},
    {255, 191, 0},
    {255, 255, 0},
    {191, 255, 0},
    {127, 255, 0},
    {64, 255, 0},
    {0, 255, 0},
    {0, 255, 64},
    {0, 255, 127},
    {0, 255, 191},
    {0, 255, 255},
    {0, 191, 255},
    {0, 127, 255},
    {0, 64, 255},
    {0, 0, 255},
    {37, 0, 230},
    {75, 0, 205},
    {111, 0, 180},
    {148, 0, 155},
    {185, 0, 130},
    {222, 0, 105},
    {255, 0, 80}
};

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
	uint32_t count_diff = temp - preStateCount;
    if(count_diff == 1 && checkSW2 == 0)
    {
        firstSW2Press = tickCount;
        checkSW2 = 1;
    }
    else if(count_diff == 2)
    {
        sw2State = DOUBLE_CLICK;
        preStateCount = temp;
        checkSW2 = 0;
    }
    else if((tickCount - firstSW2Press > DOUBLE_CLICK_TIME) && count_diff == 1)
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
	uint32_t count_diff = temp - preStateCount;
	if(count_diff == 1 && checkSW3 == 0)
	{
		firstSW3Press = tickCount;
		checkSW3 = 1;
	}
	else if(count_diff == 2)
	{
		sw3State = DOUBLE_CLICK;
		preStateCount = temp;
		checkSW3 = 0;
	}
	else if((tickCount - firstSW3Press > DOUBLE_CLICK_TIME) && count_diff == 1)
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

void change_colour();
void turn_off_led();

static inline void Check_Playing() {
    if (playing_flag == 0){
    	turn_off_led();
    } else if (tickCount % LED_CHANGE_DUR == 0 && playing_flag == 1) {
        change_colour();
    }
}
/******************************************************************************
 * IRQ handlers
 ******************************************************************************/
void LPUART1_RxTx_IRQHandler(void)
{
	if(LPUART1->STAT & LPUART_STAT_RDRF_MASK) {
        queue_put_data(LPUART_DRV_ReadByte(LPUART1));
	}
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
    initFTM();

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
                    playing_flag = 1;
                } else if (data[MEASSAGE_OPTION_BYTE] == OPTION_PAUSE){
                    playing_flag = 0;
                }
            }
        }

        Check_Playing();
    }
    return 0;
}

void change_colour() {
	static int index = 0;
    FTM_DRV_setDutyCycle(FTM0, FTM_Chnl_0, (uint32_t)(colors[index][0] / 2.55));
    FTM_DRV_setDutyCycle(FTM0, FTM_Chnl_1, (uint32_t)(colors[index][1] / 2.55));
    FTM_DRV_setDutyCycle(FTM0, FTM_Chnl_2, (uint32_t)(colors[index][2] / 2.55));
	index++;
	index = index % COLOUR_NUMBERS;
}

void turn_off_led() {
    FTM_DRV_setDutyCycle(FTM0, FTM_Chnl_0, 100);
    FTM_DRV_setDutyCycle(FTM0, FTM_Chnl_1, 100);
    FTM_DRV_setDutyCycle(FTM0, FTM_Chnl_2, 100);
}

/******************************************************************************
 * EOF
 ******************************************************************************/
