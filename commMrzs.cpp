#include <iostream>
#include "header.h"
#include "scMrzs.h"

//struct timeval noTimeAtAll;

char CommunicationMode;				// Network or serial
char SomeBinaryEventsWereSent;

char DNPMessage[NETWORK_MAX_MESSAGE_SIZE + DNP_MAX_DATA_LINK_FRAME_SIZE_WITH_CRC];
int16_t CompiledMessageSize;

static char XmitBuf[TRANSMIT_BUFFER_COUNT][DNP_MAX_DATA_LINK_FRAME_SIZE_WITH_CRC];

static int16_t XmitBufSize[TRANSMIT_BUFFER_COUNT];

static char SolicitedApplicationMessage[DNP_MAX_APPLICATION_FRAGMENT_WITHOUT_CRC];
static char UnsolicitedApplicationMessage[DNP_MAX_APPLICATION_FRAGMENT_WITHOUT_CRC];

static int16_t ApplicationIndex = 0;
static int16_t LengthOfLastSentSolicitedApplicationMessageWithoutCRC = 0;
static int16_t LengthOfLastSentUnsolicitedApplicationMessageWithoutCRC = 0;
static int16_t XmitHead, XmitTail, XmitFull;
static char CompilingUnsolicitedMessage, *AppMsgPtr;

/*
 * Called from initCDOAN
 */
void InitComm()
{
	XmitHead = XmitTail = 0;
	XmitFull = FALSE;
	ApplicationIndex = 0;
	LengthOfLastSentSolicitedApplicationMessageWithoutCRC = 0;
	LengthOfLastSentUnsolicitedApplicationMessageWithoutCRC = 0;
}

static void InsertIntoFrame(char *msg, int16_t start, int16_t length)
{
 std::cout<<'\n';
 std::cout<<"{InsertIntoFrame1"<<'\n';
 std::cout<<"*start= "<<start<<'\n';
 std::cout<<"*length= "<<length<<'\n';
   int16_t i, index;
	
	index = XmitBufSize[XmitHead];
	
	for (i=0; i<length; i++)
		XmitBuf[XmitHead][index++] = msg[start++];
	XmitBufSize[XmitHead] = index;
 std::cout<<"}InsertIntoFrame_"<<'\n';
}
/*
 * Add application octets to a message to be transmitted
 */
void AddApplicationData(char *buf, int16_t length)
{
	while(length--)
		AppMsgPtr[ApplicationIndex++] = *(buf++);
}
/*
 * Replace an application value. Called, for example, to change internal indication bits
 */
void ReplaceApplicationInt(int16_t val, int16_t Location, int16_t length)
{
	int16_t temp;
		
	while (length--)
	{
		temp = val / 256;				// This logic is required to support differnt endian usage
		AppMsgPtr[Location++] = (char) (val - (temp * 256));
		val = temp;
	}
}
/*
 * Add an integer to a message to be transmitted
 */
void AddApplicationInt(int32_t val, int16_t length)
{   uint16_t temp;
	char c;
	
	union
	{
		int32_t iVal;
		uint32_t uVal;
	} ValToSend;
	
	ValToSend.iVal = val;
	while (length--)
	{
		temp = ValToSend.uVal / 256;				// This logic is required to support differnt endian usage
		c = (char) (ValToSend.uVal - (temp * 256));
		AppMsgPtr[ApplicationIndex++] = c;
		ValToSend.uVal = temp;
	}
}
/*
 * Return number of octets left in application message to transmit
 */
int16_t SpaceLeft(int16_t WeNeedThis)
{
	return ((ApplicationIndex + WeNeedThis) < DNP_MAX_APPLICATION_FRAGMENT_WITHOUT_CRC);
}
/*
 * Retrun to the caller the index of where we are in compiling an application message
 * 
 * This offset can be used in a later call to ReplaceApplicationInt
 */
int16_t GetApplicationOffset()
{
	return ApplicationIndex;
}

/*
 * One transmit frame has been compiled. Advance to teh next one
 */
static void IncrementTransmitHead()
{
 std::cout<<'\n';
 std::cout<<"{IncrementTransmitHead1"<<'\n';
	if (XmitHead >= (TRANSMIT_BUFFER_COUNT - 1))
		XmitHead = 0;
	else
		XmitHead++;	
	XmitFull = XmitHead == XmitTail;
 std::cout<<"*XmitHead= "<<(uint16_t)XmitHead<<'\n';
 std::cout<<"}IncrementTransmitHead_"<<'\n';
}

