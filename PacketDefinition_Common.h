///////////////////////////////////////////////////////////////////////////////////////                                                                                                                                             
//  Packet definitions for all projects @ERT
//
//  Maxime Rochat (TL C-GS) & Cyprien Lacassagne (TL C-AV) & Samuel Wahba (SE C-AV C-GS) 
///////////////////////////////////////////////////////////////////////////////////////
#ifndef PACKET_COMMON_H
#define PACKET_COMMON_H

#include <stdint.h> // for uint8_t
#include <stddef.h> // for size_t
#include <stdbool.h>

#define ACTIVE 					0xAC // 0xAC for ACtive 
#define INACTIVE 				0xDE // 0xDE for DEsactive

// This enum is used by the motherboard and the radioboard's softwares
enum CAPSULE_ID {
	GSC_CMD = 8, 			// uplink from GSC
	GSC_INTERNAL = 9,		// internal ping for GSC HW		
	GSE_TELEMETRY = 10, 	// downlink from GSE

	HOPPER_TELEMETRY = 12,	// downlink form I-AV
	AV_TELEMETRY = 12,		// downlink form C-AV
	ABORT_BOARD = 13,		// abort commands

	GSC_INTERNAL_UPLINK = 21,
	GSC_INTERNAL_VEHICLE_DOWNLINK = 22,
	GSC_INTERNAL_GSE_DOWNLINK = 23,
};

#ifdef __cplusplus
extern "C" {
	typedef struct __attribute__((__packed__)) {
		uint64_t timestamp;
		int32_t rssi;
		float snr;
		uint8_t error;
	} gsc_internal_t;
}
const uint32_t gsc_internal_size = sizeof(gsc_internal_t);
#endif

#endif
