#ifndef DRIVER_FTM_H
#define DRIVER_FTM_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "S32K144.h"
#include <driver_common.h>

/******************************************************************************
 * Definitions
 ******************************************************************************/
#define FTM_MODULO_MAX          65535
#define FTM_PWMEN_BASE_SHIFT    16u

/* @brief FTM clock source */
typedef enum {
    FTM_NONE_CLK,       /* Clock disable */
    FTM_SYS_CLK,        /* System clock*/
    FTM_RTC_CLK,        /* Realtime clock*/
    FTM_EXT_CLK         /* External clock*/
} ftm_clock_src;

/* @brief FTM Prescaler option */
typedef enum {
    FTM_PRESCALE_1,
    FTM_PRESCALE_2,
    FTM_PRESCALE_4,
    FTM_PRESCALE_8,
    FTM_PRESCALE_16,
    FTM_PRESCALE_32,
    FTM_PRESCALE_64,
    FTM_PRESCALE_128
} ftm_pre_scale_t;

/* @brief List of FTM channels */
typedef enum {
    FTM_Chnl_0,
    FTM_Chnl_1,
    FTM_Chnl_2,
    FTM_Chnl_3,
    FTM_Chnl_4,
    FTM_Chnl_5,
    FTM_Chnl_6,
    FTM_Chnl_7
} ftm_chnl_t;

/* @brief FTM avalable modes*/
typedef enum {
    DISABLE,
    INPUT_CAPTURE_RISING_EDGE,
    INPUT_CAPTURE_FAILLING_EDGE,
    OUPUT_COMPARE_TOGGLE,
    OUPUT_COMPARE_CLEAR,
    OUPUT_COMPARE_SET,
    EDGE_ALIGNED_HIGH_TRUE,
    EDGE_ALIGNED_LOW_TRUE,
    CENTER_ALIGNED_HIGH_TRUE,
    CENTER_ALIGNED_LOW_TRUE,
} ftm_chnl_mode;

/* @brief Configuration struct for FTM instances */
typedef struct {
    ftm_clock_src clk_src;          /* Clock source*/
    ftm_pre_scale_t pre_scale;      /* Prescale */
    uint32_t freq;                  /* Frequency */
    uint32_t resolution;                /* Module */
} ftm_config_t;

/* @brief Configuration struct for channels of FTM instances */
typedef struct {
    ftm_chnl_mode mode;             /* Mode of the channel*/
    uint32_t duty;                  /* Duty cycle of PWM whwen initailized*/
    bool intEnable;                 /* Interrupt Enable */
} ftm_chnl_prarams_t;

/******************************************************************************
 * API
 ******************************************************************************/

/// @brief Enable writting to FTM module
static inline void FTM_DRV_Write_Protect_Disable() {
    FTM0->MODE |= FTM_MODE_WPDIS_MASK;
}

/// @brief Configures the FTM with a configuration
/// @param base             FTM instance
/// @param config           Configuration structure for the FTM instance
/// @param srcClock_freq    Input clocksouce frequency of module
void FTM_DRV_Init(FTM_Type *base, ftm_config_t* config, uint32_t srcClock_freq);

/// @brief Configures the FTM channel with a configuration
/// @param base             FTM instance
/// @param channel          Configured channel of the FTM instance
/// @param config           Configuration structure for the FTM instance
/// @param chnlSetup        Configuration structure for the channel
void FTM_DRV_SetupChannel(FTM_Type *base, ftm_chnl_t channel, ftm_config_t* config,
                                        const ftm_chnl_prarams_t* chnlSetup);

/// @brief Get resolution of an FTM instance
/// @param base     FTM instance
/// @return         Frequency of the FTM channel       
static inline uint32_t FTM_DRV_GetResolution(FTM_Type *base) {
    return base->MOD;
}

/// @brief Enable FTM module to operate
/// @param base         FTM instance
/// @param config       Configuration structure for the FTM instance
static inline void FTM_DRV_StartCounters(FTM_Type *base, ftm_config_t* config) {
    base->SC |= FTM_SC_CLKS(config->clk_src);
}

/// @brief Set duty cycle for a FTM channel
/// @param base         FTM instance
/// @param chnl         Channel of the FTM instance
/// @param dutyCycle    Desired duty cycle
static inline void FTM_DRV_setDutyCycle(FTM_Type *base, uint32_t chnl, uint32_t dutyCycle) {
	uint32_t CV_value = base->MOD * dutyCycle / 100;
	base->CONTROLS[chnl].CnV = CV_value;
}

#endif /* DRIVER_FTM_H */

/******************************************************************************
 * EOF
 ******************************************************************************/