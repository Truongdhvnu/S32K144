#ifndef DRIVER_FTM_H
#define DRIVER_FTM_H

#include "S32K144.h"
#include <driver_common.h>

#define FTM_MODULO_MAX          65535
#define FTM_PWMEN_BASE_SHIFT    16u

typedef enum {
    NONE_CLK,
    SYS_CLK,
    RTC_CLK,
    EXT_CLK
} ftm_clock_src;

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

typedef struct {
    ftm_clock_src clk_src;
    ftm_pre_scale_t pre_scale;
    uint32_t freq;
    uint32_t modulo;
} ftm_config_t;

typedef struct {
    ftm_chnl_mode mode;
    uint32_t duty;
    bool intEnable;
} ftm_chnl_prarams_t;

static inline void FTM_DRV_Write_Protect_Disable() {
    FTM0->MODE |= FTM_MODE_WPDIS_MASK;
}

void FTM_DRV_Init(FTM_Type *base, ftm_config_t* config, uint32_t srcClock_freq);

void FTM_DRV_SetupChannel(FTM_Type *base, ftm_chnl_t channel, ftm_config_t* config,
                                        const ftm_chnl_prarams_t* chnlSetup);

static inline uint32_t FTM_DRV_GetResolution(FTM_Type *base) {
    return base->MOD;
}

static inline void FTM_DRV_StartCounters(FTM_Type *base, ftm_config_t* config) {
    base->SC |= FTM_SC_CLKS(config->clk_src);
}

static inline void FTM_DRV_setDutyCycle(FTM_Type *base, uint32_t chnl, uint32_t dutyCycle) {
	uint32_t CV_value = base->MOD * dutyCycle / 100;
	base->CONTROLS[chnl].CnV = CV_value;
}

#endif
