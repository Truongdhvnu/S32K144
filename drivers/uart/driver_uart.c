/******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_uart.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/


/******************************************************************************
 * Code
 ******************************************************************************/
void LPUART_DRV_Init(LPUART_Type *base, const lpuart_config_t *config, uint32_t srcClock_Hz)
{
    assert(NULL != config);
    assert(0U < config->baudRate_Bps);

    uint32_t temp;
    uint16_t sbr, sbrTemp;
    uint8_t  osr, osrTemp;
    uint32_t tempDiff, calculatedBaud, baudDiff;

    /* This LPUART instantiation uses a slightly different baud rate calculation
     * The idea is to use the best OSR (over-sampling rate) possible
     * Note, OSR is typically hard-set to 16 in other LPUART instantiations
     * loop to find the best OSR value possible, one that generates minimum baudDiff
     * iterate through the rest of the supported values of OSR */

    baudDiff = config->baudRate_Bps;
    osr      = 0U;
    sbr      = 0U;
    for (osrTemp = 4U; osrTemp <= 32U; osrTemp++)
    {
        /* calculate the temporary sbr value   */
        sbrTemp = (uint16_t)((srcClock_Hz * 10U / (config->baudRate_Bps * (uint32_t)osrTemp) + 5U) / 10U);
        /*set sbrTemp to 1 if the sourceClockInHz can not satisfy the desired baud rate*/
        if (sbrTemp == 0U)
        {
            sbrTemp = 1U;
        }
        else if (sbrTemp > LPUART_BAUD_SBR_MASK)
        {
            sbrTemp = LPUART_BAUD_SBR_MASK;
        }
        else
        {
            /* Avoid MISRA 15.7 */
        }
        /* Calculate the baud rate based on the temporary OSR and SBR values */
        calculatedBaud = (srcClock_Hz / ((uint32_t)osrTemp * (uint32_t)sbrTemp));
        tempDiff       = calculatedBaud > config->baudRate_Bps ?
                         (calculatedBaud - config->baudRate_Bps) : (config->baudRate_Bps - calculatedBaud);

        if (tempDiff <= baudDiff)
        {
            baudDiff = tempDiff;
            osr      = osrTemp; /* update and store the best OSR value calculated */
            sbr      = sbrTemp; /* update store the best SBR value calculated */
        }
    }

    temp = base->BAUD;

    /* Acceptable baud rate, check if OSR is between 4x and 7x oversampling.
     * If so, then "BOTHEDGE" sampling must be turned on */
    /*
     * $Branch Coverage Justification$
     * $ref fsl_lpuart_c_ref_1$
     */
    if ((osr > 3U) && (osr < 8U))
    {
        temp |= LPUART_BAUD_BOTHEDGE_MASK;
    }

    /* program the osr value (bit value is one less than actual value) */
    temp &= ~LPUART_BAUD_OSR_MASK;
    temp |= LPUART_BAUD_OSR((uint32_t)osr - 1UL);

    /* write the sbr value to the BAUD registers */
    temp &= ~LPUART_BAUD_SBR_MASK;
    base->BAUD = temp | LPUART_BAUD_SBR(sbr);

    /* Set bit count and parity mode. */
    base->BAUD &= ~LPUART_BAUD_M10_MASK;

    temp = base->CTRL & ~(LPUART_CTRL_PE_MASK | LPUART_CTRL_PT_MASK  |
                          LPUART_CTRL_M_MASK  | LPUART_CTRL_ILT_MASK |
                          LPUART_CTRL_IDLECFG_MASK);


    if (LPUART_ParityDisabled != config->parityMode)
    {
        temp |= LPUART_CTRL_M_MASK; /* Eight data bits and one parity bit */
    }

    base->CTRL = temp;

    /* Clear all status flags */
    temp = (LPUART_STAT_RXEDGIF_MASK | LPUART_STAT_IDLE_MASK | LPUART_STAT_OR_MASK | LPUART_STAT_NF_MASK |
            LPUART_STAT_FE_MASK | LPUART_STAT_PF_MASK);

    /* Set data bits order. */
    if (true == config->isMsb)
    {
        temp |= LPUART_STAT_MSBF_MASK;
    }
    else
    {
        temp &= ~LPUART_STAT_MSBF_MASK;
    }

    base->STAT |= temp;

    /* Enable TX/RX base on configure structure. */
    temp = base->CTRL;
    if (true == config->enableTx)
    {
        temp |= LPUART_CTRL_TE_MASK;
    }

    if (true == config->enableRx)
    {
        temp |= LPUART_CTRL_RE_MASK;
    }

    base->CTRL = temp;
}

void LPUART_DRV_Deinit(LPUART_Type *base)
{
    uint32_t temp;

    /* Wait last char shift out */
    while (0U == (base->STAT & LPUART_STAT_TC_MASK))
    {
    }

    /* Clear all status flags */
    temp = (LPUART_STAT_RXEDGIF_MASK | LPUART_STAT_IDLE_MASK |
            LPUART_STAT_OR_MASK      | LPUART_STAT_NF_MASK   |
            LPUART_STAT_FE_MASK      | LPUART_STAT_PF_MASK);

    base->STAT |= temp;

    /* Disable the module. */
    base->CTRL = 0U;
}

void LPUART_DRV_WriteBlocking(LPUART_Type *base, const uint8_t *data, size_t length)
{
    assert(NULL != data);

    const uint8_t *dataAddress = data;
    size_t transferSize        = length;

    while (0U != transferSize)
    {
        while (0U == (base->STAT & LPUART_STAT_TDRE_MASK))
        {
        }
        base->DATA = *(dataAddress);
        dataAddress++;
        transferSize--;
    }
    /* Ensure all the data in the transmit buffer are sent out to bus. */
    while (0U == (base->STAT & LPUART_STAT_TC_MASK))
    {
    }
}

/******************************************************************************
 * EOF
 ******************************************************************************/
