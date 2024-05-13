#ifndef _EXTERN_H
#define _EXTERN_H

extern int16_t GetXmitFull();
extern void TimeSliceMrzs();
extern int16_t InitNetwork();
extern void CheckForNetworkConnection();
extern int16_t CheckForNetworkMessage(char *msg);
extern void ProcessDNPMessageMrzs(char *msg, int16_t len);
extern void NetworkSend(char *msg, int16_t length);
extern void StartCDOANInit();//char *FileName);
extern void FinishCDOANInit();
extern void StartComm();
extern void NewConnection();
extern char CommunicationMode;
extern char DNPMessage[NETWORK_MAX_MESSAGE_SIZE + DNP_MAX_DATA_LINK_FRAME_SIZE_WITH_CRC];
extern int16_t CompiledMessageSize;
extern void SetOption(char *OptionName, void *OptionValue);

/*
 * serial
 */
 
////#ifndef REMOVE_SERIAL_COMMUNICATION

extern int CheckForSerialMessageMrzs(uint8_t *rawMsg);
extern int initSerial(char *portName, int32_t BaudRate);
extern void SendSerialMessage(char *msg, int length);
////extern char SerialPortName[SERIAL_PORT_NAME_MAX_SIZE + 1];
extern int32_t BaudRate;

////#endif // ifndef REMOVE_SERIAL_COMMUNICATION

/*
 * utilities
 */

extern int32_t CharsToInt(char *c, int16_t start, int16_t length);
extern void IntToChars(char *c, int16_t start, int16_t length, int32_t val);
extern char ToLowerCase(char c);
extern int32_t EncodeFloatAs32BitInt(float f);
extern int16_t GetDNPMessage(char *msg);

/*
 * 
 */

extern int16_t DNP_Port;
extern struct timeval noTimeAtAll;

/*
 * dnp.c
 */
extern char LastIncomingApplicationMessageWithoutCRC[DNP_MAX_DATA_LINK_FRAME_SIZE_WITHOUT_CRC];
extern char LastSentApplicationMessageWithoutCRC[DNP_MAX_APPLICATION_FRAGMENT_WITHOUT_CRC];
extern int16_t LengthOfLastIncomingApplicationMessageWithoutCRC;
extern int32_t LocalTimingAccuracy;
extern int16_t UnsolicitedTimeoutCountdown;
extern int16_t UnsolicitedRetriggerCountdown, MaxUnsolicitedTimeout, InitialUnsolcitedTimeout;
extern char UnsolicitedSts, UnsolicitedCapable;
extern char SupportBroadcastClearRestartIIN;
extern char SupportBroadcastDONOAck;
extern int16_t ColdRestartTime;
extern int16_t UnsolicitedDegradedStartCount;

extern int16_t ApplicationConfirmTimeout;
extern int16_t ApplicationConfirmTimer;



extern void UnsolcitedAcknowledgeTimeout();

extern int16_t DNP_Network_Port;
extern uint16_t OutstationAddress;
extern uint16_t MasterAddress;
extern int16_t AnalogDeadband;
extern int16_t RunningCounterDeadband;
extern int16_t NeedTimeFrequency;
extern int16_t TimeInvalidFrequency;
extern int16_t DelayMeasurementTime;
extern int16_t SelectOperateMaximumTimeInSecconds;
extern int16_t MaxControlsPerRequest;
extern char SupportSelfAddress;

extern unsigned char UnsolicitedSequenceNumber;

extern char BinaryOutputStatusEventClass;
extern char AnalogOutputStatusEventClass;
extern char BinaryEventClass;
extern char AnalogEventClass;
extern char CounterEventClass;
extern char SubsetLevel;
extern char Software_Version[];
extern char Hardware_Version[];
extern char Installed_Location[];
extern char User_id[];
extern char Device_name[];
extern char Serial_Number[];
extern char Product_Name[];
extern char Manufacturers_Name[];
extern char AnalogDefaultStaticVariation; 
extern char RunningCounterDefaultStaticVariation; 
extern char FrozenCounterDefaultStaticVariation; 
extern char BinaryDefaultStaticVariation; 
extern char AnalogDefaultEventVariation; 
extern char RunningCounterDefaultEventVariation; 
extern char FrozenCounterDefaultEventVariation; 
extern char BinaryDefaultEventVariation; 
extern char AnalogOutputStatusDefaultStaticVariation;
extern char AnalogDeadbandDefaultVariation;
extern char UseUTCTime;
extern char AllowAIAndDIInSameRequest;
extern char ActionOnAck;
extern void UnsolicitedAcknowledgeTimeout();
extern void DNPCrc(char *msg, int16_t start, int16_t length, char *crc);
//extern uint16_t DNPCrc(uint8_t* input1, uint16_t length);
extern char EventGroupToStaticGroup(char EventObjectGroup);
extern char StaticGroupToEventGroup(char EventObjectGroup);
extern void InitDNP();
extern void SendInitialUnsolicited();
extern void SendUnsolicitedEvents();
extern int32_t FileHandle;				// Non-zero if file is open

