/*
 * sc.h
 * 
 * Routines only have to include this file. All other files are then included
 * 
 * Version 1.0.0
 * 
 * March 2, 2021
 */
 
#ifndef _SC_H
#define _SC_H

//
// The following equates starting with "REMOVE_" are used to remove code at compile time and are not supported in any run-time calls
//
//#define REMOVE_FILE_OBJECT_GROUP_SUPPORT		// If defined, will cause support for file operattions (Object Group 70) to be removed, saving space
//#define REMOVE_SERIAL_COMMUNICATION
// #define REMOVE_NETWORK_COMMUNICATION
/*
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <errno.h>
#include "/usr/include/dirent.h"
#include <sys/ioctl.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>

#ifndef REMOVE_SERIAL_COMMUNICATION
#include <fcntl.h>
#include <termios.h>
#include <stdint.h>
#endif // REMOVE_SERIAL_COMMUNICATION

#ifndef REMOVE_NETWORK_COMMUNICATION
#include <sys/socket.h> 
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>	// hostent
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <linux/if.h>
#endif

#ifndef REMOVE_FILE_OBJECT_GROUP_SUPPORT
#include <sys/stat.h>
#endif

*/


// #define DEBUG_PRINT	// If defined, will print each DNP message received

#define TRUE	1
#define FALSE	0

#define COMM_MODE_TCP			0x01
#define COMM_MODE_UDP			0x02
#define COMM_MODE_SERIAL		0x04

#define NETWORK_MAX_MESSAGE_SIZE	8192
#define ANY_MASTER_ADDRESS			65535

#define NO_ERROR						 	 0
#define ERROR_UNSPECIFIED					-1			// Used
#define ERROR_POINT_DOES_NOT_EXIST			-2
#define ERROR_SERIAL_PORT_OPEN_ERROR		-3
#define ERROR_NETWORK_SUPPORT_NOT_ENABLED	-4

#define MSG_SOURCE_NETWORK	1
#define MSG_SOURCE_SERIAL	2

#define ABSOLUTE_VALUE(val) (((val) >= 0) ? (val) : (0-(val)))

typedef union
{
	float f;
	int i;
} IntFloat_u;


#include "deviceprofilemRZS.h"
#include "dnpMrzs.h"
////#include "points.h"
#include "comm.h"
////#include "serial.h"
#include "externMrzs.h"


#endif	// defined _SC_H
