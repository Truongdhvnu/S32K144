#ifndef DRIVERS_DMAMUX_DRIVER_DMAMUX_H_
#define DRIVERS_DMAMUX_DRIVER_DMAMUX_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_common.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
/**
 * @brief DMAMUX channel source slot number
 */
typedef enum _dmamux_source
{
    DMAMUX_LPUART0_RX = EDMA_REQ_LPUART0_RX,
    DMAMUX_LPUART0_TX = EDMA_REQ_LPUART0_TX,
    DMAMUX_LPUART1_RX = EDMA_REQ_LPUART1_RX,
    DMAMUX_LPUART1_TX = EDMA_REQ_LPUART1_TX,
    DMAMUX_ADC0       = EDMA_REQ_ADC0,
    DMAMUX_ADC1       = EDMA_REQ_ADC1,
} dmamux_source_t;

/******************************************************************************
 * API
 ******************************************************************************/
/**
 * @brief Enables the DMA channel.
 *
 * @param base DMAMUX peripheral base address.
 * @param channel Channel index.
 */
static inline void DMAMUX_DRV_ChannelEnable(DMAMUX_Type * base, uint8_t channel)
{
    base->CHCFG[channel] |= DMAMUX_CHCFG_ENBL(1);
}

/**
 * @brief Disables the DMA channel.
 *
 * @param base DMAMUX peripheral base address.
 * @param channel Channel index.
 */
static inline void DMAMUX_DRV_ChannelDisable(DMAMUX_Type * base, uint8_t channel)
{
    base->CHCFG[channel] &= ~DMAMUX_CHCFG_ENBL(1);
}

/**
 * @brief Select DMA channel source
 *
 * @param base DMAMUX peripheral base address.
 * @param channel Channel index.
 * @param source trigger source index.
 */
static inline void DMAMUX_DRV_ChannelSourceSelect(DMAMUX_Type * base,
                                                  uint8_t channel,
                                                  dmamux_source_t source)
{
    base->CHCFG[channel] |= DMAMUX_CHCFG_SOURCE(source);
}


#endif /* DRIVERS_DMAMUX_DRIVER_DMAMUX_H_ */

/******************************************************************************
 * EOF
 ******************************************************************************/