#ifndef REMOVE_FILE_OBJECT_GROUP_SUPPORT
extern char FileNameBeingReadorWritten[MAX_FILE_NAME_SIZE+1];
extern char FileBaseFolder[MAX_FILE_NAME_SIZE+1];
extern int32_t AllowedFileOptions;
#endif

/*
 * non-XML
 */
 
extern int64_t TimeOffset;
extern int64_t RecordedTime;
extern int16_t SelectOperateTimer;
extern int16_t NeedTimeCountdown;
extern int16_t TimeInvalidCountdown;
extern int16_t InternalIndications;
extern int16_t LastBroadcastAddress;
extern unsigned char ExpectedConfirmSequenceNumber;
extern unsigned char UnsolcitedSeqeunceNumber;
extern char LinkIsReset, ExpectedFCB;
extern char SendThisDeviceAttribute[DNP_DEV_ATT_HIGHEST - DNP_DEV_ATT_LOWEST + 3];
extern char SupportThisDeviceAttribute[DNP_DEV_ATT_HIGHEST - DNP_DEV_ATT_LOWEST + 1];

/*
 * Comm.c
 */

extern int16_t SpaceLeft(int16_t WeNeedThis);
extern int16_t GetApplicationOffset();
extern void ReplaceApplicationInt(int16_t val, int16_t location, int16_t length);
extern void CreateDataLink(char ControlByte, uint16_t Destination, uint16_t source, char Unsolicited);
extern void SendCompiledMessage();
extern void AddApplicationData(char *buf, int16_t length);
extern void AddApplicationInt(int32_t val, int16_t length);
// extern int16_t XmitHead, XmitTail, XmitFull;
extern int16_t InternalIndications;
extern void InitComm();
extern void RevertApplicationOffset(char Unsolicited);

/*
 *  Points.c
 */
 
extern void PrintBinaryQueueInfo();		// DEBUG ONLY
extern int16_t AnyPoints(char DNPPointType);
////extern BinaryEventQueue_t *BinaryEventQueue;
extern int16_t EventQueueSize;
////extern points_h *PointData;
extern int16_t MaximumPointCount;
extern int16_t AnythingLeftToSend(int16_t what);
extern void PrintPointTable();
extern void AddPoints(char DNPType, int16_t index, int16_t deadband);
extern int16_t FindPoint(char DNYType, int16_t index);
extern void SetReadFlag(char flag, char PointType, char variation, int16_t Start, int16_t Stop, char Explicit);
extern void AddStatics();
extern int16_t AddBinaryEvents(int16_t MaxEvents, char Unsolicited);
extern void EDUnsolcited(int16_t DNPObject, int16_t Variation, char SetOrReset, int16_t Start, int16_t Stop);
extern void QueueUnsolicitedEvents();

extern int16_t AddNonBinaryEvents(int16_t MaxEvents);
extern void ClearAllStaticAndEventFlags();
extern void EventsWereConfirmed();
extern void EventsWereNotConfirmed();
extern int16_t BinaryEventSpaceLeft();
extern int16_t AnyUnsolcitedEvents();

extern int16_t UpdatePointValue(char DNPType, int16_t index, int32_t iValue, float fValue, char Flags, char ValueType);
extern int16_t UpdatePointValue16(char DNPType, int16_t index, int16_t value, char flags);
extern int16_t UpdatePointValue32(char DNPType, int16_t index, int32_t value, char flags);
extern int16_t UpdatePointValueFloat(char DNPType, int16_t index, float value, char flags);
extern void initPoints();
extern int64_t LinuxTimeToDNPTime();
extern void SetEventFlagByClass(char DNPCLass);
extern char SomeBinaryEventsWereSent;
extern void InitControlRequest(char DNPFunc, char ApplicationControlOctet);
extern int16_t AddControlPoint(char DNPObjectGroup, int16_t DNPPointIndex, char QualifierCode, char ControlOctet, int32_t Value, float fValue, int32_t offtime, char ValueType);
extern int16_t AllSBOValidated();
extern void FinshedCompilingControlRequest();
extern void AddObjectGroup0();


extern void FreezeCounters(int16_t AndClear);
extern int16_t SetDeadband(int16_t DNPIndex, int16_t val);
extern void AddDeadbands();
extern void ChangeEventClass(char ObjectType, char newClass, int16_t start, int16_t stop);
extern void RecalculateCLassIINs();
extern char *DNPTimeToChar(int64_t DNPTime);
/*
 * Network
 */
extern int16_t TCPConnected();


/*
 * User routines
 */
////extern int16_t UserControlRequest(char ApplicationFunctionCode, Control_t cRequest);
extern void ColdRestart();


#endif
