#ifndef DRIVERS_DMA_DRIVER_EDMA_H_
#define DRIVERS_DMA_DRIVER_EDMA_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_common.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
/* @brief DMA transfer size */
typedef enum {
    DMA_TRANSFER_SIZE_1B  = 0x0U,
    DMA_TRANSFER_SIZE_2B  = 0x1U,
    DMA_TRANSFER_SIZE_4B  = 0x2U,
} edma_transfer_size_t;

/* @brief DMA configuration structure. */
typedef struct _dma_channel_config
{
    uint32_t srcAddr;                                 /*!< Memory address pointing to the source data. */
    uint32_t destAddr;                                /*!< Memory address pointing to the destination data. */
    edma_transfer_size_t srcTransferSize;             /*!< Source data transfer size. */
    edma_transfer_size_t destTransferSize;            /*!< Destination data transfer size. */
} dma_channel_config_t;

/******************************************************************************
 * API
 ******************************************************************************/
/**
 * @brief Setting channel config for DMA.
 *
 * @param base     DMA peripheral base address.
 * @param channel  Channel index.
 * @param config   Pointer to "dma_channel_config_t" structure.
 */
void DMA_DRV_SetChannelConfig(DMA_Type * base, uint32_t channel,
                              const dma_channel_config_t *config);

/**
 * @brief Starts an DMA channel.
 *
 * @param base DMA  peripheral base address.
 * @param channel   DMA channel number.
 */
static inline void DMA_DRV_StartChannel(DMA_Type * base, uint8_t channel)
{
    base->TCD[channel].CSR |= DMAMUX_CHCFG_SOURCE(EDMA_REQ_ADC0);
}


#endif /* DRIVERS_DMA_DRIVER_EDMA_H_ */

/******************************************************************************
 * EOF
 ******************************************************************************/
