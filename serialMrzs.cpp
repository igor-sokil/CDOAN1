#include <iostream>

#include "header.h"
#include "scMrzs.h"

uint16_t *getLenInputMessage(void);
uint16_t DNPCrc1(uint8_t* input1, uint16_t length);

int CheckForSerialMessageMrzs(uint8_t *inputMessage)
{   	
 std::cout<<'\n';
 std::cout<<"{CheckForSerialMessageMrzs1"<<'\n';

  uint16_t lenInputMessage = *getLenInputMessage();

  for(uint16_t i=0; i<lenInputMessage; i++)
  {
   DNPMessage[i] = inputMessage[i];
  }

				if ((DNPMessage[0] == 0x05) && (DNPMessage[1] == 0x64))
				{
//					if ((CompiledMessageSize - i) >= (DNP_MIN_MESSAGE_SIZE_WITH_CRC-1))  // Enough bytes
					//{   
                                            char crc[2];
					    DNPCrc(DNPMessage, 0, DNP_DATA_LINK_LENGTH_WTHOUT_CRC, crc);						
//					    DNPCrc1(DNPMessage, 0, DNP_DATA_LINK_LENGTH_WTHOUT_CRC, crc);						
//						if ((crc[0] == DNPMessage[i+7]) && (crc[1] == DNPMessage[i+8]))	// Lock baud rate

                                            std::cout<<"*crc[0]= "<<(uint16_t)crc[0]<<'\n';
                                            std::cout<<"*crc[1]= "<<(uint16_t)crc[1]<<'\n';
                                            std::cout<<"*DNPMessage[8]= "<<(uint16_t)DNPMessage[8]<<'\n';
                                            std::cout<<"*DNPMessage[9]= "<<(uint16_t)DNPMessage[9]<<'\n';

//					}//if
                                 }//if
 std::cout<<"}CheckForSerialMessageMrzs_"<<'\n';
  return lenInputMessage;
}

void SendSerialMessage(char *msg, int length)
{
 std::cout<<'\n';
 std::cout<<"*SendSerialMessage!!"<<'\n';
//	write(SerialPort.fd, msg, length); 	
 for(int i=0; i<length; i++)
                            std::cout<<"*msg[i]= "<<std::hex<<(uint16_t)msg[i]<<'\n';

}
