#include <iostream>
#include "header.h"

#include "scMrzs.h"

static char DeferredReadMsg[DNP_MAX_DATA_LINK_FRAME_SIZE_WITH_CRC];
static int16_t DeferredLength, DeferredSource, DeferredDest;
static char LastSolcitedResponseApplicationHeader;
static int ReadWriteFile;
static char IsADirectoryFile;
static int16_t NextBlockNumber;
static char FileBlock[256];
static int16_t SizeOfFileCommand_t;
////static DIR *Directory;
static struct dirent *DirectorytEntry;

/*
 * Process a user message that includes user data (not just data link)
 */
static void ProcessUserDataRequest(char *msg, int16_t length, uint16_t source, uint16_t dest)
{   
 std::cout<<'\n';
 std::cout<<"{ProcessUserDataRequest1"<<'\n';

        int16_t i, j, k, start, stop, MaxEvents = 0, IINOffset, ApplicationHeaderOffset, sts, newClass, ObjectSize, count;
	char variation, ObjectType, NumericFormat;
	int16_t ControlStatus, ApplicationFunctionCode, ObjectIndex, QualifierCode, BlockStartIndex;
	int64_t TimeFromMessage, LinuxTime, T56, Power256;
	IntFloat_u fi;
	struct stat st;
	char ControlCode, ObjectGroup0Requested = FALSE, DeadbandsRequested = FALSE;

        char IsABrodcastAddress = false;
/*	
	char IsABrodcastAddress = BroadcastAddress(dest);
	if (IsABrodcastAddress)
	{
		LastBroadcastAddress = dest;
		SET_IIN(IIN_BROADCAST);
	}
*/	
	CLEAR_IIN(IIN_BAD_FUNC_CODE | IIN_OBJECT_UNKNOWN | IIN_PARAMETER_ERROR);	// Unless set below		
/*
	if (ApplicationRequestIsTheSame(msg, length))					// Got same message twice
	{
		RevertApplicationOffset(FALSE);
		SendCompiledMessage();		
		return;
	}
*/

	CreateDataLink(DLFUNC_UNCONFIRMED_USER_DATA, source, dest, FALSE);
	ApplicationHeaderOffset = GetApplicationOffset();
	LastSolcitedResponseApplicationHeader = (int16_t) msg[MSG_OFFSET_APPLICATION_CONTROL_OCTET];
	AddApplicationInt(LastSolcitedResponseApplicationHeader, 1);
	AddApplicationInt(DNP_APPLICATION_FUNCTION_CODE_RESPONSE, 1);
/*	
	if ((ActionOnAck & DNP_WAITING_FOR_SOLICITED_EVENT_CONFIRMATION) &&
		(msg[MSG_OFFSET_APPLICATION_FUNCTION_OCTET] != DNP_APPLICATION_FUNCTION_CODE_CONFIRM))
			EventsWereNotConfirmed();
*/
	ApplicationFunctionCode = msg[MSG_OFFSET_APPLICATION_FUNCTION_OCTET];
	if (ApplicationFunctionCode != DNP_APPLICATION_FUNCTION_CODE_OPERATE)
		SelectOperateTimer = -1;		// No prior select
	if ((ApplicationFunctionCode != DNP_APPLICATION_FUNCTION_CODE_READ) && 
		(ApplicationFunctionCode != DNP_APPLICATION_FUNCTION_CODE_CONFIRM) && 
		!IsABrodcastAddress)
			ActionOnAck &= ~DNP_GOT_A_DELAYED_READ;	// Not read request terminates delayed read

	switch (ApplicationFunctionCode)
	{
		case DNP_APPLICATION_FUNCTION_CODE_READ:	
		{
			if (ActionOnAck & DNP_WAITING_FOR_UNSOLICITED_EVENT_CONFIRMATION)
			{
				DeferredRead(msg, length, source, dest);
				return;
			}			
			SomeBinaryEventsWereSent = FALSE;
			SendThisDeviceAttribute[DNP_DEV_ATT_SEND_INDEX_FOR_254] = SendThisDeviceAttribute[DNP_DEV_ATT_SEND_INDEX_FOR_255] = FALSE;
			ClearAllStaticAndEventFlags();							// New read
			i = MSG_OFFSET_APPLICATION_FUNCTION_OCTET + 1;			// First object
			while (i < length) 
			{
				ObjectType = msg[i++];			
				switch (ObjectType)
				{
/*
#ifndef REMOVE_FILE_OBJECT_GROUP_SUPPORT					
					case DNPOBJ_FILE_CONTROL:
					{
						FileTransport_t fv5;
						FileTransportStatus_t fv6;
						char *cptr = (char *) &fv6;					
						
						variation = msg[i++];
						QualifierCode = GetRange(msg, &i, &start, &stop);
						stop++;
						if ((variation != DNP_FILE_VARIATION_TRANSPORT) ||
							(QualifierCode != DNPQUAL_VARIABLE_LENGTH) || 
							(stop != sizeof(fv5)))
						{
							SET_IIN(IIN_PARAMETER_ERROR);
							AddApplicationInt(InternalIndications, 2);
							SendCompiledMessage();
							return;
						}
						j = sizeof(fv5.FileHandle); 
						fv5.FileHandle = CharsToInt(msg, i, sizeof(fv5.FileHandle)); 
						fv6.StatusCode = DNP_FILE_STATUS_CODE_SUCCESS;		// Default
						IntToChars(fv6.FileHandle, 0, j, fv5.FileHandle);
						i += j;
						j = sizeof(fv5.BlockNumber); 
						fv5.BlockNumber = CharsToInt(msg, i, sizeof(fv5.FileHandle)); 
						IntToChars(fv6.BlockNumber, 0, j, fv5.BlockNumber);
						i += j;
						if (fv5.FileHandle != FileHandle)
							fv6.StatusCode = DNP_FILE_STATUS_CODE_INVALID_HANDLE;
						else if (fv5.BlockNumber != NextBlockNumber)
							fv6.StatusCode = DNP_FILE_STATUS_CODE_BLOCK_SEQ;
						AddApplicationInt(InternalIndications, 2);				// IIN
						AddApplicationInt(DNPOBJ_FILE_CONTROL, 1);
						if (fv6.StatusCode == DNP_FILE_STATUS_CODE_SUCCESS)
						{
							j = sizeof(fv5) + 5;
							if (IsADirectoryFile)
							{
							    if (!DirectorytEntry)		// There are no files
							    {
									k = 0;
									FileBlock[0] = '\0';
									fv6.BlockNumber[3] |= 0x80;	// Last block
									closedir(Directory);
									IsADirectoryFile = FALSE;									
								}
								else
								{
									strcpy(FileBlock, DirectorytEntry->d_name);
									k = strlen(FileBlock);
									do
									{
										
										DirectorytEntry  = readdir(Directory);
										if (!DirectorytEntry)
											break;
									} while (!ValidFileName(DirectorytEntry->d_name));
									if (!DirectorytEntry)
									{
										fv6.BlockNumber[3] |= 0x80;	// Last block									
										closedir(Directory);
										IsADirectoryFile = FALSE;
									}									
								}
							}								
							else   // Not a directory
							{
								k = read(ReadWriteFile, FileBlock, FILE_MAXIMUM_BLOCK_SIZE);	
								j = sizeof(fv5) + k;
								if (k != FILE_MAXIMUM_BLOCK_SIZE)
									fv6.BlockNumber[3] |= 0x80;			
							}

							if (k < 0)
							{
								fv6.StatusCode = DNP_FILE_STATUS_CODE_FATAL;
								FileHandle = 0;
								close(ReadWriteFile);
							}
							else
							{
								AddApplicationInt(DNP_FILE_VARIATION_TRANSPORT, 1);
								AddApplicationInt(DNPQUAL_VARIABLE_LENGTH, 1);
								AddApplicationInt(1, 1);
								AddApplicationInt(j, 2);				// size of file transport
								AddApplicationData(fv6.FileHandle, sizeof(fv6.FileHandle));
								AddApplicationData(fv6.BlockNumber, sizeof(fv6.BlockNumber));
								AddApplicationData(FileBlock, k);	
								NextBlockNumber++;							
							}							
						}
						if (fv6.StatusCode != DNP_FILE_STATUS_CODE_SUCCESS)
						{
							AddApplicationInt(DNP_FILE_VARIATION_TRANSPORT_STATUS, 1);
							AddApplicationInt(DNPQUAL_VARIABLE_LENGTH, 1);
							AddApplicationInt(1, 1);
							AddApplicationInt(sizeof(fv6), 2);
							AddApplicationData(cptr, sizeof(fv6));
						}			
						SendCompiledMessage();
					}//case DNPOBJ_FILE_CONTROL:
					return;							// End read/File
#endif						
*/
					case DNPOBJ_DEVICE_ATTRIBUTE:
						variation = msg[i++];
						QualifierCode = GetRange(msg, &i, &start, &stop);
						if ((QualifierCode != DNPQUAL_ALL) && ((start != 0) || (stop != 0)))
							SET_IIN(IIN_PARAMETER_ERROR);
						else
						{
							switch (variation)
							{
								case DNP_DEV_ATT_VARIATION_READ_ALL_ATTRIBUTES:
									for (j = 0; j<=(DNP_DEV_ATT_HIGHEST - DNP_DEV_ATT_LOWEST); j++)
										SendThisDeviceAttribute[j] = SupportThisDeviceAttribute[j];
									SendThisDeviceAttribute[DNP_DEV_ATT_SEND_INDEX_FOR_254] = TRUE;
									ObjectGroup0Requested = TRUE;
									break;
										
								case DNP_DEV_ATT_VARIATION_READ_ATTRIBUTE_LIST:
									SendThisDeviceAttribute[DNP_DEV_ATT_SEND_INDEX_FOR_255] = TRUE;
									ObjectGroup0Requested = TRUE;
									break;				// Implement later
									
								default:
									if ((variation < DNP_DEV_ATT_LOWEST) || (variation > DNP_DEV_ATT_HIGHEST))
										SET_IIN(IIN_OBJECT_UNKNOWN);
									else
										if (SupportThisDeviceAttribute[variation - DNP_DEV_ATT_LOWEST])
										{
											SendThisDeviceAttribute[variation - DNP_DEV_ATT_LOWEST] = TRUE;
											ObjectGroup0Requested = TRUE;
											SendThisDeviceAttribute[DNP_DEV_ATT_SEND_INDEX_FOR_254] = TRUE;	// this jsut forces something to be sent
										}
										else
											SET_IIN(IIN_OBJECT_UNKNOWN);
									break;
							}						// switch : Object group 0 variation
						}							// Valid read qualifier
						break;
						
					case DNPOBJ_ANALOG_INPUT_EVENT:
					case DNPOBJ_BINARY_INPUT_EVENT:
					case DNPOBJ_DOUBLE_BINARY_INPUT_EVENT:
					case DNPOBJ_FROZEN_COUNTER_INPUT_EVENT:
					case DNPOBJ_RUNNING_COUNTER_INPUT_EVENT:
						variation = msg[i++];					
						QualifierCode = GetRange(msg, &i, &start, &stop);						
						if (QualifierCode < 0)
							SET_IIN(IIN_PARAMETER_ERROR);
						else
						{
							if ((stop - start) >= MaxEvents)
								MaxEvents = stop - start + 1;
							SetReadFlag(POINT_MISC_SEND_EVENT, EventGroupToStaticGroup(ObjectType), variation, 0, 32767, TRUE);
						}
						break;		
						
					case DNPOBJ_ANALOG_INPUT:
					case DNPOBJ_BINARY_INPUT:
					case DNPOBJ_DOUBLE_BINARY_INPUT:
					case DNPOBJ_FROZEN_COUNTER_INPUT:
					case DNPOBJ_RUNNING_COUNTER_INPUT:
					case DNPOBJ_ANALOG_OUTPUT_STATUS_INPUT:
					case DNPOBJ_BINARY_OUTPUT_STATUS_INPUT:
						variation = msg[i++];
						QualifierCode = GetRange(msg, &i, &start, &stop);						
						if (QualifierCode < 0)
							SET_IIN(IIN_PARAMETER_ERROR);
						else
							SetReadFlag(POINT_MISC_SEND_STATIC, ObjectType, variation, start, stop, TRUE);
						break;		

					case DNPOBJ_ANALOG_DEADBAND:
						variation = msg[i++];
						QualifierCode = GetRange(msg, &i, &start, &stop);						
						if (QualifierCode < 0)
							SET_IIN(IIN_PARAMETER_ERROR);
						else
						{
							SetReadFlag(POINT_MISC_SEND_DEADBAND, DNPOBJ_ANALOG_INPUT, variation, start, stop, TRUE);
							DeadbandsRequested = TRUE;
						}
						break;		
						
					case DNPOBJ_CLASS_DATA:
						variation = msg[i++];
						QualifierCode = GetRange(msg, &i, &start, &stop);						
						if (QualifierCode < 0)
							SET_IIN(IIN_PARAMETER_ERROR);
						else
						{
							if (variation == 1)
							{
								SetReadFlag(POINT_MISC_SEND_STATIC, DNPOBJ_ANALOG_OUTPUT_STATUS_INPUT, 0, start, stop, FALSE);
								SetReadFlag(POINT_MISC_SEND_STATIC, DNPOBJ_BINARY_OUTPUT_STATUS_INPUT, 0, start, stop, FALSE);
								SetReadFlag(POINT_MISC_SEND_STATIC, DNPOBJ_ANALOG_INPUT, 0, start, stop, FALSE);
								SetReadFlag(POINT_MISC_SEND_STATIC, DNPOBJ_BINARY_INPUT, 0, start, stop, FALSE);
								SetReadFlag(POINT_MISC_SEND_STATIC, DNPOBJ_DOUBLE_BINARY_INPUT, 0, start, stop, FALSE);
								SetReadFlag(POINT_MISC_SEND_STATIC, DNPOBJ_FROZEN_COUNTER_INPUT, 0, start, stop, FALSE);
								SetReadFlag(POINT_MISC_SEND_STATIC, DNPOBJ_RUNNING_COUNTER_INPUT, 0, start, stop, FALSE);
							}
							else
							{
								SetEventFlagByClass(variation - 1);
								if ((stop - start) >= MaxEvents)
									MaxEvents = stop - start + 1;
							}
						}
						break;										
				}
			}
			IINOffset = GetApplicationOffset();
			AddApplicationInt(0, 2);				// IIN
			if (!(ANY_ERR_IIN_SET(InternalIndications)))
			{
				int16_t MaxEventsToStart = MaxEvents;

				MaxEvents -= AddBinaryEvents(MaxEvents, FALSE);
				SomeBinaryEventsWereSent = MaxEventsToStart != MaxEvents;		
				MaxEvents -= AddNonBinaryEvents(MaxEvents);
				if (MaxEvents != MaxEventsToStart)
				{
					LastSolcitedResponseApplicationHeader |= AC_CON;
					ExpectedConfirmSequenceNumber = LastSolcitedResponseApplicationHeader & (AC_SEQ | AC_UNS);
					ActionOnAck |= DNP_WAITING_FOR_SOLICITED_EVENT_CONFIRMATION;
					ReplaceApplicationInt(LastSolcitedResponseApplicationHeader, ApplicationHeaderOffset, 1);
				}					
				AddStatics();
				if (DeadbandsRequested)
					AddDeadbands();
				if (ObjectGroup0Requested)
					AddObjectGroup0();
				if (AnythingLeftToSend(POINT_MISC_SEND_DEADBAND | POINT_MISC_SEND_STATIC))
				{
					LastSolcitedResponseApplicationHeader &= ~AC_FIN;
					LastSolcitedResponseApplicationHeader |= AC_CON;
					ReplaceApplicationInt(LastSolcitedResponseApplicationHeader, ApplicationHeaderOffset, 1);
					ActionOnAck |= DNP_MORE_DATA_TO_SEND;
					ExpectedConfirmSequenceNumber = LastSolcitedResponseApplicationHeader & AC_SEQ;
				}
			}
			ReplaceApplicationInt(InternalIndications, IINOffset, 2);			// In case class data flags were cleared
	}//case DNP_APPLICATION_FUNCTION_CODE_READ:	
			break;							// End Read
       }//switch (ApplicationFunctionCode)

 std::cout<<"*ApplicationFunctionCode= "<<ApplicationFunctionCode<<'\n';
 std::cout<<"}ProcessUserDataRequest1_"<<'\n';
}


