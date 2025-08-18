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
#define ENGINE_STATE_VENT_N2	(1 << 6)
#define ENGINE_STATE_VENT_LOX	(1 << 5)
#define ENGINE_STATE_VENT_FUEL	(1 << 4)
#define ENGINE_STATE_P_LOX	(1 << 3)
#define ENGINE_STATE_P_FUEL	(1 << 2)
#define ENGINE_STATE_MAIN_LOX	(1 << 1)
#define ENGINE_STATE_MAIN_FUEL	(1 << 0)

/* Cameras recording map (0: rec OFF, 1: rec ON) */
#define CAMERA_REC_PARACHUTE    (1 << 2)
#define CAMERA_REC_AIRFRAME_UP  (1 << 1)
#define CAMERA_REC_AIRFRAME_DN  (1 << 0)

// This enum is used by the motherboard and the radioboard's softwares
enum CAPSULE_ID {
	//////////////////////////////////
	// Rocket & GSE
	AV_TELEMETRY = 8,
	GSE_TELEMETRY,
	GSC_CMD, // uplink from GS
	ABORT_BOARD,
};

enum CMD_ID {
	NO_PACKET = 0,
	AV_CMD_CALIBRATE = 3,
	AV_CMD_RECOVER,
	AV_CMD_ARM,
	AV_CMD_IGNITION,
	AV_CMD_ABORT,
	AV_CMD_MANUAL_DEPLOY,
	AV_CMD_P_LOX,
	AV_CMD_P_FUEL,
	AV_CMD_MAIN_LOX,
	AV_CMD_MAIN_FUEL,
	AV_CMD_VENT_LOX,
	AV_CMD_VENT_FUEL,
	AV_CMD_PRESSURIZE,
	/* GSE commands left untouched, just replaced N20 with LOX */
	GSE_CMD_IDLE,
	GSE_CMD_ARM,
	GSE_CMD_CALIBRATE,
	GSE_CMD_PASSIVATE,

	GSE_CMD_SERVO_1,
	GSE_CMD_SERVO_2,

	GSE_CMD_TOGGLE_11,
	GSE_CMD_TOGGLE_12,
	GSE_CMD_TOGGLE_13,
	GSE_CMD_TOGGLE_14,
	GSE_CMD_TOGGLE_15,
	GSE_CMD_TOGGLE_16,
	GSE_CMD_TOGGLE_21,
	GSE_CMD_TOGGLE_22,
	GSE_CMD_TOGGLE_23,
	GSE_CMD_TOGGLE_24,
	GSE_CMD_TOGGLE_25,
	GSE_CMD_TOGGLE_26,
	GSE_CMD_TOGGLE_27,
	GSE_CMD_TOGGLE_28,
};


/////////////////////////////////////////////////////////////////
// Here is a template for writing new packet structures 
/*typedef struct __attribute__((__packed__)) {
	uint8_t data1;
	uint8_t data2;
	uint16_t data3;
} PacketTemplate;
#ifdef __cplusplus
const uint32_t packetTemplateSize = sizeof(PacketTemplate);
#endif*/

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
	uint8_t  engine_state 		 : 7;   // binary states of the valves
	uint8_t  av_state     		 : 4;   // FSM state
	uint8_t  cam_rec             : 3;   // Cameras recording state
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
	uint8_t  cam_rec;
} av_downlink_unpacked;

/////////////////////////////////////////////////////////////////
// ---------------------- GSE PACKETS ---------------------- // 

// AV UPLINK PACKET
typedef struct __attribute__((__packed__)) {
	uint8_t order_id;
	uint8_t order_value;
} gse_uplink_t;
#ifdef __cplusplus
const size_t gse_uplink_size = sizeof(gse_uplink_t);
#endif

typedef struct __attribute__((__packed__)) {
	uint8_t GQN_NC1; //Nitrogen and Ethanol disconnect actuation
	uint8_t GQN_NC2; //LOX disconnect actuation
	uint8_t GQN_NC3; // reserved
	uint8_t GQN_NC4; // reserved

	uint8_t GQN_NC5; // Low mass flow anti-freeze lox disconnect
	uint8_t GPN_NC1; // Controls the activation of the pressure booster

	uint8_t GPN_NC2; // Control the opening of the high pressure bottle
	uint8_t GVN_NC;  // Vents the tube before disconnect
	uint8_t GFE_NC;  // Controls the filling of ethanol along with the pump
	uint8_t GFO_NCC; // Controls LOX filling	
	uint8_t GDO_NCC; // Vent the tube before disconnect
	uint8_t PC_OLC;  // Trigger Lox disconnect and purge the tube of LOX 

	float GP1;		 // Nitrogen pressure in the filling line
	float GP2;		 // LOX pressure in the deware
	float GP3;		 // Pressure in the low-pressure side of the gas booster
	float GP4;		 // Pressure before the pneumatic valve
	float GP5;		 // Pressure in the ethanol filling line
} gse_downlink_t;
#ifdef __cplusplus
const uint32_t gse_downlink_size = sizeof(gse_downlink_t);
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
