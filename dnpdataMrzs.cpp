/*
 * dnpdata.c
 * 
 * Version 1.0.0
 * 
 * March 2, 2021
 */
#include "header.h"

#include "scMrzs.h"

////int32_t LocalTimingAccuracy = LOCAL_TIMING_ACCURACY;

#ifndef REMOVE_FILE_OBJECT_GROUP_SUPPORT
int32_t FileHandle;				// Non-zero if file is open
int32_t	AllowedFileOptions;
char FileBaseFolder[MAX_FILE_NAME_SIZE+1];
char FileNameBeingReadorWritten[MAX_FILE_NAME_SIZE+1];
#endif
uint16_t OutstationAddress = OUTSTATION_ADDRESS;
char SupportSelfAddress = SUPPORT_SELF_ADDRESS;
char ActionOnAck;
int16_t ApplicationConfirmTimeout = APPLICATION_CONFIRM_TIMEOUT;
int16_t ApplicationConfirmTimer = 0;
/*
int16_t DNP_Network_Port = NETWORK_PORT;
uint16_t MasterAddress = MASTER_ADDRESS;
int16_t AnalogDeadband = ANALOG_DEADBAND;
int16_t RunningCounterDeadband = RUNNING_COUNTER_DEADBAND;
int16_t NeedTimeFrequency = NEED_TIME_FREQUENCY;
int16_t TimeInvalidFrequency = ((NEED_TIME_FREQUENCY == 0) ? 0 :  TIME_INVALID_FREQUENCY);
int16_t DelayMeasurementTime = DELAY_MEASUREMENT_TIME;
int16_t SelectOperateMaximumTimeInSecconds = MAX_TIME_BETWEEN_SELECT_AND_OPERATE;
int16_t MaxControlsPerRequest = MAX_CONTROLS_PER_REQUEST;
int16_t ColdRestartTime = COLD_RESTART_TIME;

char BinaryOutputStatusEventClass = BINARY_OUTPUT_STATUS_EVENT_CLASS;
char AnalogOutputStatusEventClass = ANALOG_OUTPUT_STATUS_EVENT_CLASS;
char BinaryEventClass = BINARY_EVENT_CLASS;
char AnalogEventClass = ANALOG_EVENT_CLASS;
char CounterEventClass = COUNTER_EVENT_CLASS;
char SubsetLevel = SUBSET_LEVEL;
char Software_Version[MAX_SIZE_GROUP_0_STRING+1] = {SOFTWARE_VERSION};
char Hardware_Version[MAX_SIZE_GROUP_0_STRING+1]	= {HARDWARE_VERSION};
char Installed_Location[MAX_SIZE_GROUP_0_STRING+1] = {INSTALLED_LOCATION};
char User_id[MAX_SIZE_GROUP_0_STRING+1] = {USER_ID};
char Device_name[MAX_SIZE_GROUP_0_STRING+1] = {DEVICE_NAME};
char Serial_Number[MAX_SIZE_GROUP_0_STRING+1] = {SERIAL_NUMBER};
char Product_Name[MAX_SIZE_GROUP_0_STRING+1] = {PRODUCT_NAME};
char Manufacturers_Name[MAX_SIZE_GROUP_0_STRING+1] = {MANUFACTURERS_NAME};

char AnalogDefaultStaticVariation = ANALOG_STATIC_VARIATION;
char RunningCounterDefaultStaticVariation = RUNNING_COUNTER_STATIC_VARIATION;
char FrozenCounterDefaultStaticVariation = FROZEN_COUNTER_STATIC_VARIATION;
char BinaryDefaultStaticVariation = BINARY_STATIC_VARIATION;
char AnalogDefaultEventVariation = ANALOG_EVENT_VARIATION;
char RunningCounterDefaultEventVariation = RUNNING_COUNTER_EVENT_VARIATION;
char FrozenCounterDefaultEventVariation = FROZEN_COUNTER_EVENT_VARIATION;
char BinaryDefaultEventVariation = BINARY_EVENT_VARIATION;
char AnalogOutputStatusDefaultStaticVariation = ANALOG_OUTPUT_STATUS_STATIC_VARIATION;
char AnalogDeadbandDefaultVariation;
char UseUTCTime = USE_UTC_TIME;
char AllowAIAndDIInSameRequest = BINARY_AND_ANALOG_OUT_IN_SAME_MESSAGE;
char SupportBroadcastClearRestartIIN = SUPPORT_BROADCAST_CLEAR_RESTART;
char SupportBroadcastDONOAck = SUPPORT_BROADCAST_DIRECT_OPERATE_NOACK;
char LastIncomingApplicationMessageWithoutCRC[DNP_MAX_DATA_LINK_FRAME_SIZE_WITHOUT_CRC];
int16_t LengthOfLastIncomingApplicationMessageWithoutCRC;
*/
 
int64_t TimeOffset;
int64_t RecordedTime = 0;
int16_t SelectOperateTimer;
int16_t NeedTimeCountdown;
int16_t TimeInvalidCountdown;
int16_t UnsolicitedTimeoutCountdown;
int16_t UnsolicitedDegradedStartCount = UNSOLICITED_DEGRADED_COUNT;
int16_t UnsolicitedRetriggerCountdown, MaxUnsolicitedTimeout, InitialUnsolcitedTimeout;
char UnsolicitedCapable, UnsolicitedSts;
int16_t InternalIndications;
int16_t LastBroadcastAddress;
unsigned char ExpectedConfirmSequenceNumber = 0xFF;
unsigned char UnsolicitedSequenceNumber;
char LinkIsReset, ExpectedFCB;
char SendThisDeviceAttribute[DNP_DEV_ATT_HIGHEST - DNP_DEV_ATT_LOWEST + 3];
char SupportThisDeviceAttribute[DNP_DEV_ATT_HIGHEST - DNP_DEV_ATT_LOWEST + 1];


