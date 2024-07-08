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
                            
static bool queue_full_status = false;

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
        /* Reset 2D index */
        if (QUEUE.head_2D == QUEUE.size_2D)
        {
            QUEUE.head_2D = 0;
        }
        if (QUEUE.head_2D == QUEUE.tail) 
        {
            queue_full_status = true;
        }
	}
}

uint8_t* queue_get_data()
{   
    if (is_queue_empty()) {
        return NULL;
    }
	/* Take 1 array from queue */
	uint8_t* data = QUEUE.buffer[QUEUE.tail];
    if (QUEUE.head_2D == QUEUE.tail) 
    {
        queue_full_status = false;
    }
	/* Increase tail index */
	QUEUE.tail++;
	/* Reset tail index */
	if (QUEUE.tail == QUEUE.size_2D)
	{
		QUEUE.tail = 0;
	}

	return data;
}

inline bool is_queue_empty()
{
    return (!queue_full_status) && (QUEUE.tail == QUEUE.head_2D);
}

inline bool is_queue_full()
{
    return queue_full_status;
}

/******************************************************************************
 * EOF
 ******************************************************************************/
