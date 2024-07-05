#ifndef DRIVERS_EDMA_DRIVER_EDMA_H_
#define DRIVERS_EDMA_DRIVER_EDMA_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_common.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * API
 ******************************************************************************/
/**
 * @brief Initializes the eDMA module.
 *
 * This function initializes the run-time state structure to provide the eDMA channel allocation
 * release, protect, and track the state for channels. This function also resets the eDMA modules,
 * initializes the module to user-defined settings and default settings.
 * @param edmaState The pointer to the eDMA peripheral driver state structure. The user passes
 * the memory for this run-time state structure and the eDMA peripheral driver populates the
 * members. This run-time state structure keeps track of the eDMA channels status. The memory must
 * be kept valid before calling the EDMA_DRV_DeInit.
 * @param userConfig User configuration structure for eDMA peripheral drivers. The user populates the
 * members of this structure and passes the pointer of this structure into the function.
 * @param chnStateArray Array of pointers to run-time state structures for eDMA channels;
 * will populate the state structures inside the eDMA driver state structure.
 * @param chnConfigArray Array of pointers to channel initialization structures.
 * @param chnCount The number of eDMA channels to be initialized.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
void EDMA_DRV_Init(edma_state_t * edmaState,
                   const edma_user_config_t *userConfig,
                   edma_chn_state_t * const chnStateArray[],
                   const edma_channel_config_t * const chnConfigArray[],
                   uint32_t chnCount);

#endif /* DRIVERS_EDMA_DRIVER_EDMA_H_ */

/******************************************************************************
 * EOF
 ******************************************************************************/
