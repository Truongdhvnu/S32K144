#ifndef DRIVERS_DRIVER_UART_H_
#define DRIVERS_DRIVER_UART_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_common.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
/* @brief LPUART parity mode. */
typedef enum _lpuart_parity_mode
{
    LPUART_ParityDisabled = 0x0U, /* Parity disabled */
    LPUART_ParityEven     = 0x2U, /* Parity enabled, type even */
    LPUART_ParityOdd      = 0x3U, /* Parity enabled, type odd */
} lpuart_parity_mode_t;

/* @brief LPUART data bits count. */
typedef enum _lpuart_data_bits
{
    LPUART_EightDataBits = 0x0U, /*!< Eight data bit */
    LPUART_SevenDataBits = 0x1U, /*!< Seven data bit */
} lpuart_data_bits_t;

/* @brief LPUART stop bit count. */
typedef enum _lpuart_stop_bit_count
{
    LPUART_OneStopBit = 0U, /* One stop bit */
    LPUART_TwoStopBit = 1U, /* Two stop bits */
} lpuart_stop_bit_count_t;

/* @brief LPUART idle flag type defines when the receiver starts counting. */
typedef enum _lpuart_idle_type_select
{
    LPUART_IdleTypeStartBit = 0U, /* Start counting after a valid start bit. */
    LPUART_IdleTypeStopBit  = 1U, /* Start counting after a stop bit. */
} lpuart_idle_type_select_t;

/* @brief LPUART configuration structure. */
typedef struct _lpuart_config
{
    uint32_t baudRate_Bps;                 /* LPUART baud rate  */
    lpuart_parity_mode_t parityMode;       /* Parity mode, disabled (default), even, odd */
    lpuart_data_bits_t dataBitsCount;      /* Data bits count, eight (default), seven */
    bool isMsb;                            /* Data bits order, LSB (default), MSB */
    lpuart_stop_bit_count_t stopBitCount;  /* Number of stop bits, 1 stop bit (default) or 2 stop bits  */
    bool enableTx;                         /* Enable TX */
    bool enableRx;                         /* Enable RX */
} lpuart_config_t;

/******************************************************************************
 * API
 ******************************************************************************/
/**
 * @brief Initializes an LPUART instance with the user configuration structure and the peripheral clock.
 *
 * This function configures the LPUART module with user-defined settings. Call the LPUART_GetDefaultConfig() function
 * to configure the configuration structure and get the default configuration.
 * The example below shows how to use this API to configure the LPUART.
 * @code
 *  lpuart_config_t lpuartConfig;
 *  lpuartConfig.baudRate_Bps = 115200U;
 *  lpuartConfig.parityMode = LPUART_ParityDisabled;
 *  lpuartConfig.dataBitsCount = LPUART_EightDataBits;
 *  lpuartConfig.isMsb = false;
 *  lpuartConfig.stopBitCount = LPUART_OneStopBit;
 *  LPUART_Init(LPUART1, &lpuartConfig, 20000000U);
 * @endcode
 *
 * @param base         LPUART peripheral base address.
 * @param config       Pointer to a user-defined configuration structure.
 * @param srcClock_Hz  LPUART clock source frequency in HZ.
 */
void LPUART_DRV_Init(LPUART_Type *base, const lpuart_config_t *config, uint32_t srcClock_Hz);

/**
 * @brief Deinitializes a LPUART instance.
 *
 * This function waits for transmit to complete, disables TX and RX, and disables the LPUART clock.
 *
 * @param base  LPUART peripheral base address.
 */
void LPUART_DRV_Deinit(LPUART_Type *base);

/**
 * @brief Writes to the transmitter register.
 *
 * This function writes data to the transmitter register directly. The upper layer must
 * ensure that the TX register is empty or that the TX FIFO has room before calling this function.
 *
 * @param base  LPUART peripheral base address.
 * @param data  Data write to the TX register.
 */
static inline void LPUART_DRV_WriteByte(LPUART_Type *base, uint8_t data)
{
    /* Wait for any ongoing transmission to finish */
    while (!(base->STAT & LPUART_STAT_TDRE_MASK));
    base->DATA = data;
}

/**
 * @brief Reads the receiver register.
 *
 * This function reads data from the receiver register directly. The upper layer must
 * ensure that the receiver register is full or that the RX FIFO has data before calling this function.
 *
 * @param base  LPUART peripheral base address.
 * @return Data read from data register.
 */
static inline uint8_t LPUART_DRV_ReadByte(LPUART_Type *base)
{
    return (uint8_t)(base->DATA);
}

/**
 * @brief Writes to the transmitter register using a blocking method.
 *
 * This function polls the transmitter register, first waits for the register to be empty or TX FIFO to have room,
 * and writes data to the transmitter buffer, then waits for the data to be sent out to the bus.
 *
 * @param base    LPUART peripheral base address.
 * @param data    Start address of the data to write.
 * @param length  Size of the data to write.
 */
void LPUART_DRV_WriteBlocking(LPUART_Type *base, const uint8_t *data, size_t length);

#endif /* DRIVERS_DRIVER_UART_H_ */

/******************************************************************************
 * EOF
 ******************************************************************************/
