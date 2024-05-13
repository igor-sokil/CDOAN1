#include <iostream>

#include "header.h"

#include "scMrzs.h"

static  uint8_t inputMessage[292];//DNP_MAX_DATA_LINK_FRAME_SIZE_WITH_CRC];			
static  uint16_t lenInputMessage;
uint8_t *getInputMessage(void);

uint16_t *getLenInputMessage(void);
uint16_t *getLenInputMessage(void)
{
  return &lenInputMessage;
}

uint8_t *getInputMessage(void)
{
  return inputMessage;
}

/*
 * Imiotialize CDOAN libarary. Load options from file, if specified
 */
void StartCDOANInit(void)//char *ConfigFileName)
{   		
///	noTimeAtAll.tv_sec = noTimeAtAll.tv_usec = 0;
	CommunicationMode = 0;
/*
#ifndef REMOVE_SERIAL_COMMUNICATION	
	if (SERIAL_COMMUNICATION)
		CommunicationMode |= COMM_MODE_SERIAL;
#endif 
#ifndef REMOVE_NETWORK_COMMUNICATION
	if (NETWORK_COMMUNICATION)
		CommunicationMode |= (UDP_ONLY) ? COMM_MODE_UDP : COMM_MODE_TCP;
#endif		
	if (!CommunicationMode)
	{
		printf("No communicaiton mode enabled\n");
		return;				// No communicaiton mode selected
	}
#ifndef REMOVE_SERIAL_COMMUNICATION	
	strcpy(SerialPortName, SERIAL_PORT_NAME);		// Default
#endif	
*/
	RecordedTime = 0;								// For LAN time sync
	TimeInvalidCountdown = NeedTimeCountdown = 0;	
	InitComm();
	InitDNP();
}

void TimeSliceMrzs()
{	
 std::cout<<"lenInputMessage= "<<lenInputMessage<<'\n';
 if(lenInputMessage > 0)
 {
 std::cout<<"inputMessage[0]= "<<std::hex<<(uint16_t)inputMessage[0]<<'\n';
 std::cout<<"inputMessage[1]= "<<std::hex<<(uint16_t)inputMessage[1]<<'\n';
 std::cout<<"inputMessage[2]= "<<std::hex<<(uint16_t)inputMessage[2]<<'\n';
 std::cout<<"inputMessage[3]= "<<std::hex<<(uint16_t)inputMessage[3]<<'\n';
 std::cout<<"inputMessage[4]= "<<std::hex<<(uint16_t)inputMessage[4]<<'\n';
 std::cout<<"inputMessage[5]= "<<std::hex<<(uint16_t)inputMessage[5]<<'\n';
 std::cout<<"inputMessage[6]= "<<std::hex<<(uint16_t)inputMessage[6]<<'\n';
 std::cout<<"inputMessage[7]= "<<std::hex<<(uint16_t)inputMessage[7]<<'\n';
 std::cout<<"inputMessage[8]= "<<std::hex<<(uint16_t)inputMessage[8]<<'\n';
 std::cout<<"inputMessage[9]= "<<std::hex<<(uint16_t)inputMessage[9]<<'\n';

 int len = CheckForSerialMessageMrzs(inputMessage);
 ProcessDNPMessageMrzs((char*)inputMessage, len);		

 }
////	int16_t sts, len;
////	char msg[DNP_MAX_DATA_LINK_FRAME_SIZE_WITH_CRC];			
// int16_t  len = CheckForSerialMessageMrzs(inputMessage);
}

