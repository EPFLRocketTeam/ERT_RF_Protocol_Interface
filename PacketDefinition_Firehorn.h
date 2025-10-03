///////////////////////////////////////////////////////////////////////////////////////                                                                                                                                             
//  Packet definitions for Firehorn Project 2024-2025
//
//  Maxime Rochat (TL C-GS) & Cyprien Lacassagne (TL C-AV) & Samuel Wahba (SE C-AV C-GS) 
///////////////////////////////////////////////////////////////////////////////////////
#ifndef PACKET_FIREHORN_H
#define PACKET_FIREHORN_H

#include <cstdint>
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
#define ENGINE_STATE_P_LOX	    (1 << 3)
#define ENGINE_STATE_P_FUEL	    (1 << 2)
#define ENGINE_STATE_MAIN_LOX	(1 << 1)
#define ENGINE_STATE_MAIN_FUEL	(1 << 0)

/* Cameras recording map (0: rec OFF, 1: rec ON) */
#define CAMERA_REC_PARACHUTE    (1 << 2)
#define CAMERA_REC_AIRFRAME_UP  (1 << 1)
#define CAMERA_REC_AIRFRAME_DN  (1 << 0)

enum CMD_ID {
	NO_PACKET = 0,
	/*FC FSM*/
	AV_CMD_CALIBRATE = 3,
	AV_CMD_RECOVER,
	AV_CMD_ARM,
	AV_CMD_PRESSURIZE,
	AV_CMD_LAUNCH,
	AV_CMD_ABORT,
	/* FC MANUAL*/
	AV_CMD_P_LOX,
	AV_CMD_P_FUEL,
	AV_CMD_MAIN_LOX,
	AV_CMD_MAIN_FUEL,
	AV_CMD_VENT_LOX,
	AV_CMD_VENT_FUEL,
	AV_CMD_VENT_N2,
	/* GSE FSM*/
	GSE_CMD_IDLE,
	GSE_CMD_ARM,
	GSE_CMD_CALIBRATE,
	GSE_CMD_PASSIVATE,
	/* GSE Manual */
	GSE_CMD_SERVO_1,
	GSE_CMD_SERVO_2,
	GSE_CMD_TOGGLE_GQN1,
	GSE_CMD_TOGGLE_GQN2,
	GSE_CMD_TOGGLE_GQN3,
	GSE_CMD_TOGGLE_GQN4,
	GSE_CMD_TOGGLE_GQN5,
	GSE_CMD_TOGGLE_GQN6,
	GSE_CMD_TOGGLE_GPN,
	GSE_CMD_TOGGLE_GPA,
	GSE_CMD_TOGGLE_GVN,
	GSE_CMD_TOGGLE_GFE,
	GSE_CMD_TOGGLE_GFO,
	GSE_CMD_TOGGLE_GDO,
	GSE_CMD_TOGGLE_PC,
	GSE_CMD_TOGGLE_PR,
	GSE_CMD_TOGGLE_PUMP,
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
	//	Type	Name	         : bits;   Representation          | Range    | Res. | Unit
	uint32_t packet_nbr    		 : 19;
	int32_t	 gnss_lon      		 : 20;  // b bbbbbbbb.bbbbbbbb bbb | -180,180 | 5e-4 | °
	int32_t	 gnss_lat     		 : 20;  // b bbbbbbbb.bbbbbbbb bbb | -180,180 | 5e-4 | °
	uint16_t gnss_alt     		 : 9;   // b bbbbbbbb              | 0,4000   | 10   | m
	int16_t  gnss_vertical_speed : 9;   //   bbbbbbbb              | -350,350 | 2    | m/s
	uint8_t  N2_pressure  		 : 8;   //   bbbbbbbb              | 0,450    | 2    | bar
    uint8_t  N2_temp             : 5;   //      bbbbb              | 20,80    | 2    | °C
    uint8_t  N2_PT1000_temp      : 5;   //      bbbbb              | 20,80    | 2    | °C
	uint8_t  fuel_pressure		 : 6;   //     bbbbbb              | 0,100    | 2    | bar
	uint8_t  LOX_pressure 		 : 6;   //     bbbbbb              | 0,100    | 2    | bar
	int8_t   LOX_temp     		 : 8;   //   bbbbbbbb              | -200,80  | 2    | °C
    uint8_t  LOX_inj_pressure    : 6;   //     bbbbbb              | 0,100    | 2    | bar
	int8_t 	 LOX_cap_fls_0		 : 8;	//   bbbbbbbb              | -200,80  | 2    | °C
	int8_t 	 LOX_fls_10          : 8;	//   bbbbbbbb              | -200,80  | 2    | °C
	int8_t 	 LOX_fls_50          : 8;	//   bbbbbbbb              | -200,80  | 2    | °C
	int8_t 	 LOX_fls_80          : 8;	//   bbbbbbbb              | -200,80  | 2    | °C
	int8_t 	 LOX_fls_90          : 8;	//   bbbbbbbb              | -200,80  | 2    | °C
    uint8_t  fuel_inj_pressure   : 6;   //     bbbbbb              | 0,100    | 2    | bar
    uint8_t  chamber_pressure    : 8;   //   bbbbbbbb              | 0,450    | 2    | bar
	uint8_t  engine_state 		 : 8;   // binary states of the valves
	uint8_t  lpb_voltage  		 : 6;   //        bbb.bbb          | 0,5      | 0.1  | V
    uint8_t  lpb_current         : 6;   //        bbb.bbb          | 0,4      | 0.1  | A
	uint16_t hpb_voltage  		 : 9;   //     bbbbbb.bbb          | 0,26     | 0.1  | V
    uint8_t  hpb_current         : 6;   //      bbbbb.b            | 0,30     | 0.5  | A
	uint8_t  av_fc_temp 	  	 : 5;   //      bbbbb              | 20,80    | 2    | °C
	uint8_t  ambient_temp 	  	 : 5;   //      bbbbb              | 20,80    | 2    | °C
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
	uint16_t gnss_alt;
	int16_t  gnss_vertical_speed; // m/s
	uint16_t N2_pressure;
    uint8_t  N2_temp;
    uint8_t  N2_PT1000_temp;
	uint8_t  fuel_pressure;
	uint8_t  LOX_pressure;
	int16_t  LOX_temp;
    uint8_t  LOX_inj_pressure;
	int32_t  LOX_inj_temp;
    uint8_t  fuel_inj_pressure;
	uint16_t chamber_pressure;
	uint8_t  engine_state;
	float    lpb_voltage;
    float    lpb_current;
	float    hpb_voltage;
    float    hpb_current;
	int8_t   av_fc_temp;
	int8_t   ambient_temp;
	uint8_t  av_state;
	uint8_t  cam_rec;
	float    LOX_cap_fls_0;
	float    LOX_fls_10;	
	float    LOX_fls_50;	
	float    LOX_fls_80;	
	float    LOX_fls_90;	
} av_downlink_unpacked_t;

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
	uint8_t PUMP;    // Pump status

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