void ProcessDNPMessageMrzs(char *msg, int16_t len)
{
	char crc[DNP_BYTES_PER_CRC], LastSentApplicationMessageWithoutCRC[DNP_MAX_DATA_LINK_FRAME_SIZE_WITH_CRC];
	char Supported, Broad;
	int16_t i, j, FrameSize = 0;	
	uint16_t source, dest;
	
	int16_t BlockSize = 8;
	char DataLinkFunction, DataLinkControl;
#ifdef DEBUG_PRINT
	printf("Got a DNP message of length %d", len);
	for (i=0; i<len; i++)
	{
		if (!(i & 15)) printf("\n%5d:", i);
		printf(" %02X", msg[i]);
	}
	printf("\n\n");
#endif
//
// First, validate and remove CRC bytes
//
	i = 0;
	while (TRUE)
	{
		for (j=0; j<BlockSize; j++)
			LastSentApplicationMessageWithoutCRC[FrameSize++] = msg[j+i];
		DNPCrc(msg, i, BlockSize, crc);		
		i += BlockSize + DNP_BYTES_PER_CRC;

		if ((crc[0] != msg[i-2]) || (crc[1] != msg[i-1]))	
			return;				// Bad crc
		if (i >= len)			// Checked all CRC blocks
			break;
		if ((i + 18) >= len)
			BlockSize = len - i - DNP_BYTES_PER_CRC;
		else
			BlockSize = 16;		
	}							// End CRC check

	if (!(LastSentApplicationMessageWithoutCRC[DL_CONTROL_OCTET_OFFSET] & DL_DIR))
		return;							// Wrong diection
	if (FrameSize > DNP_DATA_LINK_LENGTH_WTHOUT_CRC)
		if ((LastSentApplicationMessageWithoutCRC[MSG_OFFSET_TRANSPORT_CONTROL_OCTET] & (TH_FIN | TH_FIR)) != (TH_FIN | TH_FIR))
			return;					// Only accept single frame messages 

//
// If we get here, we have a message with:
// 		DIR is right
//  	Transport header shows a single frame
//
    dest = CharsToInt(LastSentApplicationMessageWithoutCRC, DL_DESTINATION_OCTET_OFFSET, 2);
    source = CharsToInt(LastSentApplicationMessageWithoutCRC, DL_SOURCE_OCTET_OFFSET, 2);
 std::cout<<"dest ="<<(uint16_t)dest<<'\n';
 std::cout<<"source ="<<(uint16_t)source<<'\n';

	if (dest != OutstationAddress)
	{		
		if (SupportSelfAddress && (dest == DNP_SELF_ADDRESS))
			dest = OutstationAddress;
/*
		else
		{   
			
			Supported = SupportedBroadcastFunction(LastSentApplicationMessageWithoutCRC, len);
			Broad = BroadcastAddress(dest);			
			if (!Supported || !Broad)
				return;	// Not to me
		}
*/
	}
/*
	if ((MasterAddress != ANY_MASTER_ADDRESS) && !Broad)
		if (source != MasterAddress) return;	// Not from expected master
*/
	DataLinkControl = LastSentApplicationMessageWithoutCRC[DL_CONTROL_OCTET_OFFSET];
	DataLinkFunction = DataLinkControl & (DL_PRM | DL_FCV | DL_FUNCTION_CODE);

 std::cout<<"DataLinkControl ="<<std::hex<<(uint16_t)DataLinkControl<<'\n';
 std::cout<<"DataLinkFunction ="<<std::hex<<(uint16_t)DataLinkFunction<<'\n';


	switch (DataLinkFunction)
	{
		case DLFUNC_RESETLINK:
 std::cout<<"*DLFUNC_RESETLINK!!"<<'\n';
			LinkIsReset = TRUE;
			ExpectedFCB = DL_FCB;
			CreateDataLink(DLFUNC_ACK, source, dest, FALSE);
			SendCompiledMessage();		
			break;
			
		case DLFUNC_UNCONFIRMED_USER_DATA:
 std::cout<<"*DLFUNC_UNCONFIRMED_USER_DATA!!"<<'\n';
			CreateDataLink(DLFUNC_UNCONFIRMED_USER_DATA, source, dest, FALSE);
			ProcessUserDataRequest(LastSentApplicationMessageWithoutCRC, FrameSize, source, dest);
//	SendCompiledMessage();		
			break;
/*		
		case DLFUNC_TESTLINK:
			if (LinkIsReset && ((DataLinkControl & (DL_FCB  | DL_FCV)) == (DL_FCV | ExpectedFCB)))
			{
				ExpectedFCB ^= DL_FCB;
				CreateDataLink(DLFUNC_ACK, source, dest, FALSE);	
			}
			else
				CreateDataLink(DLFUNC_NACK, source, dest, FALSE);
			SendCompiledMessage();
			break;
		
		case DLFUNC_CONFIRMED_USER_DATA:
			if (!Broad)
			{
				if (LinkIsReset)
				{
					CreateDataLink(DLFUNC_ACK, source, dest, FALSE);				
					SendCompiledMessage();
					if ((DataLinkControl & (DL_FCB  | DL_FCV)) == (DL_FCV | ExpectedFCB))
					{
						ExpectedFCB ^= DL_FCB;
						ProcessUserDataRequest(LastSentApplicationMessageWithoutCRC, FrameSize, source, dest);				
					}
				}
				else
				{
					CreateDataLink(DLFUNC_NACK, source, dest, FALSE);
					SendCompiledMessage();				
				}
			}						// Not broadcast
			else
				ProcessUserDataRequest(LastSentApplicationMessageWithoutCRC, FrameSize, source, dest);		
			break;
		
		case DLFUNC_REQUEST_LINK_STATUS:
			CreateDataLink(DLFUNC_LINK_STATUS, source, dest, FALSE);
			SendCompiledMessage();
			break;

		case DLFUNC_ACK:	
		case DLFUNC_NACK:
		case DLFUNC_LINK_STATUS:
		case DLFUNC_NOT_SUPPORTED:
			break;
*/
	}										// End Data Link Function switch

}											// End ProcessDNPMessgae


