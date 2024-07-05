#ifndef DRIVERS_DRIVER_GPIO_H_
#define DRIVERS_DRIVER_GPIO_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "S32K144.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
/* @brief GPIO direction definition */
typedef enum _gpio_pin_direction
{
    GPIO_DigitalInput  = 0U,   /* Set current pin as digital input */
    GPIO_DigitalOutput = 1U,   /* Set current pin as digital output */
} gpio_pin_direction_t;

/* @brief The GPIO pin configuration structure */
typedef struct _gpio_pin_config
{
    gpio_pin_direction_t pinDirection; /* GPIO direction, input or output */
    /* Output configurations; ignore if configured as an input pin */
    uint8_t outputLogic; /* Set a default output logic */
} gpio_pin_config_t;

/******************************************************************************
 * API
 ******************************************************************************/
/**
 * @brief Initializes a GPIO pin used by the board.
 *
 * @param base    GPIO peripheral base pointer (PTA, PTB, PTC, ...)
 * @param pin     GPIO port pin number
 * @param config  GPIO pin configuration pointer
 */
void GPIO_DRV_PinInit(GPIO_Type *base,uint32_t pin,
                      const gpio_pin_config_t *config);

/**
 * @brief Sets the output level of the multiple FGPIO pins to the logic 1 or 0.
 *
 * @param base    GPIO peripheral base pointer (PTA, PTB, PTC, ...)
 * @param pin     GPIO pin number
 * @param output  GPIOpin output logic level.
 *        - 0: corresponding pin output low-logic level.
 *        - 1: corresponding pin output high-logic level.
 */
static inline void GPIO_DRV_PinWrite(GPIO_Type *base, uint32_t pin,
                                                      uint8_t output)
{
    if (output == 0U)
    {
        base->PCOR = (1UL << pin);
    }
    else
    {
        base->PSOR = (1UL << pin);
    }
}

/**
 * @brief Reverses the current output logic of the multiple FGPIO pins.
 *
 * @param base  GPIO peripheral base pointer (PTA, PTB, PTC, ...)
 * @param mask  GPIO pin number
 */
static inline void GPIO_DRV_PortToggle(GPIO_Type *base, uint32_t pin)
{
    base->PTOR |= (1U << pin);
}

/**
 * @brief Reads the current input value of the GPIO port.
 *
 * @param base GPIO peripheral base pointer (PTA, PTB, PTC, and so on.)
 * @param pin  GPIO pin number
 * @retval GPIO port input value
 *        - 0: corresponding pin input low-logic level.
 *        - 1: corresponding pin input high-logic level.
 */
static inline uint32_t GPIO_DRV_PinRead(GPIO_Type *base, uint32_t pin)
{
    return (((base->PDIR) >> pin) & 0x01U);
}

#endif /* DRIVERS_DRIVER_GPIO_H_ */

/******************************************************************************
 * EOF
 ******************************************************************************/
