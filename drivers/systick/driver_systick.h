#ifndef DRIVERS_DRIVER_SYSTICK_H_
#define DRIVERS_DRIVER_SYSTICK_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "S32K144.h"

/******************************************************************************
 * API
 ******************************************************************************/
/**
 * @brief Initialises and starts the System Tick Timer and its interrupt.
 *
 * @param ticks  Number of ticks between two interrupts
 * @retval 0 - success
 *         1 - failure
 */
uint32_t SysTick_Config	(uint32_t ticks);

#endif /* DRIVERS_DRIVER_SYSTICK_H_ */

/******************************************************************************
 * EOF
 ******************************************************************************/
