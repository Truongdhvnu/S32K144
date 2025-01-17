#ifndef DRIVERS_LPIT_DRIVER_LPIT_H_
#define DRIVERS_LPIT_DRIVER_LPIT_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_common.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
/* @brief LPIT configuration structure */
typedef struct _lpit_config
{
    bool enableRunInDebug; /*!< true: Timers run in debug mode; false: Timers stop in debug mode */
    bool enableRunInDoze;  /*!< true: Timers run in doze mode; false: Timers stop in doze mode */
} lpit_config_t;

/* @brief List of LPIT channels */
typedef enum _lpit_chnl
{
    LPIT_Chnl_0 = 0U, /*!< LPIT channel number 0*/
    LPIT_Chnl_1,      /*!< LPIT channel number 1 */
    LPIT_Chnl_2,      /*!< LPIT channel number 2 */
    LPIT_Chnl_3,      /*!< LPIT channel number 3 */
} lpit_chnl_t;

/* @brief Mode options available for the LPIT timer. */
typedef enum _lpit_timer_modes
{
    LPIT_PeriodicCounter = 0U, /*!< Use the all 32-bits, counter loads and decrements to zero */
    LPIT_DualPeriodicCounter,  /*!< Counter loads, lower 16-bits  decrement to zero, then
                                     upper 16-bits  decrement */
    LPIT_TriggerAccumulator,   /*!< Counter loads on first trigger and decrements on each trigger */
    LPIT_InputCapture          /*!< Counter  loads with 0xFFFFFFFF, decrements to zero. It stores
                                    the inverse of the current value when a input trigger is detected */
} lpit_timer_modes_t;

/* @brief Trigger source options available */
typedef enum _lpit_trigger_source
{
    LPIT_TriggerSource_External = 0U, /*!< Use external trigger input */
    LPIT_TriggerSource_Internal       /*!< Use internal trigger */
} lpit_trigger_source_t;

/* @brief Structure to configure the channel timer. */
typedef struct _lpit_chnl_params
{
    bool chainChannel;                   /* true: Timer chained to previous timer;
                                            false: Timer not chained */
    lpit_timer_modes_t timerMode;        /* Timers mode of operation. */
    lpit_trigger_source_t triggerSource; /* Decides if we use external or internal trigger. */
    bool enableReloadOnTrigger;          /* true: Timer reloads when a trigger is detected;
                                            false: No effect */
    bool enableStopOnTimeout;            /* true: Timer will stop after timeout;
                                            false: does not stop after timeout */
    bool enableStartOnTrigger;           /* true: Timer starts when a trigger is detected;
                                            false: decrement immediately */
} lpit_chnl_params_t;

/******************************************************************************
 * API
 ******************************************************************************/
/**
 * @brief Configures the LPIT peripheral for a basic operation.
 *
 * @param base    LPIT peripheral base address.
 * @param config  Pointer to the user configuration structure.
 */
void LPIT_DRV_Init(LPIT_Type *base, const lpit_config_t *config);

/**
 * brief Disables the module and gates the LPIT clock.
 *
 * param base LPIT peripheral base address.
 */
static inline void LPIT_DRV_Deinit(LPIT_Type *base)
{
    /* Disable the module */
    base->MCR &= ~LPIT_MCR_M_CEN_MASK;
}

/**
 * @brief Sets up an LPIT channel based on the user's preference.
 *
 * @param base       LPIT peripheral base address.
 * @param channel    Channel that is being configured.
 * @param chnlSetup  Configuration parameters.
 */
void LPIT_DRV_SetupChannel(LPIT_Type *base, lpit_chnl_t channel,
                           const lpit_chnl_params_t *chnlSetup);

/**
 * @brief Enables the selected PIT interrupts.
 *
 * @param base  LPIT peripheral base address.
 * @param mask  The interrupts to enable. This is a logical OR of members of the
 *              enumeration ::lpit_interrupt_enable_t
 */
static inline void LPIT_DRV_EnableInterrupts(LPIT_Type *base, uint32_t mask)
{
    base->MIER |= mask;
}

/**
 * @brief Disables the selected PIT interrupts.
 *
 * @param base  LPIT peripheral base address.
 * @param mask  The interrupts to enable. This is a logical OR of members of the
 *              enumeration ::lpit_interrupt_enable_t
 */
static inline void LPIT_DRV_DisableInterrupts(LPIT_Type *base, uint32_t mask)
{
    base->MIER = ~mask;
}

/**
 * @brief Sets the timer period in units of count.
 *
 * @param base    LPIT peripheral base address.
 * @param channel Timer channel number.
 * @param ticks   Timer period in units of ticks.
 */
static inline void LPIT_DRV_SetTimerPeriod(LPIT_Type *base,
                                           lpit_chnl_t channel,
                                           uint32_t ticks)
{
    assert(ticks > 2U);
    base->TMR[channel].TVAL = ticks - 1U;
}

/**
 * @brief Reads the current timer counting value.
 *
 * @param base     LPIT peripheral base address.
 * @param channel  Timer channel number.
 *
 * @return Current timer counting value in ticks.
 */
static inline uint32_t LPIT_DRV_GetCurrentTimerCount(LPIT_Type *base,
                                                     lpit_chnl_t channel)
{
    return base->TMR[channel].CVAL;
}

/**
 * @brief Starts the timer counting.
 *
 * After calling this function, timers load the period value and count down to 0
 * When the timer reaches 0, it generates a trigger pulse and sets the timeout
 * interrupt flag.
 *
 * @param base    LPIT peripheral base address.
 * @param channel Timer channel number.
 */
static inline void LPIT_DRV_StartTimer(LPIT_Type *base, lpit_chnl_t channel)
{
    uint32_t shift = LPIT_SETTEN_SET_T_EN_0_MASK;

    base->SETTEN |= shift << ((uint32_t)channel);
}

/**
 * @brief Stops the timer counting.
 *
 * @param base    LPIT peripheral base address.
 * @param channel Timer channel number.
 */
static inline void LPIT_DRV_StopTimer(LPIT_Type *base, lpit_chnl_t channel)
{
    uint32_t shift = LPIT_CLRTEN_CLR_T_EN_0_MASK;

    base->CLRTEN |= shift << ((uint32_t)channel);
}

/**
 * @brief Performs a software reset on the LPIT module.
 *
 * This resets all channels and registers except the Module Control Register.
 *
 * @param base LPIT peripheral base address.
 */
static inline void LPIT_DRV_Reset(LPIT_Type *base)
{
    base->MCR |= LPIT_MCR_SW_RST_MASK;
    base->MCR &= ~LPIT_MCR_SW_RST_MASK;
}

#endif /* DRIVERS_LPIT_DRIVER_LPIT_H_ */

/******************************************************************************
 * EOF
 ******************************************************************************/