/*
 * Send all frames of an application message
 */
static void SendFrames()
{		
 std::cout<<'\n';
 std::cout<<"{SendFrames1"<<'\n';
 std::cout<<"*XmitFull="<<(uint16_t)XmitFull<<'\n';
 std::cout<<"*XmitHead="<<(uint16_t)XmitHead<<'\n';
 std::cout<<"*XmitTail="<<(uint16_t)XmitTail<<'\n';

	while (XmitFull || (XmitHead != XmitTail))	// While more frames to send
	{
		XmitFull = FALSE;
//#ifndef REMOVE_SERIAL_COMMUNICATION			
//		if (CommunicationMode & COMM_MODE_SERIAL)
			SendSerialMessage(XmitBuf[XmitTail], XmitBufSize[XmitTail]);
//#endif
//#ifndef REMOVE_NETWORK_COMMUNICATION	
//		if (CommunicationMode & (COMM_MODE_TCP | COMM_MODE_UDP))
//			NetworkSend(XmitBuf[XmitTail], XmitBufSize[XmitTail]);		
//#endif
		if (XmitTail == (TRANSMIT_BUFFER_COUNT - 1))	// Wrap queue tail
			XmitTail = 0;
		else
			XmitTail++;	
	}	
 std::cout<<"}SendFrames1_"<<'\n';
}

void CreateDataLink(char ControlByte, uint16_t Destination, uint16_t Source, char Unsolicited)
{   
 std::cout<<'\n';
 std::cout<<"{CreateDataLink1"<<'\n';
        char *cp;
	
	ApplicationIndex = 0;	
	if (Unsolicited)
	{
		AppMsgPtr = UnsolicitedApplicationMessage;
		CompilingUnsolicitedMessage = TRUE;
	}
	else
	{
		AppMsgPtr = SolicitedApplicationMessage;
		CompilingUnsolicitedMessage = FALSE;
	}
	CompilingUnsolicitedMessage = Unsolicited;
	AppMsgPtr[ApplicationIndex++] = 0x05; 
	AppMsgPtr[ApplicationIndex++] = 0x64;
	ApplicationIndex++;						// Length to be filled in later
	AppMsgPtr[ApplicationIndex++] = ControlByte;
	IntToChars(AppMsgPtr, ApplicationIndex, 2, Destination);
	ApplicationIndex += 2;
	IntToChars(AppMsgPtr, ApplicationIndex, 2, Source);
	ApplicationIndex += 2;	
 std::cout<<"}CreateDataLink_"<<'\n';
}

/*
 * Send an application message
 */
