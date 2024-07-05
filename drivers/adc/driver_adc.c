/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_adc.h"

/******************************************************************************
 * Code
 ******************************************************************************/
void ADC_DRV_Init(ADC_Type *base, const adc_config_t *config)
{
    assert(config);

    uint32_t tmp32;

    /* ADCx_CFG1 */
    tmp32  = (base->CFG1 &
            ~(ADC_CFG1_ADICLK_MASK | ADC_CFG1_ADIV_MASK | ADC_CFG1_MODE_MASK));
    tmp32 |= (ADC_CFG1_ADICLK(config->clockSource) |
              ADC_CFG1_ADIV(config->clockDivider)  |
              ADC_CFG1_MODE(config->resolution));
    base->CFG1 = tmp32;

    /* ADCx_CFG2. */
    tmp32  = (base->CFG2 & ~ADC_CFG2_SMPLTS_MASK);
    tmp32 |= ADC_CFG2_SMPLTS(config->sampleClockCount - 1U);
    base->CFG2 = tmp32;

    /* ADCx_SC2. */
    tmp32  = (base->SC2 &
              ~(ADC_SC2_ADTRG_MASK | ADC_SC2_REFSEL_MASK | ADC_SC2_DMAEN_MASK));
    tmp32 |= (ADC_SC2_ADTRG(config->triggerType) |
              ADC_SC2_REFSEL(config->referenceVoltageSource));
    if (config->dmaEnable)
    {
        tmp32 |= ADC_SC2_DMAEN(1);
    }
    base->SC2 = tmp32;

    /* ADCx_SC3. */
    tmp32 = (base->SC3 & ~ADC_SC3_ADCO_MASK);
    if (true == config->enableContinuousConversion)
    {
        tmp32 |= ADC_SC3_ADCO_MASK;
    }
    base->SC3 = tmp32;
}

void ADC_DRV_SetChannelConfig(ADC_Type *base, uint32_t channelGroup,
                            const adc_channel_config_t *config)
{
    assert(channelGroup < ADC_SC1_COUNT);

    uint32_t tmp32;

    /* ADCx_SC1n. */
    tmp32  = (base->SC1[channelGroup] & ~(ADC_SC1_ADCH_MASK | ADC_SC1_AIEN_MASK));
    tmp32 |= ADC_SC1_ADCH(config->channelNumber);
    if (true == config->enableInterruptOnConversionCompleted)
    {
        tmp32 |= ADC_SC1_AIEN_MASK;
    }
    base->SC1[channelGroup] = tmp32;
}

/******************************************************************************
 * EOF
 ******************************************************************************/