//uint16_t DNPCrc1(uint8_t* input1, uint16_t length);

/*
 * Calculate CRC
 */
/*
void DNPCrc(char *msg, int16_t start, int16_t length, char *crc)
{   int16_t data, i, j, bit;
	
	crc[0] = crc[1] = 0;			// This protects against Endien representation
	
	for (i=0; i<length; i++)
	{
		data = msg[start++] & 0xFF;
		for (j=0; j<8; j++)
		{
			bit = (crc[0] ^ data) & 1;
			
			crc[0] >>= 1;
			if (crc[1] & 1)
				crc[0] |= 0x80;
			crc[1] >>= 1;
			data >>= 1;
			if (bit)
			{
				crc[0] ^= 0xBC;
				crc[1] ^= 0xA6;
			}			
		}
	}
	crc[0] ^= 0xFF;
	crc[1] ^= 0xFF;
}
*/
static uint16_t crcTable_in_CRC[256]
  = {0x0000, 0x365E, 0x6CBC, 0x5AE2, 0xD978, 0xEF26, 0xB5C4, 0x839A, 0xFF89, 0xC9D7, 0x9335, 0xA56B, 0x26F1, 0x10AF,
     0x4A4D, 0x7C13, 0xB26B, 0x8435, 0xDED7, 0xE889, 0x6B13, 0x5D4D, 0x07AF, 0x31F1, 0x4DE2, 0x7BBC, 0x215E, 0x1700,
     0x949A, 0xA2C4, 0xF826, 0xCE78, 0x29AF, 0x1FF1, 0x4513, 0x734D, 0xF0D7, 0xC689, 0x9C6B, 0xAA35, 0xD626, 0xE078,
     0xBA9A, 0x8CC4, 0x0F5E, 0x3900, 0x63E2, 0x55BC, 0x9BC4, 0xAD9A, 0xF778, 0xC126, 0x42BC, 0x74E2, 0x2E00, 0x185E,
     0x644D, 0x5213, 0x08F1, 0x3EAF, 0xBD35, 0x8B6B, 0xD189, 0xE7D7, 0x535E, 0x6500, 0x3FE2, 0x09BC, 0x8A26, 0xBC78,
     0xE69A, 0xD0C4, 0xACD7, 0x9A89, 0xC06B, 0xF635, 0x75AF, 0x43F1, 0x1913, 0x2F4D, 0xE135, 0xD76B, 0x8D89, 0xBBD7,
     0x384D, 0x0E13, 0x54F1, 0x62AF, 0x1EBC, 0x28E2, 0x7200, 0x445E, 0xC7C4, 0xF19A, 0xAB78, 0x9D26, 0x7AF1, 0x4CAF,
     0x164D, 0x2013, 0xA389, 0x95D7, 0xCF35, 0xF96B, 0x8578, 0xB326, 0xE9C4, 0xDF9A, 0x5C00, 0x6A5E, 0x30BC, 0x06E2,
     0xC89A, 0xFEC4, 0xA426, 0x9278, 0x11E2, 0x27BC, 0x7D5E, 0x4B00, 0x3713, 0x014D, 0x5BAF, 0x6DF1, 0xEE6B, 0xD835,
     0x82D7, 0xB489, 0xA6BC, 0x90E2, 0xCA00, 0xFC5E, 0x7FC4, 0x499A, 0x1378, 0x2526, 0x5935, 0x6F6B, 0x3589, 0x03D7,
     0x804D, 0xB613, 0xECF1, 0xDAAF, 0x14D7, 0x2289, 0x786B, 0x4E35, 0xCDAF, 0xFBF1, 0xA113, 0x974D, 0xEB5E, 0xDD00,
     0x87E2, 0xB1BC, 0x3226, 0x0478, 0x5E9A, 0x68C4, 0x8F13, 0xB94D, 0xE3AF, 0xD5F1, 0x566B, 0x6035, 0x3AD7, 0x0C89,
     0x709A, 0x46C4, 0x1C26, 0x2A78, 0xA9E2, 0x9FBC, 0xC55E, 0xF300, 0x3D78, 0x0B26, 0x51C4, 0x679A, 0xE400, 0xD25E,
     0x88BC, 0xBEE2, 0xC2F1, 0xF4AF, 0xAE4D, 0x9813, 0x1B89, 0x2DD7, 0x7735, 0x416B, 0xF5E2, 0xC3BC, 0x995E, 0xAF00,
     0x2C9A, 0x1AC4, 0x4026, 0x7678, 0x0A6B, 0x3C35, 0x66D7, 0x5089, 0xD313, 0xE54D, 0xBFAF, 0x89F1, 0x4789, 0x71D7,
     0x2B35, 0x1D6B, 0x9EF1, 0xA8AF, 0xF24D, 0xC413, 0xB800, 0x8E5E, 0xD4BC, 0xE2E2, 0x6178, 0x5726, 0x0DC4, 0x3B9A,
     0xDC4D, 0xEA13, 0xB0F1, 0x86AF, 0x0535, 0x336B, 0x6989, 0x5FD7, 0x23C4, 0x159A, 0x4F78, 0x7926, 0xFABC, 0xCCE2,
     0x9600, 0xA05E, 0x6E26, 0x5878, 0x029A, 0x34C4, 0xB75E, 0x8100, 0xDBE2, 0xEDBC, 0x91AF, 0xA7F1, 0xFD13, 0xCB4D,
     0x48D7, 0x7E89, 0x246B, 0x1235
    };

