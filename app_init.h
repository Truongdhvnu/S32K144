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

/******************************************************************************
 * API
 ******************************************************************************/
/* @brief Initialize the Clock module. */
void initSCG();

/* @brief Initialize the GPIO module. */
void initGPIO();

/* @brief Initialize the ADC module. */
void initADC();

/* @brief Initialize the UART module. */
void initUART();

/* @brief Initialize the LPIT module. */
void initLPIT();

/* @brief Initialize the SIM module. */
void initSIM();

/**
 * @brief Initialize the DMA module.
 *
 * @param storing_address   address to store DMA destination.
 */
void initDMA(uint32_t storing_address);

#endif /* APP_INIT_H_ */

/******************************************************************************
 * EOF
 ******************************************************************************/
