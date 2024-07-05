#ifndef DRIVERS_SIM_DRIVER_SIM_H_
#define DRIVERS_SIM_DRIVER_SIM_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_common.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
/* @brief ADC trigger source */
typedef enum _sim_adc_trigger_source
{
    SIM_ADC_TriggerSource_PDB    = 0U,
    SIM_ADC_TriggerSource_TRGMUX = 1U,
} sim_adc_trigger_source_t;

/* @brief ADC trigger source */
typedef enum _sim_adc_pretrigger_source
{
    SIM_ADC_PreTriggerSource_PDB      = 0U,
    SIM_ADC_PreTriggerSource_TRGMUX   = 1U,
    SIM_ADC_PreTriggerSource_Software = 2U,
} sim_adc_pretrigger_source_t;

/* @brief SIM ADC0 Options  */
typedef struct _sim_adc0_opt
{
    sim_adc_trigger_source_t ADC_Trigger_source;
    sim_adc_pretrigger_source_t ADC_PreTrigger_source;
} sim_adc0_opt_t;

/******************************************************************************
 * API
 ******************************************************************************/
static inline void SIM_DRV_ADC0option(sim_adc0_opt_t *option)
{
    SIM->ADCOPT |= SIM_ADCOPT_ADC0TRGSEL(option->ADC_Trigger_source) |
                   SIM_ADCOPT_ADC0PRETRGSEL(option->ADC_PreTrigger_source);
}

#endif /* DRIVERS_SIM_DRIVER_SIM_H_ */

/******************************************************************************
 * EOF
 ******************************************************************************/
