/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_lpit.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Code
 ******************************************************************************/
/**
 * brief Ungates the LPIT clock and configures the peripheral for a basic operation.
 *
 * This function issues a software reset to reset all channels and registers except the Module
 * Control register.
 *
 * note This API should be called at the beginning of the application using the LPIT driver.
 *
 * param base   LPIT peripheral base address.
 * param config Pointer to the user configuration structure.
 */
void LPIT_DRV_Init(LPIT_Type *base, const lpit_config_t *config)
{
    assert(NULL != config);

    /* Reset the timer channels and registers except the MCR register */
    LPIT_DRV_Reset(base);

    /* Setup timer operation in debug and doze modes and enable the module */
    base->MCR = (LPIT_MCR_DBG_EN(config->enableRunInDebug) |
                 LPIT_MCR_DOZE_EN(config->enableRunInDoze) |
                 LPIT_MCR_M_CEN_MASK);
}

void LPIT_DRV_SetupChannel(LPIT_Type *base, lpit_chnl_t channel, const lpit_chnl_params_t *chnlSetup)
{
    assert(NULL != chnlSetup);

    uint32_t reg = 0U;

    /* Setup the channel counters operation mode, trigger operation, chain mode */
    reg = (LPIT_TMR_TCTRL_MODE(chnlSetup->timerMode)             |
           LPIT_TMR_TCTRL_TRG_SRC(chnlSetup->triggerSource)      |
           LPIT_TMR_TCTRL_TRG_SEL(chnlSetup->triggerSelect)      |
           LPIT_TMR_TCTRL_TROT(chnlSetup->enableReloadOnTrigger) |
           LPIT_TMR_TCTRL_TSOI(chnlSetup->enableStopOnTimeout)   |
           LPIT_TMR_TCTRL_TSOT(chnlSetup->enableStartOnTrigger)  |
           LPIT_TMR_TCTRL_CHAIN(chnlSetup->chainChannel));

    base->TMR[channel].TCTRL = reg;
}

/******************************************************************************
 * EOF
 ******************************************************************************/
