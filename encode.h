#ifndef ENCODE_H
#define ENCODE_H
#include "S32K144.h"

#define BUFFER_SIZE_2D          4
#define MESSAGE_LENGTH          5

#define START_BYTE 			    0
#define START_BYTE_VALUE 	    '0'

#define STOP_BYTE 			    4
#define STOP_BYTE_VALUE 	    '0'

#define CHECK_SUM_START 	    1
#define CHECK_SUM_STOP 		    2
#define CHECK_SUM_INDEX 	    3

#define MESSAGE_CORRECT 	    0
#define MESSAGE_ERROR 		    1
#define MESSAGE_NO_AVALABLE	    2

// #define 
uint8_t check_message();

void push_message(uint8_t option, uint8_t value);

#endif