#ifndef DRIVERS_TRGMUX_DRIVER_TRGMUX_H_
#define DRIVERS_TRGMUX_DRIVER_TRGMUX_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_common.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/* @brief Defines the MUX select for peripheral trigger input. */
typedef enum _trgmux_trigger_input
{
    TRGMUX_TriggerInput0 = 0, /*!< The MUX select for peripheral trigger input 0*/
    TRGMUX_TriggerInput1 = 1, /*!< The MUX select for peripheral trigger input 1*/
    TRGMUX_TriggerInput2 = 2, /*!< The MUX select for peripheral trigger input 2*/
    TRGMUX_TriggerInput3 = 3, /*!< The MUX select for peripheral trigger input 3*/
} trgmux_trigger_input_t;

/* @brief Defines the MUX select for peripheral trigger input. */
typedef enum _trgmux_trigger_source
{
    TRGMUX_Source_LPIT_CH0 = 0x11U,
    TRGMUX_Source_LPIT_CH1 = 0x12U,
    TRGMUX_Source_LPIT_CH2 = 0x13U,
    TRGMUX_Source_LPIT_CH3 = 0x14U,
} trgmux_trigger_source_t;
/******************************************************************************
 * API
 ******************************************************************************/
/**
 * brief Configures the trigger source of the appointed peripheral.
 *
 * The function configures the trigger source of the appointed peripheral.
 * Example:
   code
   TRGMUX_SetTriggerSource(TRGMUX0, kTRGMUX_Trgmux0Dmamux0, kTRGMUX_TriggerInput0, kTRGMUX_SourcePortPin);
   endcode
 * param base        TRGMUX peripheral base address.
 * param index       The index of the TRGMUX register, see TRGMUX index offsets
 *                    defined in S32K144.h
 * param input       The MUX select for peripheral trigger input
 * param trigger_src The trigger inputs for various peripherals. See the enum trgmux_source_t
 *                    defined in \<SOC\>.h.
 */
static inline void TRGMUX_DRV_SetTriggerSource(TRGMUX_Type *base,
                                           uint32_t index,
                                           trgmux_trigger_input_t input,
                                           trgmux_trigger_source_t trigger_src)
{
    switch (input)
    {
        case TRGMUX_TriggerInput0:
            base->TRGMUXn[index] |= TRGMUX_TRGMUXn_SEL0(trigger_src);
            break;
        case TRGMUX_TriggerInput1:
            base->TRGMUXn[index] |= TRGMUX_TRGMUXn_SEL1(trigger_src);
            break;
        case TRGMUX_TriggerInput2:
            base->TRGMUXn[index] |= TRGMUX_TRGMUXn_SEL2(trigger_src);
            break;
        case TRGMUX_TriggerInput3:
            base->TRGMUXn[index] |= TRGMUX_TRGMUXn_SEL3(trigger_src);
            break;
        default:
            /* Nothing */
            break;
    }
}

#endif /* DRIVERS_TRGMUX_DRIVER_TRGMUX_H_ */

/******************************************************************************
 * EOF
 ******************************************************************************/
