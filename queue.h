#ifndef HAL_QUEUE_QUEUE_H_
#define HAL_QUEUE_QUEUE_H_

#include "S32K144.h"
#include "encode.h"
/******************************************************************************
 * Definitions
 ******************************************************************************/

struct queue
{
	uint8_t(*buffer)[MESSAGE_LENGTH];    /* Pointer to queue buffer */
	uint8_t  size_1D;                    /* Size of 1st dimension of queue */
	uint8_t  size_2D;                    /* Size of 2nd dimension of queue */
	uint8_t  head_1D;                    /* Head pointer of 1st dimension */
	uint8_t  head_2D;                    /* Head pointer of 2nd dimension */
	uint8_t  tail;                       /* Tail pointer */
};

/******************************************************************************
 * Public fucntions
 ******************************************************************************/
/**
  \brief         take a charater from UART, put in in queue buffer, move head pt
  \param [in]    data : data of a character from UART
 */
void queue_put_data(const uint8_t data);

/**
  \brief     take out a array from queue buffer, move tail pointer
  \return    pointer to a array queue buffer
 */
uint8_t* queue_get_data();

/**
  \brief     check if queue is empty
  \return    1 if queue is empty, 0 if not
 */
uint8_t queue_empty();

/**
  \brief     check if queue is full
  \return    1 if queue is full, 0 if not
 */
uint8_t queue_full();

uint8_t check_message();

void push_message(uint8_t option, uint8_t value);

#endif /* HAL_QUEUE_QUEUE_H_ */
