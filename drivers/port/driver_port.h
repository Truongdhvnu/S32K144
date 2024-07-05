#ifndef DRIVERS_DRIVER_PORT_H_
#define DRIVERS_DRIVER_PORT_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_common.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
/* @brief Configures the interrupt generation condition. */
typedef enum _port_interrupt
{
    PORT_InterruptOrDMADisabled = 0x0U, /*!< Interrupt/DMA request is disabled. */
    PORT_DMARisingEdge        = 0x1U, /* DMA request on rising edge. */
    PORT_DMAFallingEdge       = 0x2U, /* DMA request on falling edge. */
    PORT_DMAEitherEdge        = 0x3U, /* DMA request on either edge. */
    PORT_InterruptLogicZero   = 0x8U, /* Interrupt when logic zero. */
    PORT_InterruptRisingEdge  = 0x9U, /* Interrupt on rising edge. */
    PORT_InterruptFallingEdge = 0xAU, /* Interrupt on falling edge. */
    PORT_InterruptEitherEdge  = 0xBU, /* Interrupt on either edge. */
    PORT_InterruptLogicOne    = 0xCU, /* Interrupt when logic one. */
} port_interrupt_t;

/* @brief Pin mux selection */
typedef enum _port_mux
{
    PORT_MuxAlt0             = 0U,
    PORT_MuxAlt1             = 1U,
    PORT_MuxAlt2             = 2U,
    PORT_MuxAlt3             = 3U,
    PORT_MuxAlt4             = 4U,
    PORT_MuxAlt5             = 5U,
    PORT_MuxAlt6             = 6U,
    PORT_MuxAlt7             = 7U,
} port_mux_t;

/******************************************************************************
 * API
 ******************************************************************************/
/**
 * @brief Configures the pin muxing.
 *
 * @param base  PORT peripheral base pointer.
 * @param pin   PORT pin number.
 * @param mux   pin muxing slot selection.
 */
static inline void PORT_DRV_SetPinMux(PORT_Type *base, uint32_t pin, port_mux_t mux)
{
    base->PCR[pin] = (base->PCR[pin] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(mux);
}

/**
 * @brief Configures the port pin interrupt/DMA request.
 *
 * @param base    PORT peripheral base pointer.
 * @param pin     PORT pin number.
 * @param config  PORT pin interrupt configuration.
 *        - #PORT_InterruptOrDMADisabled: Interrupt/DMA request disabled.
 *        - #PORT_DMARisingEdge : DMA request on rising edge(if the DMA requests exit).
 *        - #PORT_DMAFallingEdge: DMA request on falling edge(if the DMA requests exit).
 *        - #PORT_DMAEitherEdge : DMA request on either edge(if the DMA requests exit).
 *        - #PORT_FlagRisingEdge : Flag sets on rising edge(if the Flag states exit).
 *        - #PORT_FlagFallingEdge : Flag sets on falling edge(if the Flag states exit).
 *        - #PORT_FlagEitherEdge : Flag sets on either edge(if the Flag states exit).
 *        - #PORT_InterruptLogicZero  : Interrupt when logic zero.
 *        - #PORT_InterruptRisingEdge : Interrupt on rising edge.
 *        - #PORT_InterruptFallingEdge: Interrupt on falling edge.
 *        - #PORT_InterruptEitherEdge : Interrupt on either edge.
 *        - #PORT_InterruptLogicOne   : Interrupt when logic one.
 */
static inline void PORT_DRV_SetPinInterruptConfig(PORT_Type *base,
                                                  uint32_t pin,
                                                  port_interrupt_t config)
{
    base->PCR[pin] = (base->PCR[pin] & ~PORT_PCR_IRQC_MASK) |
                                                          PORT_PCR_IRQC(config);
}

/**
 * @brief Reads the whole port status flag.
 *
 *
 * @param base  PORT peripheral base pointer.
 * @return Current port interrupt status flags, for example, 0x00010001 means the
 *         pin 0 and 16 have the interrupt.
 */
static inline uint32_t PORT_DRV_GetPinsInterruptFlags(PORT_Type *base)
{
    return base->ISFR;
}

/**
 * @brief Clears the multiple pin interrupt status flag.
 *
 * @param base  PORT peripheral base pointer.
 * @param mask  PORT pin number macro.
 */
static inline void PORT_DRV_ClearPinsInterruptFlags(PORT_Type *base,
                                                    uint32_t mask)
{
    base->ISFR = mask;
}

#endif /* DRIVERS_DRIVER_PORT_H_ */

/******************************************************************************
 * EOF
 ******************************************************************************/
