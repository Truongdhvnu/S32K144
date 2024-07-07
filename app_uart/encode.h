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

#define MEASSAGE_OPTION_BYTE    1
#define MEASSAGE_VALUE_BYTE     2

#define MESSAGE_CORRECT 	    0
#define MESSAGE_ERROR 		    1
#define MESSAGE_NO_AVALABLE	    2

#define OPTION_VOLTAGE          'v'
#define OPTION_UP               'r'
#define OPTION_CONFIRM          'z'
#define OPTION_FORWARD          '>'
#define OPTION_GO_BACK          '<'
#define OPTION_PLAYING          'p'
#define OPTION_PAUSE            't'

#define MESSAGE_DEFAULT_VALUE           '0'

// #define 
uint8_t check_message();

void push_message(uint8_t option, uint8_t value);

int checkReceiveCommandValid(uint8_t* cmd);

#endif