void SendCompiledMessage()
{	
 std::cout<<'\n';
 std::cout<<"{SendCompiledMessage1"<<'\n';
	int16_t i, UserDataBytesThisFrame, OctetsThisBlock, FirstCrcIndex;
	char TransportFin = 0, TransportFir = TH_FIR, TransportSeq = 0;
	char crc[DNP_BYTES_PER_CRC], DataLink[DNP_DATA_LINK_LENGTH_WTHOUT_CRC], th;
	int16_t sts, temp;
	
	if (XmitFull || (ApplicationIndex < DNP_DATA_LINK_LENGTH_WTHOUT_CRC))
		return;					// Nothing to send
	
	if (CompilingUnsolicitedMessage)			// Save message in case we have to send the same one later
	{
		LengthOfLastSentUnsolicitedApplicationMessageWithoutCRC = ApplicationIndex;
		AppMsgPtr = UnsolicitedApplicationMessage;
	}
	else
	{
		LengthOfLastSentSolicitedApplicationMessageWithoutCRC = ApplicationIndex;	// In case we have to send it again
		AppMsgPtr = SolicitedApplicationMessage;
	}
	XmitBufSize[XmitHead] = 0;		// Initialize next circular queue entry
/*
 * Build Data Link Frame
 */	
 std::cout<<"*SendCompiledMessage2"<<'\n';

	for (i=0; i<DNP_DATA_LINK_LENGTH_WTHOUT_CRC; i++)
		DataLink[i] = AppMsgPtr[i];

 std::cout<<"*ApplicationIndex= "<<ApplicationIndex<<'\n';

	if (ApplicationIndex == DNP_DATA_LINK_LENGTH_WTHOUT_CRC)	// Just data link
	{
 std::cout<<"*SendCompiledMessage3"<<'\n';
		DataLink[DL_LENGTH_OFFSET] = DNP_MINIMUM_DATA_LINK_LENGTH_VALUE;
		InsertIntoFrame(DataLink, 0, DNP_DATA_LINK_LENGTH_WTHOUT_CRC);
		DNPCrc(DataLink, 0, DNP_DATA_LINK_LENGTH_WTHOUT_CRC, crc);	
		InsertIntoFrame(crc, 0, DNP_BYTES_PER_CRC);
		IncrementTransmitHead();
	}
	else 
	{
 std::cout<<"*SendCompiledMessage4"<<'\n';
		if (InternalIndications & IIN_BROADCAST)		// If we are acknoeldging a broadcast message
		{
			if (LastBroadcastAddress == DNP_BROADCAST_ADDRESS_DONT_CONFIRM) // And we do not need a confirm
				InternalIndications &= ~IIN_BROADCAST;
			else  // We need a confirmation
			{
				AppMsgPtr[MSG_OFFSET_APPLICATION_CONTROL_OCTET-1] |= AC_CON;	// -1 since transport header is missing
				ExpectedConfirmSequenceNumber = AppMsgPtr[MSG_OFFSET_APPLICATION_CONTROL_OCTET-1] & (AC_SEQ | AC_UNS);
				ActionOnAck |= DNP_WAITING_BROADCAST_IIN_ACK;
			}
		}
		if (AppMsgPtr[MSG_OFFSET_APPLICATION_CONTROL_OCTET-1] & AC_CON)
			 ApplicationConfirmTimer = ApplicationConfirmTimeout + 1;
		while (i < ApplicationIndex)					// Not just data link frame
		{
			UserDataBytesThisFrame = ApplicationIndex - i;
			if (UserDataBytesThisFrame > (DNP_MAX_DATA_LINK_FRAME_SIZE_WITHOUT_CRC - DNP_TRANSPORT_SIZE - DNP_MINIMUM_DATA_LINK_LENGTH_VALUE))
			{
				TransportFin = 0;						// This is not the last frame
				UserDataBytesThisFrame = DNP_MAX_DATA_LINK_FRAME_SIZE_WITHOUT_CRC - DNP_TRANSPORT_SIZE - DNP_MINIMUM_DATA_LINK_LENGTH_VALUE;
			}
			else
				TransportFin = TH_FIN;	
			DataLink[DL_LENGTH_OFFSET] = UserDataBytesThisFrame + DNP_TRANSPORT_SIZE + DNP_MINIMUM_DATA_LINK_LENGTH_VALUE; // Bytes this frame
			XmitBufSize[XmitHead] = 0;		
			InsertIntoFrame(DataLink, 0, DNP_DATA_LINK_LENGTH_WTHOUT_CRC);
			DNPCrc(DataLink, 0, DNP_DATA_LINK_LENGTH_WTHOUT_CRC, crc);		// Insert data link header CRC
			InsertIntoFrame(crc, 0, DNP_BYTES_PER_CRC);
			th = TransportFin | TransportFir | TransportSeq;		
			FirstCrcIndex = XmitBufSize[XmitHead];
			InsertIntoFrame(&th, 0, 1);
			TransportFir = 0;
			TransportSeq++;					// Can never wrap 
			OctetsThisBlock = 15;			// Plus one for transport header

			while (UserDataBytesThisFrame > 0)					// Next crc block
			{
				if (OctetsThisBlock > UserDataBytesThisFrame)
					OctetsThisBlock = UserDataBytesThisFrame;
				InsertIntoFrame(AppMsgPtr, i, OctetsThisBlock);	// Insert data bytes
				DNPCrc(XmitBuf[XmitHead], FirstCrcIndex, XmitBufSize[XmitHead] - FirstCrcIndex, crc);				
				InsertIntoFrame(crc, 0, DNP_BYTES_PER_CRC);		// Insert CRC
				i += OctetsThisBlock;
				UserDataBytesThisFrame -= OctetsThisBlock;
				OctetsThisBlock = 16;
				FirstCrcIndex = XmitBufSize[XmitHead];
			}		
			IncrementTransmitHead();				// Done with another frame
		}		
	}
	SendFrames();									// Send all frames
 std::cout<<"}SendCompiledMessage_"<<'\n';
}