//uint16_t CalcCrc_in_CRC_staticOver1(uint8_t* input1, uint16_t length)
//uint16_t DNPCrc(uint8_t* input1, uint16_t length)
void DNPCrc(char *input1, int16_t start, int16_t length, char *crc)
{
  uint16_t CRC1 = 0;

  for (int32_t i = start; i < length+start; ++i)
  {
    uint8_t index = (CRC1 ^ input1[i]) & 0xFF;
    CRC1 = crcTable_in_CRC[index] ^ (CRC1 >> 8);
  }

  CRC1 = ~(int)CRC1;
  crc[0] = CRC1 & 0xFF;
  crc[1] = (CRC1>>8) & 0xFF;
}

/*
 * initDNP() - Called once at startup
 */
void InitDNP()
{
//   int i;
//	FileCommand_t FS;
/*
 * This next statement is required since "Sizeof" on structures with variable int sizes may not work because of padding
 */
/*
	SizeOfFileCommand_t =
		sizeof(FS.FileNameStringOffset) +
		sizeof(FS.FileNameStringSize) +
		sizeof(FS.TimeOfCreation) +
		sizeof(FS.Permissions) +
		sizeof(FS.AuthenticationKey) +
		sizeof(FS.FileSize) +
		sizeof(FS.OperationalMode) +
		sizeof(FS.MaximumBlockSize) +
		sizeof(FS.RequestID);

#ifndef REMOVE_FILE_OBJECT_GROUP_SUPPORT	
	AllowedFileOptions = 0;
	FileHandle = 0;			// No file open
	strcpy(FileBaseFolder, FILE_READ_WRITE_BASE_FOLDER);

	i = strlen(FileBaseFolder);
	if (i > 0)
	{
		if (FileBaseFolder[i-1] != '/')
		{
			FileBaseFolder[i++] = '/';
			FileBaseFolder[i] = '\0';
		}
	}
	
	if (ALLOW_FILE_DELETE) AllowedFileOptions |= FILE_OPTIONS_ALLOW_DELETE;
	if (ALLOW_FILE_READ) AllowedFileOptions |= FILE_OPTIONS_ALLOW_READ;
	if (ALLOW_FILE_WRITE) AllowedFileOptions |= FILE_OPTIONS_ALLOW_WRITE;
#endif		
*/
	LastSolcitedResponseApplicationHeader = AC_FIN;				// Looks like last reqesut was complete
	UnsolicitedTimeoutCountdown = 0;
	UnsolicitedSequenceNumber = 0;
	UnsolicitedRetriggerCountdown = InitialUnsolcitedTimeout = INITIAL_UNSOLICITED_ACK_TIMEOUT;
	MaxUnsolicitedTimeout = MAXIMUM_UNSOLICITED_ACK_TIMEOUT;
	UnsolicitedCapable = UNSOLICITED_CAPABLE;
	LastBroadcastAddress = 0;
	ExpectedConfirmSequenceNumber = 0;
	ActionOnAck = 0;
	LinkIsReset = FALSE;
	InternalIndications = 0;
	TimeOffset = 0;
	SET_IIN(IIN_DEVICE_RESTART);
	SET_IIN(IIN_NEED_TIME);
	ExpectedFCB	= DL_FCB;
	
	
/*
 * Set up group 0 support
 */	
/*
	for (i=DNP_DEV_ATT_LOWEST ; i<= DNP_DEV_ATT_HIGHEST; i++)
	{
		SendThisDeviceAttribute[i-DNP_DEV_ATT_LOWEST] = FALSE;
		SupportThisDeviceAttribute[DNP_DEV_ATT_HIGHEST - DNP_DEV_ATT_LOWEST + 1] = FALSE;
	}
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_MAX_CROB_PER_REQUEST - DNP_DEV_ATT_LOWEST] = TRUE;	
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_LOCAL_TIMING_ACCURACY - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_TIME_ACCURACY_DURATION - DNP_DEV_ATT_LOWEST] = TRUE;	
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_SUPPORT_ANALOG_OUTPUT_EVENTS - DNP_DEV_ATT_LOWEST] = TRUE;	
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_MAX_ANALOG_OUTPUT_INDEX - DNP_DEV_ATT_LOWEST] = TRUE;	
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_NUMBER_OF_ANALOG_OUTPUTS - DNP_DEV_ATT_LOWEST] = TRUE;	
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_SUPPORT_BINARY_OUTPUT_EVENTS - DNP_DEV_ATT_LOWEST] = TRUE;	
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_MAX_BINARY_OUTPUT_INDEX - DNP_DEV_ATT_LOWEST] = TRUE;	
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_NUMBER_OF_BINARY_OUTPUTS - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_SUPPORT_FROZEN_COUNTER_EVENTS - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_SUPPORT_FROZEN_COUNTERS - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_SUPPORT_FOR_COUNTER_EVENTS - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_MAX_COUNTER_INDEX - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_NUMBER_OF_COUNTER_POINTS - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_SUPPORT_FROZEN_ANALOGS - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_SUPPORT_ANALOG_INPUT_EVENTS - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_MAX_ANALOG_INPUT_INDEX - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_NUMBER_OF_ANALOG_INPUTS - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_SUPPORT_DOUBLE_EVENTS - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_MAX_DOUBLE_INDEX - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_NUMBER_OF_DOUBLE_INPUTS - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_SUPPORT_BINARY_INPUT_EVENTS - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_MAX_BINARY_INPUT_INDEX - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_NUMBER_BINARY_INPUTS - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_MAX_TRANSMIT_FRAGMENT_SIZE - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_MAX_RECEIVE_FRAGMENT_SIZE - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_SOFTWARE_VERSION_NUMBER - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_HARDWARE_VERSION - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_INSTALLED_LOCATION - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_USER_ID - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_DEVICE_NAME - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_DEVICE_SERIAL_NUMBER - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_DNP_SUBSET - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_DEVICE_PRODUCT_NAME - DNP_DEV_ATT_LOWEST] = TRUE;
	SupportThisDeviceAttribute[DNP_DEV_ATT_VARIATION_MANUFACTURERS_NAME - DNP_DEV_ATT_LOWEST] = TRUE;
*/
}
