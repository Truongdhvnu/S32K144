/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_dma.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Code
 ******************************************************************************/
void DMA_DRV_SetChannelConfig(DMA_Type * base, uint32_t channel,
                              const dma_channel_config_t *config)
{
    /* Source Address */
    base->TCD[channel].SADDR = DMA_TCD_SADDR_SADDR(config->srcAddr);
    /* Source offset = 0 */
    base->TCD[channel].SOFF = DMA_TCD_SOFF_SOFF(0);
    /* ATTR: source and destination size */
    base->TCD[channel].ATTR = DMA_TCD_ATTR_SMOD(0)                        |
                              DMA_TCD_ATTR_SSIZE(config->srcTransferSize) |
                              DMA_TCD_ATTR_DMOD(0)                        |
                              DMA_TCD_ATTR_DSIZE(config->destTransferSize);
    /* Minor Byte Transfer Count is 4-bytes */
    base->TCD[channel].NBYTES.MLOFFNO = DMA_TCD_NBYTES_MLNO_NBYTES(4);
    /* Last Source Address Adjustment is -0 */
    base->TCD[channel].SLAST = DMA_TCD_SLAST_SLAST(0);
    /* Destination Address of Buffer */
    base->TCD[channel].DADDR = DMA_TCD_DADDR_DADDR(config->destAddr);
    /* Destination Address Signed Offset is 0 */
    base->TCD[channel].DOFF = DMA_TCD_DOFF_DOFF(0);
    /* Current Major Iteration Count is 4, channel-to-channel linking disabled*/
    base->TCD[channel].CITER.ELINKNO = DMA_TCD_CITER_ELINKNO_CITER(4) |
                                       DMA_TCD_CITER_ELINKNO_ELINK(0);
    /* Destination last address adjustment is -0 */
    base->TCD[channel].DLASTSGA = DMA_TCD_DLASTSGA_DLASTSGA(0);
    /* Starting major iteration count is 4, minor channel-to-channel linking disabled */
    base->TCD[channel].BITER.ELINKNO = DMA_TCD_BITER_ELINKNO_BITER(4) |
                                       DMA_TCD_BITER_ELINKNO_ELINK(0);
    /* Clear CSR */
    base->TCD[channel].CSR = DMA_TCD_CSR_BWC(0)         |
                             DMA_TCD_CSR_MAJORELINK(0)  |
                             DMA_TCD_CSR_MAJORLINKCH(0) |
                             DMA_TCD_CSR_ESG(0)         |
                             DMA_TCD_CSR_DREQ(0)        |
                             DMA_TCD_CSR_INTHALF(0)     |
                             DMA_TCD_CSR_INTMAJOR(0)    |
                             DMA_TCD_CSR_START(0);
    /* Enable channel HW trigger */
    base->SERQ = channel;
}
/******************************************************************************
 * EOF
 ******************************************************************************/


