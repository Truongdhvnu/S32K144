/******************************************************************************
 * Includes
 ******************************************************************************/
#include "app_init.h"
#include "encode.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
#define DOUBLE_CLICK_TIME   500
#define ADC_RESOLUTION      4095

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
    if(tickCount % 200 == 0)
    {       
        if(volume != adc_value_to_volume(current_adc_value))
        {
            vol_flag = 1;
            volume = adc_value_to_volume(current_adc_value);
        }
    }
}

static inline void Check_SW2()
{
    if(pressSW2Count == 1 && checkSW2 == 0)
    {
        firstSW2Press = tickCount;
        checkSW2 = 1;
    }
    if(pressSW2Count == 2)
    {
        sw2State = DOUBLE_CLICK;
        pressSW2Count = 0;
        checkSW2 = 0;
    }
    else if((tickCount - firstSW2Press > 500) && pressSW2Count == 1)
    {
        sw2State = SINGLE_CLICK;
        pressSW2Count = 0;
        checkSW2 = 0;
    }
    else
    {
        /* Nothing */
    }
}

static inline void Check_SW3()
{
    if(pressSW3Count == 1 && checkSW3 == 0)
    {
        firstSW3Press = tickCount;
        checkSW3 = 1;
    }
    if(pressSW3Count == 2)
    {
        sw3State = DOUBLE_CLICK;
        pressSW3Count = 0;
        checkSW3 = 0;
    }
    else if((tickCount - firstSW3Press > 500) && pressSW3Count == 1)
    {
        sw3State = SINGLE_CLICK;
        pressSW3Count = 0;
        checkSW3 = 0;
    }
    else
    {
        /* Nothing */
    }
}

static inline uint8_t ADC_value_scale(uint32_t value)
{
    return (uint8_t)(value * 100 / ADC_RESOLUTION + 1);
}

/******************************************************************************
 * IRQ handlers
 ******************************************************************************/
void LPUART1_RxTx_IRQHandler(void)
{
    temp = LPUART_DRV_ReadByte(LPUART1);
//    LPUART_DRV_WriteByte(LPUART1, current_adc_value);
}

void PORTC_IRQHandler(void)
{
    if (PORT_DRV_CheckPinInterruptFlags(PORTC, SWITCH_2_PIN))
    {
        PORT_DRV_ClearPinsInterruptFlags(PORTC, (1u << SWITCH_2_PIN));
        pressSW2Count++;
        GPIO_DRV_PortToggle(PTD, LED_BLUE_PIN);
    }
    else if (PORT_DRV_CheckPinInterruptFlags(PORTC, SWITCH_3_PIN))
    {
        PORT_DRV_ClearPinsInterruptFlags(PORTC, (1u << SWITCH_3_PIN));
        pressSW3Count++;
        GPIO_DRV_PortToggle(PTD, LED_RED_PIN);
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
    initClock();
    initGPIO();
    initUART();
    initADC();
    initLPIT();
    initDMA((uint32_t)&current_adc_value);
    initSIM();

    SysTick_Config(SystemCoreClock/1000);
    NVIC_EnableIRQ(SysTick_IRQn);

    char* data = "Start\n";
    LPUART_DRV_WriteBlocking(LPUART1, (uint8_t *)data, 6);

    while(1) {
        Check_ADC();
        Check_SW2();
        Check_SW3();

        switch (sw2State)
        {
            case SINGLE_CLICK:
//                UART1_SendChar('n');
                push_message('5', '0');
                sw2State = NONE;
                break;
            case DOUBLE_CLICK:
//                UART1_SendChar('p');
                push_message('6', '0');
                sw2State = NONE;
                break;
            case LONG_CLICK:
                break;
            case NONE:
                break;
            default:
                /* Nothing */
                break;
        }

        switch (sw3State)
        {
            case SINGLE_CLICK:
//                UART1_SendChar('s');
                push_message('2', '0');
                sw3State = NONE;
                break;
            case DOUBLE_CLICK:
//                UART1_SendChar('r');
                push_message('3', '0');
                sw3State = NONE;
                break;
            case LONG_CLICK:
                break;
            case NONE:
                break;
            default:
                /* Nothing */
                break;
        }

        if(vol_flag)
        {
            push_message('9', volume);
            vol_flag = 0;
        }
    }

    return 0;
}

/******************************************************************************
 * EOF
 ******************************************************************************/
