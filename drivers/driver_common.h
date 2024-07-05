#ifndef DRIVERS_DRIVER_COMMON_H_
#define DRIVERS_DRIVER_COMMON_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "S32K144.h"
#include "S32K144_features.h"
#include "system_S32K144.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef   __STATIC_FORCEINLINE
  #define __STATIC_FORCEINLINE      __attribute__((always_inline)) static inline
#endif
/*******************************************************************************
 * API
 ******************************************************************************/
/**
 * @brief   Disable IRQ Interrupts
 *
 * Disables IRQ interrupts by setting special-purpose register PRIMASK.
 *
 *         Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __disable_irq(void)
{
	__asm volatile ("cpsid i" : : : "memory");
}

/**
 * @brief   Get Priority Mask
 *
 * Returns the current state of the priority mask bit from the Priority Mask Register.
 *
 * @return               Priority Mask value
 */
__STATIC_FORCEINLINE uint32_t __get_PRIMASK(void)
{
  uint32_t result;

  __asm volatile ("MRS %0, primask" : "=r" (result) );
  return(result);
}

/**
 * @brief   Set Priority Mask
 * Assigns the given value to the Priority Mask Register.
 * @param [in]    priMask  Priority Mask
 */
__STATIC_FORCEINLINE void __set_PRIMASK(uint32_t priMask)
{
	__asm volatile ("MSR primask, %0" : : "r" (priMask) : "memory");
}

/**
 * @brief Disable the global IRQ
 *
 * Disable the global interrupt and return the current primask register. User is required to provided the primask
 * register for the EnableGlobalIRQ().
 *
 * @return Current primask value.
 */
static inline uint32_t DisableGlobalIRQ(void)
{
    uint32_t mask = __get_PRIMASK();

    __disable_irq();

    return mask;
}

/**
 * @brief Enable the global IRQ
 *
 * Set the primask register with the provided primask value but not just enable the primask. The idea is for the
 * convenience of integration of RTOS. some RTOS get its own management mechanism of primask. User is required to
 * use the EnableGlobalIRQ() and DisableGlobalIRQ() in pair.
 *
 * @param primask value of primask register to be restored. The primask value is supposed to be provided by the
 * DisableGlobalIRQ().
 */
static inline void EnableGlobalIRQ(uint32_t primask)
{
    __set_PRIMASK(primask);
}

#endif /* DRIVERS_DRIVER_COMMON_H_ */

/******************************************************************************
 * EOF
 ******************************************************************************/
