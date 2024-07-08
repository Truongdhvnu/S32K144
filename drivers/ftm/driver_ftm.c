/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_ftm.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Code
 ******************************************************************************/

/**
 * brief Configure the FTM with a configuration struture
 * User can only set FTM with a certain Frequency or Resolution
 * If user want to set a certain frequency, set the resolution equal 0 and vice versa
 * 
 * The Resolution must be less than FTM_MODULO_MAX (65535)
 * 
 * note This API should be called at the beginning of the application using the FTM driver.
 */
void FTM_DRV_Init(FTM_Type *base, ftm_config_t* config, uint32_t srcClock_freq) {
    assert(NULL != config);

    FTM_DRV_Write_Protect_Disable();

    base->SC |= FTM_SC_PS(config->pre_scale);

    base->COMBINE = 0x00000000;	/* FTM mode settings used: DECAPENx, MCOMBINEx, COMBINEx=0  */
	base->POL = 0x00000000;    	/* Polarity for all channels is active high (default) 	    */

    if (config->freq == 0) {
        assert(config->resolution < FTM_MODULO_MAX);
        base->MOD = config->resolution;
        config->freq = srcClock_freq / ((config->resolution + 1) * config->pre_scale);
    } else if (config->freq != 0) {
        uint32_t mod = (srcClock_freq / (config->freq * config->pre_scale)) - 1;
        assert(mod < FTM_MODULO_MAX);
        base->MOD = mod;
    }
}

void FTM_DRV_SetupChannel(FTM_Type *base, ftm_chnl_t channel, ftm_config_t* config,
                                        const ftm_chnl_prarams_t* chnlSetup) {
                    
    // assert(NULL != chnlSetup);
    
    switch (chnlSetup->mode) {
        case EDGE_ALIGNED_LOW_TRUE:
            base->SC |= (uint32_t)(1u << (channel + FTM_PWMEN_BASE_SHIFT));
            base->CONTROLS[channel].CnSC = FTM_CnSC_MSB_MASK
							               |FTM_CnSC_ELSB_MASK
                                           |FTM_CnSC_CHIE(chnlSetup->intEnable);
            base->CONTROLS[channel].CnV = config->resolution * chnlSetup->duty / 100;
            break;
        default:
            break;
    }
}
