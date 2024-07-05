#ifndef DRIVERS_DRIVER_ADC_H_
#define DRIVERS_DRIVER_ADC_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_common.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
/* @brief Reference voltage source. */
typedef enum _adc_reference_voltage_source
{
    /* For external pins pair of VrefH and VrefL. */
    ADC_ReferenceVoltageSourceVref = 0U,
    /* For alternate reference pair of ValtH and ValtL. */
    ADC_ReferenceVoltageSourceValt = 1U,
} adc_reference_voltage_source_t;

/* @brief Conversion clock source. */
typedef enum _adc_clock_source
{
    ADC_ClockSourceAlt0 = 0U, /*!< Alternate clock 1 (ADC_ALTCLK1). */
    ADC_ClockSourceAlt1 = 1U, /*!< Alternate clock 2 (ADC_ALTCLK2). */
    ADC_ClockSourceAlt2 = 2U, /*!< Alternate clock 3 (ADC_ALTCLK3). */
    ADC_ClockSourceAlt3 = 3U, /*!< Alternate clock 4 (ADC_ALTCLK4). */
} adc_clock_source_t;

/* @brief Clock divider for the converter. */
typedef enum _adc_clock_divider
{
    ADC_ClockDivider1 = 0U, /* Divider 1 from the input clock to the module. */
    ADC_ClockDivider2 = 1U, /* Divider 2 from the input clock to the module. */
    ADC_ClockDivider4 = 2U, /* Divider 4 from the input clock to the module. */
    ADC_ClockDivider8 = 3U, /* Divider 8 from the input clock to the module. */
} adc_clock_divider_t;

/* @brief Converter's resolution. */
typedef enum _adc_resolution
{
    ADC_Resolution8Bit  = 0U, /* 8 bit resolution. */
    ADC_Resolution12Bit = 1U, /* 12 bit resolution. */
    ADC_Resolution10Bit = 2U, /* 10 bit resolution. */
} adc_resolution_t;

/* @brief Conversion trigger type. */
typedef enum _adc_trigger_type
{
    ADC_TriggerTypeSoftware = 0U, /* Software trigger selected. */
    ADC_TriggerTypeHardware = 1U, /* Hardware trigger selected. */
} adc_trigger_type_t;

/* @brief Converter configuration. */
typedef struct _adc_config
{
    adc_reference_voltage_source_t referenceVoltageSource;  /* Select the reference voltage source. */
    adc_clock_source_t clockSource;                         /* Select the input clock source to converter. */
    adc_clock_divider_t clockDivider;                       /* Select the divider of input clock source. */
    adc_resolution_t resolution;                            /* Select the sample resolution mode. */
    uint32_t sampleClockCount;                              /* Select the sample clock count. */
    bool enableContinuousConversion;                        /* Enable continuous conversion mode. */
    adc_trigger_type_t triggerType;                         /* Select the conversion trigger type */
    bool dmaEnable;                                         /* Enable DMA  */
} adc_config_t;

/* @brief Channel conversion configuration. */
typedef struct _adc_channel_config
{
    /* Setting the conversion channel number. The available range is 0-31.
       See channel connection information for each chip in Reference Manual
       document. */
    uint32_t channelNumber;
    /* Generate a interrupt request once the conversion is completed. */
    bool enableInterruptOnConversionCompleted;
} adc_channel_config_t;

/******************************************************************************
 * API
 ******************************************************************************/
/**
 * @brief Initialize the ADC module.
 *
 * @param base    ADC peripheral base address.
 * @param config  Pointer to "adc12_config_t" structure.
 */
void ADC_DRV_Init(ADC_Type *base, const adc_config_t *config);

/**
 * @brief Configure the conversion channel.
 *
 * @param base          ADC peripheral base address.
 * @param channelGroup  Channel group index.
 * @param config        Pointer to "adc_channel_config_t" structure.
 */
void ADC_DRV_SetChannelConfig(ADC_Type *base, uint32_t channelGroup,
                             const adc_channel_config_t *config);

/**
 * @brief Get the conversion value.
 *
 * @param base          ADC peripheral base address.
 * @param channelGroup  Channel group index.
 *
 * @return Conversion value.
 */
static inline uint32_t
ADC_DRV_GetChannelConversionValue(ADC_Type *base, uint32_t channelGroup)
{
    return base->R[channelGroup];
}

/**
 * @brief Enable or disable the hardware trigger mode.
 *
 * @param base    ADC peripheral base address.
 * @param enable  "true" means to enable, "false" means not.
 */
static inline void ADC_DRV_EnableHardwareTrigger(ADC_Type *base, bool enable)
{
    if (enable)
    {
        base->SC2 |= ADC_SC2_ADTRG_MASK;
    }
    else
    {
        base->SC2 &= ~ADC_SC2_ADTRG_MASK;
    }
}

#endif /* DRIVERS_DRIVER_ADC_H_ */

/******************************************************************************
 * EOF
 ******************************************************************************/
