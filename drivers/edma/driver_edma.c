/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_uart.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Code
 ******************************************************************************/
//void EDMA_DRV_Init(edma_state_t *edmaState,
//                   const edma_user_config_t *userConfig,
//                   edma_chn_state_t * const chnStateArray[],
//                   const edma_channel_config_t * const chnConfigArray[],
//                   uint32_t chnCount)
//{
//    uint32_t index = 0U;
//    DMA_Type *edmaRegBase = NULL;
//    IRQn_Type irqNumber = NotAvail_IRQn;
//    status_t edmaStatus = STATUS_SUCCESS;
//    status_t chnInitStatus = STATUS_SUCCESS;
//#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
//    uint32_t freq = 0U;
//    status_t clockManagerStatus = STATUS_SUCCESS;
//#endif
//
//    /* Check the state and configuration structure pointers are valid */
//    DEV_ASSERT((edmaState != NULL) && (userConfig != NULL));
//
//    /* Check the module has not already been initialized */
//    DEV_ASSERT(s_virtEdmaState == NULL);
//
//#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
//    /* Check that eDMA and DMAMUX modules are clock gated on */
//    for (index = 0U; index < (uint32_t)DMA_INSTANCE_COUNT; index++)
//    {
//        clockManagerStatus = CLOCK_SYS_GetFreq(s_edmaClockNames[index], &freq);
//        DEV_ASSERT(clockManagerStatus == STATUS_SUCCESS);
//    }
//#ifdef FEATURE_DMAMUX_AVAILABLE
//    for (index = 0U; index < (uint32_t)DMAMUX_INSTANCE_COUNT; index++)
//    {
//        clockManagerStatus = CLOCK_SYS_GetFreq(s_dmamuxClockNames[index], &freq);
//        DEV_ASSERT(clockManagerStatus == STATUS_SUCCESS);
//    }
//#endif /* FEATURE_DMAMUX_AVAILABLE */
//#endif /* (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT) */
//
//    /* Save the runtime state structure for the driver */
//    s_virtEdmaState = edmaState;
//
//    /* Clear the state structure. */
//    EDMA_DRV_ClearStructure((uint8_t *)s_virtEdmaState, sizeof(edma_state_t));
//
//    /* Init all DMA instances */
//    for(index = 0U; index < (uint32_t)DMA_INSTANCE_COUNT; index++)
//    {
//        edmaRegBase = s_edmaBase[index];
//
//        /* Init eDMA module on hardware level. */
//        EDMA_Init(edmaRegBase);
//
//#ifdef FEATURE_DMA_HWV3
//        /* Set arbitration mode */
//        EDMA_SetChannelArbitrationMode(edmaRegBase, userConfig->chnArbitration);
//#else /* FEATURE_DMA_HWV3 */
//        /* Set arbitration mode */
//        EDMA_SetChannelArbitrationMode(edmaRegBase, userConfig->chnArbitration);
//#if (FEATURE_DMA_CHANNEL_GROUP_COUNT > 0x1U)
//        EDMA_SetGroupArbitrationMode(edmaRegBase, userConfig->groupArbitration);
//        EDMA_SetGroupPriority(edmaRegBase, userConfig->groupPriority);
//#endif /* (FEATURE_DMA_CHANNEL_GROUP_COUNT > 0x1U) */
//#endif /* FEATURE_DMA_HWV3 */
//        /* Set 'Halt on error' configuration */
//        EDMA_SetHaltOnErrorCmd(edmaRegBase, userConfig->haltOnError);
//    }
//
//#if defined FEATURE_DMA_HAS_ERROR_IRQ
//    /* Enable the error interrupts for eDMA module. */
//    for (index = 0U; index < (uint32_t)FEATURE_DMA_VIRTUAL_ERROR_INTERRUPT_LINES; index++)
//    {
//        /* Enable channel interrupt ID. */
//        irqNumber = s_edmaErrIrqId[index];
//        INT_SYS_EnableIRQ(irqNumber);
//    }
//#endif
//
//    /* Register all edma channel interrupt handlers into vector table. */
//    for (index = 0U; index < (uint32_t)FEATURE_DMA_VIRTUAL_CHANNELS_INTERRUPT_LINES; index++)
//    {
//        /* Enable channel interrupt ID. */
//        irqNumber = s_edmaIrqId[index];
//        INT_SYS_EnableIRQ(irqNumber);
//    }
//
//#ifdef FEATURE_DMAMUX_AVAILABLE
//    /* Initialize all DMAMUX instances */
//    for (index = 0U; index < (uint32_t)DMAMUX_INSTANCE_COUNT; index++)
//    {
//        DMAMUX_Init(s_dmaMuxBase[index]);
//    }
//#endif
//
//    /* Initialize the channels based on configuration list */
//    if ((chnStateArray != NULL) && (chnConfigArray != NULL))
//    {
//        for (index = 0U; index < chnCount; index++)
//        {
//            chnInitStatus = EDMA_DRV_ChannelInit(chnStateArray[index], chnConfigArray[index]);
//            if (chnInitStatus != STATUS_SUCCESS)
//            {
//                edmaStatus = chnInitStatus;
//            }
//        }
//    }
//
//    return edmaStatus;
//}
/******************************************************************************
 * EOF
 ******************************************************************************/


