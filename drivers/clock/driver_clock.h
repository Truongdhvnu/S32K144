#ifndef DRIVERS_DRIVER_CLOCK_H_
#define DRIVERS_DRIVER_CLOCK_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_common.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
/**
 * @brief Peripheral clock name definition used for clock gate, clock source
 * and clock divider setting
 */
typedef enum _clock_ip_name
{
    CLOCK_PORTA   = PCC_PORTA_INDEX,
    CLOCK_PORTB   = PCC_PORTB_INDEX,
    CLOCK_PORTC   = PCC_PORTC_INDEX,
    CLOCK_PORTD   = PCC_PORTD_INDEX,
    CLOCK_PORTE   = PCC_PORTE_INDEX,
    CLOCK_ADC0    = PCC_ADC0_INDEX,
    CLOCK_LPUART1 = PCC_LPUART1_INDEX,
    CLOCK_LPIT    = PCC_LPIT_INDEX,
} clock_ip_name_t;

/* @brief Clock source for peripherals that support various clock selections. */
typedef enum _clock_ip_src
{
    CLOCK_IpSrcNoneOrExt   = 0U, /*!< Clock is off or external clock is used. */
    CLOCK_IpSrcSysOscAsync = 1U, /*!< System Oscillator async clock.          */
    CLOCK_IpSrcSircAsync   = 2U, /*!< Slow IRC async clock.                   */
    CLOCK_IpSrcFircAsync   = 3U, /*!< Fast IRC async clock.                   */
    CLOCK_IpSrcLpFllAsync  = 5U  /*!< LPFLL async clock.                      */
} clock_ip_src_t;

/* @brief SCG asynchronous clock type. */
typedef enum _scg_async_clk
{
    SCG_AsyncDiv1Clk, /*!< The async clock by DIV1, e.g. SIRCDIV1_CLK. */
    SCG_AsyncDiv2Clk, /*!< The async clock by DIV2, e.g. SIRCDIV2_CLK. */
} scg_async_clk_t;

/* @brief SCG asynchronous clock divider value. */
typedef enum _scg_async_clk_div
{
    SCG_AsyncClkDisable = 0U, /*!< Clock output is disabled.  */
    SCG_AsyncClkDivBy1  = 1U, /*!< Divided by 1.              */
    SCG_AsyncClkDivBy2  = 2U, /*!< Divided by 2.              */
    SCG_AsyncClkDivBy4  = 3U, /*!< Divided by 4.              */
    SCG_AsyncClkDivBy8  = 4U, /*!< Divided by 8.              */
    SCG_AsyncClkDivBy16 = 5U, /*!< Divided by 16.             */
    SCG_AsyncClkDivBy32 = 6U, /*!< Divided by 32.             */
    SCG_AsyncClkDivBy64 = 7U  /*!< Divided by 64.             */
} scg_async_clk_div_t;

/******************************************************************************
 * API
 ******************************************************************************/
/**
 * @brief Enable the clock for specific IP.
 *
 * @param name  Which clock to enable, see clock_ip_name_t.
 */
static inline void CLOCK_DRV_EnableClock(clock_ip_name_t name)
{
    PCC->PCCn[name] |= PCC_PCCn_CGC(1);
}

/**
 * @brief Disable the clock for specific IP.
 *
 * @param name  Which clock to disable, see \ref clock_ip_name_t.
 */
static inline void CLOCK_DRV_DisableClock(clock_ip_name_t name)
{
    PCC->PCCn[name] &= ~PCC_PCCn_CGC_MASK;
}

/**
 * @brief Set the clock source for specific IP module.
 *
 * Set the clock source for specific IP, not all modules need to set the
 * clock source, should only use this function for the modules need source
 * setting.
 *
 * @param name Which peripheral to check, see \ref clock_ip_name_t.
 * @param src Clock source to set.
 */
static inline void CLOCK_DRV_SetIpSrc(clock_ip_name_t name, clock_ip_src_t src)
{
    PCC->PCCn[name] |= PCC_PCCn_PCS(src);
}

/**
 * @brief Set the asynchronous clock divider.
 *
 * @param asyncClk Which asynchronous clock to configure.
 * @param divider The divider value to set.
 *
 * @note There might be glitch when changing the asynchronous divider, so make sure
 * the asynchronous clock is not used while changing divider.
 */
static inline void
CLOCK_DRV_SetFircAsyncClkDiv(scg_async_clk_t asyncClk, scg_async_clk_div_t divider)
{
    uint32_t reg = SCG->FIRCDIV;

    switch (asyncClk)
    {
        case SCG_AsyncDiv1Clk:
            reg = (reg & ~SCG_FIRCDIV_FIRCDIV1_MASK) | SCG_FIRCDIV_FIRCDIV1(divider);
            break;
        case SCG_AsyncDiv2Clk:
            reg = (reg & ~SCG_FIRCDIV_FIRCDIV2_MASK) | SCG_FIRCDIV_FIRCDIV2(divider);
            break;
        default:
            /* The default clause should not be reached. */
            break;
    }

    SCG->FIRCDIV = reg;
}

#endif /* DRIVERS_DRIVER_CLOCK_H_ */

/******************************************************************************
 * EOF
 ******************************************************************************/
