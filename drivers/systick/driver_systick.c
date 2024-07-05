/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_systick.h"

/******************************************************************************
 * Code
 ******************************************************************************/
uint32_t SysTick_Config	(uint32_t ticks)
{
    uint32_t retVal = 0UL;

    if ((ticks - 1U) > S32_SysTick_RVR_RELOAD_MASK)
    {
        retVal = 1U;    /* Reload value impossible */
    }
    else
    {
        /* Set reload register */
        S32_SysTick->RVR  = (uint32_t)(ticks - 1UL);
        /* Load the SysTick Counter Value */
        S32_SysTick->CVR   = 0U;
        S32_SysTick->CSR  = S32_SysTick_CSR_CLKSOURCE(1) |
                            S32_SysTick_CSR_TICKINT(1)   |
                            S32_SysTick_CSR_ENABLE(1);
        retVal = 0U;
	}

    return retVal;
}

/******************************************************************************
 * EOF
 ******************************************************************************/
