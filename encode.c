#include "queue.h"
#include "encode.h"
#include "S32K144.h"
#include "driver_uart.h"

uint8_t check_message() {
	if (!queue_empty()) {
		uint8_t* new_message = queue_get_data();

		// Check start & stop byte
		if (new_message[START_BYTE] != START_BYTE_VALUE
				|| new_message[STOP_BYTE] != STOP_BYTE_VALUE) {
					return MESSAGE_ERROR;
				}

		// Check sum
		uint8_t check_sum = 0;
		for(int i = CHECK_SUM_START; i < CHECK_SUM_STOP; i++) {
			check_sum += new_message[i];
		}

		if (check_sum != new_message[CHECK_SUM_INDEX]) {
			return MESSAGE_ERROR;
		}
	} else {
		return MESSAGE_NO_AVALABLE;
	}
	return MESSAGE_CORRECT;
}

void push_message(uint8_t option, uint8_t value) {
	LPUART_DRV_WriteByte(LPUART1, START_BYTE_VALUE);
	LPUART_DRV_WriteByte(LPUART1, option);
	LPUART_DRV_WriteByte(LPUART1, value);
	LPUART_DRV_WriteByte(LPUART1, (uint8_t)(option + value));
	LPUART_DRV_WriteByte(LPUART1, STOP_BYTE_VALUE);
}
