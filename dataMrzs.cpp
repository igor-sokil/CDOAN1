#include "header.h"

#include "scMrzs.h"

//struct timeval noTimeAtAll;

//char CommunicationMode;				// Network or serial
//char SomeBinaryEventsWereSent;

char DNPMessage[NETWORK_MAX_MESSAGE_SIZE + DNP_MAX_DATA_LINK_FRAME_SIZE_WITH_CRC];
int16_t CompiledMessageSize;

