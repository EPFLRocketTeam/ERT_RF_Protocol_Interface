///////////////////////////////////////////////////////////////////////////////////////                                                                                                                                             
//  Packet definitions for Firehorn Project 2024-2025
//
//  Maxime Rochat (TL C-GS) & Cyprien Lacassagne (TL C-AV) & Samuel Wahba (SE C-AV C-GS) 
///////////////////////////////////////////////////////////////////////////////////////
#ifndef PACKET_FIREHORN_H
#define PACKET_FIREHORN_H

#include <stdint.h> // for uint8_t
#include <stddef.h> // for size_t
#include <stdbool.h>

#define ACTIVE 					0xAC // 0xAC for ACtive 
#define INACTIVE 				0xDE // 0xDE for DEsactive

///////////////////////////////////////////////////////////////////////////////////////

/* Engine state valves map (0: open, 1: closed) */
#define ENGINE_STATE_VENT_LOX	(1 << 5)
#define ENGINE_STATE_VENT_FUEL	(1 << 4)
#define ENGINE_STATE_IGN_LOX	(1 << 3)
#define ENGINE_STATE_IGN_FUEL	(1 << 2)
#define ENGINE_STATE_MAIN_LOX	(1 << 1)
#define ENGINE_STATE_MAIN_FUEL	(1 << 0)

// This enum is used by the motherboard and the radioboard's softwares
enum CAPSULE_ID {
	//////////////////////////////////
	// Rocket & GSE
	AV_TELEMETRY = 8,
	GSE_TELEMETRY,
	GS_CMD, // uplink from GS
	//////////////////////////////////
	// Tracker
	BINOC_ATTITUDE,
	BINOC_POSITION,
	BINOC_STATUS,
	BINOC_GLOBAL_STATUS,
	//////////////////////////////////
	TRACKER_CMD,
	//////////////////////////////////
	CALIBRATE_TELEMETRY
};

enum CMD_ID {
	AV_CMD_CALIBRATE = 3,
	AV_CMD_RECOVER,
	AV_CMD_ARM,
	AV_CMD_IGNITION,
	AV_CMD_ABORT,
	AV_CMD_MANUAL_DEPLOY,
	AV_CMD_IGNITER_LOX,
	AV_CMD_IGNITER_FUEL,
	AV_CMD_MAIN_LOX,
	AV_CMD_MAIN_FUEL,
	AV_CMD_VENT_LOX,
	AV_CMD_VENT_FUEL,
	AV_CMD_PRESSURIZE,
	/* GSE commands left untouched, just replaced N20 with LOX */
	GSE_CMD_FILLING_LOX,
	GSE_CMD_VENT,
	GSE_CMD_DISCONNECT
};


/////////////////////////////////////////////////////////////////
// Here is a template for writing new packet structures 
typedef struct __attribute__((__packed__)) {
	uint8_t data1;
	uint8_t data2;
	uint16_t data3;
} PacketTemplate;
#ifdef __cplusplus
const uint32_t packetTemplateSize = sizeof(PacketTemplate);
#endif

/////////////////////////////////////////////////////////////////
// ---------------------- AV PACKETS ------------------------  // 
/////////////////////////////////////////////////////////////////

// AV UPLINK PACKET
typedef struct __attribute__((__packed__)) {
	uint8_t order_id;    // from CMD_ID
	uint8_t order_value; // only ACTIVE or INACTIVE  	254 other possibilities unconsidered
} av_uplink_t;
#ifdef __cplusplus
const size_t av_uplink_size = sizeof(av_uplink_t);
#endif

