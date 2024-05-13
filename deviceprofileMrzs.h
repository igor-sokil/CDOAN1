#ifndef _DEVICE_PROFILE_H
#define _DEVICE_PROFILE_H

#define SUBSET_LEVEL 			2
#define OUTSTATION_ADDRESS		1024
#define MASTER_ADDRESS			1 // MASTER_ADDRESS is valid when not allowing unsolcited mode
#define UNSOLICITED_CAPABLE		FALSE
#define USE_UTC_TIME			TRUE
#define SUPPORT_SELF_ADDRESS	FALSE
#define SUPPORT_BROADCAST_CLEAR_RESTART	TRUE
#define SUPPORT_BROADCAST_DIRECT_OPERATE_NOACK	TRUE
#define COLD_RESTART_TIME		60	// In seconds. Set to 0 to not support cold_restart

/*
 * Timer equates
 */
#define NEED_TIME_FREQUENCY		300		// 5 minutes, set to 0 if you never need to be synced from the master
#define TIME_INVALID_FREQUENCY	360		// 6 minutes, extra time to allow master to synchronize
#define DELAY_MEASUREMENT_TIME	10		// msec, used in response to delay measurement comand
#define INITIAL_UNSOLICITED_ACK_TIMEOUT	2		// Secomds. Timeout increases 1 second for every reattempt
#define MAXIMUM_UNSOLICITED_ACK_TIMEOUT	60		// Secomds
#define MAX_TIME_BETWEEN_SELECT_AND_OPERATE 2	// Seconds
#define APPLICATION_CONFIRM_TIMEOUT	3			// Seconds

#endif