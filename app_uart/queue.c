/******************************************************************************
 * Includes
 ******************************************************************************/
#include "queue.h"
#include "S32K144.h"
#include "driver_uart.h"
/******************************************************************************
 * Global variables
 ******************************************************************************/
static uint8_t QUEUE_BUFFER[BUFFER_SIZE_2D][MESSAGE_LENGTH];

static struct queue QUEUE = { .buffer = QUEUE_BUFFER,      \
							  .size_1D = MESSAGE_LENGTH,   \
					          .size_2D = BUFFER_SIZE_2D,   \
							  .head_1D = 0,				   \
							  .head_2D = 0,				   \
							  .tail    = 0,				   \
					        };

static uint8_t QUEUE_AVAILABLE = BUFFER_SIZE_2D;

/******************************************************************************
 * Public functions
 ******************************************************************************/
void queue_put_data(const uint8_t data)
{
	/* Put character to array index */
	QUEUE.buffer[QUEUE.head_2D][QUEUE.head_1D] = data;
	/* Move to next index */
	QUEUE.head_1D++;
	/* Increase 2D index if 1D array end */
	if (QUEUE.head_1D == MESSAGE_LENGTH)
	{
		QUEUE.head_1D = 0;
		QUEUE.head_2D++;
		QUEUE_AVAILABLE--;
	}
	/* Reset 2D index */
	if (QUEUE.head_2D == QUEUE.size_2D)
	{
		QUEUE.head_2D = 0;
	}
}

uint8_t* queue_get_data()
{
	/* Take 1 array from queue */
	uint8_t* data = QUEUE.buffer[QUEUE.tail];
	/* Increase tail index */
	QUEUE.tail++;
	/* Increase queue available */
	QUEUE_AVAILABLE++;
	/* Reset tail index */
	if (QUEUE.tail == QUEUE.size_2D)
	{
		QUEUE.tail = 0;
	}

	return data;
}

uint8_t queue_empty()
{
	return (QUEUE_AVAILABLE == BUFFER_SIZE_2D);
}

uint8_t queue_full()
{
	return (QUEUE_AVAILABLE == 0);
}

/******************************************************************************
 * EOF
 ******************************************************************************/