// AV DOWNLINK PACKET
typedef struct __attribute__((__packed__)) {
	//	Type	Name	         :	bits;	Representation | Range | Resolution | Unit
	uint32_t packet_nbr    		 : 19;
	int32_t	 gnss_lon      		 : 19;  // bbbbbbbbb.bbbbbbbbbb | -180,180 | 1e-3 | °
	int32_t	 gnss_lat     		 : 19;  // bbbbbbbbb.bbbbbbbbbb | -180,180 | 1e-3 | °
	uint16_t gnss_alt     		 : 10;  // integer              | 0,10000  | 10   | m
	int8_t 	 gnss_vertical_speed : 8;   // integer              | 0,1600   | 10   | km/h
	uint16_t N2_pressure  		 : 10;  // P-NCO | bbbbbbbbb.b  | 0,400    | 0.5  | bar
	uint16_t fuel_pressure		 : 10;  // P-ETA | bbbbbbbbb.b  | 0,400    | 0.5  | bar
	uint16_t LOX_pressure 		 : 10;  // P-OTA | bbbbbbbbb.b  | 0,400    | 0.5  | bar
	uint8_t  fuel_level   		 : 7;   // L-ETA | bbbbb.bb     | 0,24     | 0.25 | L
	uint8_t  LOX_level    		 : 7;   // L-OTA | bbbbb.bb     | 0,24     | 0.25 | L
	int16_t  N2_temp      		 : 9;   // T-NCO | integer      | -200,100 | 1    | °C
	int16_t  LOX_temp     		 : 9;   // T-OTA | integer      | -200,100 | 1    | °C
	int16_t  LOX_inj_temp 		 : 9;   // T-OIN | integer      | -200,100 | 1    | °C
	uint8_t  lpb_voltage  		 : 7;   // bbbbb.bb             | 0,26     | 0.25 | V
	uint8_t  hpb_voltage  		 : 7;   // bbbbb.bb             | 0,26     | 0.25 | V
	int16_t  av_fc_temp 	  	 : 9;   // integer              | -200,100 | 1    | °C
	int16_t  ambient_temp		 : 9;   // integer              | -200,100 | 1    | °C
	uint8_t  engine_state 		 : 6;   // binary states of the valves
	uint8_t  av_state     		 : 4;   // FSM state
} av_downlink_t;
#ifdef __cplusplus
const uint32_t av_downlink_size = sizeof(av_downlink_t);
#endif

// AV DOWNLINK UNPACKED DATA (NOT SENT BY RADIO)
typedef struct {
	uint32_t packet_nbr;
	float 	 gnss_lon;
	float    gnss_lat;
	float    gnss_alt;
	int8_t   gnss_vertical_speed;
	float    N2_pressure;
	float    fuel_pressure;
	float    LOX_pressure;
	float    fuel_level;
	float    LOX_level;
	int16_t  N2_temp;
	int16_t  LOX_temp;
	int16_t  LOX_inj_temp;
	float    lpb_voltage;
	float    hpb_voltage;
	int16_t  av_fc_temp;
	int16_t  ambient_temp;
	uint8_t  engine_state;
	uint8_t  av_state;
} av_downlink_unpacked;

/////////////////////////////////////////////////////////////////
// ---------------------- GSE PACKETS ---------------------- // 

typedef struct __attribute__((__packed__)) {
	uint8_t fillingN2O;
	uint8_t vent;
} GSE_cmd_status;
#ifdef __cplusplus
const uint32_t GSE_cmd_status_size = sizeof(GSE_cmd_status);
#endif

typedef struct __attribute__((__packed__)) {
	float tankPressure;
	float tankTemperature;
	float fillingPressure;
    GSE_cmd_status status;
	bool disconnectActive;
	int32_t loadcellRaw;
} PacketGSE_downlink;
#ifdef __cplusplus
const uint32_t packetGSE_downlink_size = sizeof(PacketGSE_downlink);
#endif

/*
/////////////////////////////////////////////////////////////////
// ---------------------- BINOC PACKETS ---------------------- // 

typedef struct __attribute__((__packed__)) {
	float azm;
	float elv;
} PacketBinocAttitude;
#ifdef __cplusplus
const uint32_t packetBinocAttitudeSize = sizeof(PacketBinocAttitude);
#endif

typedef struct __attribute__((__packed__)) {
	float lon;
	float lat;
	float alt;
} PacketBinocPosition;
#ifdef __cplusplus
const uint32_t packetBinocPositionSize = sizeof(PacketBinocPosition);
#endif

typedef struct __attribute__((__packed__)) {
	bool isInView;
	bool isCalibrated;
} PacketBinocStatus;
#ifdef __cplusplus
const uint32_t packetBinocStatusSize = sizeof(PacketBinocStatus);
#endif

typedef struct __attribute__((__packed__)) {
	PacketBinocAttitude attitude;
    PacketBinocPosition position;
    PacketBinocStatus status;
} PacketBinocGlobalStatus;
#ifdef __cplusplus
const uint32_t packetBinocGlobalStatusSize = sizeof(PacketBinocGlobalStatus);
#endif

/////////////////////////////////////////////////////////////////
// ---------------------- TRACKER PACKETS ---------------------- // 

typedef struct __attribute__((__packed__)) {
	float azm;
	float elv;
	int mode;
	float cutoffFreq;
	unsigned maxTimeWindow;
	unsigned timeStamp;
} PacketTrackerCmd;
#ifdef __cplusplus
const uint32_t packetTrackerCmdSize = sizeof(PacketTrackerCmd);
#endif
*/

#endif /* PACKET_FIREHORN_H */