///////////////////////////////////////////////////////////////////////////////////////                                                                                                                                             
//  Packet definitions for Firehorn II Project 2025-2026
//
//  Cyprien Lacassagne (SE C-AV C-EGS)
///////////////////////////////////////////////////////////////////////////////////////
#ifndef PACKET_DEFINITION_FIREHORN2_H
#define PACKET_DEFINITION_FIREHORN2_H

#include <stdint.h>  // for uint8/16/32_t
#include <stddef.h>  // for size_t

#define ACTIVE 					0xAC // 0xAC for ACtive 
#define INACTIVE 				0xDE // 0xDE for DEsactive

///////////////////////////////////////////////////////////////////////////////////////
// DPR Configuration
#define DPR_SOLENOID    0
#define DPR_BALL_VALVE  1
#define DPR_CONFIG      DPR_BALL_VALVE

// FLS Configuration
#define FLS_CAPA        0
#define FLS_DIFF        1
#define FLS_TEMP        2
#define FLS_CONFIG      FLS_TEMP
///////////////////////////////////////////////////////////////////////////////////////

/* Engine state valves map (0: open, 1: closed) */
#if defined DPR_CONFIG && DPR_CONFIG == DPR_SOLENOID
#define SOLENOID_VALVE_PRES_LOX     (1 << 6)
#define SOLENOID_VALVE_PRES_FUEL    (1 << 5)
#elif defined DPR_CONFIG && DPR_CONFIG == DPR_BALL_VALVE
#define SOLENOID_VALVE_SDPR_N2      (1 << 5)
#endif /* DPR_CONFIG */
#define SOLENOID_VALVE_VENT_N2	    (1 << 4)
#define SOLENOID_VALVE_VENT_LOX	    (1 << 3)
#define SOLENOID_VALVE_VENT_FUEL	(1 << 2)
#define SOLENOID_VALVE_MAIN_LOX	    (1 << 1)
#define SOLENOID_VALVE_MAIN_FUEL	(1 << 0)

/* Cameras recording map (0: rec OFF, 1: rec ON) */
#define CAMERA_REC_SEPMEC       (1 << 2)
#define CAMERA_REC_AEROCOVER_UP (1 << 1)
#define CAMERA_REC_AEROCOVER_DN (1 << 0)

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
	AV_CMD_PRES_LOX,    // Valid for both solenoid and ball-valve DPR config.
	AV_CMD_PRES_FUEL,   // Valid for both solenoid and ball-valve DPR config.
	AV_CMD_MAIN_LOX,
	AV_CMD_MAIN_FUEL,
	AV_CMD_VENT_LOX,
	AV_CMD_VENT_FUEL,
	AV_CMD_VENT_N2,
#if defined DPR_CONFIG && DPR_CONFIG == DPR_BALL_VALVE
    AV_CMD_SDPR_N2,
#endif
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
	//	Type	Name	           : size;   Representation             | Range    | Res. | Unit
	uint32_t packet_nbr    		   : 20;
	uint32_t av_timestamp          : 26;
	int32_t	 gnss_lon      		   : 20;  //    b bbbbbbbb.bbbbbbbb bbb | -180,180 | 5e-4 | °
	int32_t	 gnss_lat     		   : 20;  //    b bbbbbbbb.bbbbbbbb bbb | -180,180 | 5e-4 | °
	uint16_t gnss_alt     		   : 9;   //    b bbbbbbbb              | 0,4000   | 10   | m
	int16_t  vertical_speed        : 9;   //    b bbbbbbbb              | -350,350 | 2    | m/s
	uint8_t  absolute_speed        : 8;   //      bbbbbbbb              | 0,350    | 2    | m/s
	uint16_t agl_altitude          : 12;  // bbbb bbbbbbbb              | 0,4000   | 1    | m	
	uint8_t  N2_pressure  		   : 8;   //      bbbbbbbb              | 0,450    | 2    | bar
    uint8_t  N2_temp               : 6;   //         bbbbb              | 0,80     | 2    | °C
	uint16_t fuel_pressure		   : 10;  //        bbbbbb.bbb          | 0,100    | 0.1  | bar
	uint8_t  fuel_temp             : 6;   //        bbbbbb              | 0,80     | 2    | °C
	uint16_t LOX_pressure 		   : 10;  //        bbbbbb.bbb          | 0,100    | 0.1  | bar
	int8_t   LOX_temp     		   : 8;   //      bbbbbbbb              | -200,80  | 2    | °C
#if defined FLS_CONFIG && FLS_CONFIG == FLS_CAPA
	uint16_t fuel_fls_capa         : 11;  //    bbbbbbbbbb.b            | 0,1000   | 0.5  | pF
	uint16_t LOX_fls_capa          : 11;  //    bbbbbbbbbb.b            | 0,1000   | 0.5  | pF
#elif defined FLS_CONFIG && FLS_CONFIG == FLS_DIFF
	uint16_t fuel_fls_diff_bot     : 10;  //       bbbbbbb.bbb          | 0,100    | 0.1  | bar
	uint16_t LOX_fls_diff_bot      : 10;  //       bbbbbbb.bbb          | 0,100    | 0.1  | bar
#elif FLS_CONFIG == FLS_TEMP
	int8_t   LOX_fls_temp_1		   : 8;   //      bbbbbbbb              | -200,80  | 2    | °C
	int8_t   LOX_fls_temp_2		   : 8;   //      bbbbbbbb              | -200,80  | 2    | °C
	int8_t   LOX_fls_temp_3		   : 8;   //      bbbbbbbb              | -200,80  | 2    | °C
	int8_t   LOX_fls_temp_4		   : 8;   //      bbbbbbbb              | -200,80  | 2    | °C
	int8_t   LOX_fls_temp_5		   : 8;   //      bbbbbbbb              | -200,80  | 2    | °C
	int8_t   LOX_fls_temp_6		   : 8;   //      bbbbbbbb              | -200,80  | 2    | °C
	int8_t   LOX_fls_temp_7		   : 8;   //      bbbbbbbb              | -200,80  | 2    | °C
	int8_t   LOX_fls_temp_8		   : 8;   //      bbbbbbbb              | -200,80  | 2    | °C
#endif /* FLS_CONFIG */
	uint16_t fuel_inj_pressure     : 10;  //       bbbbbbb.bbb          | 0,100    | 0.1  | bar
    uint16_t LOX_inj_pressure      : 10;  //       bbbbbbb.bbb          | 0,100    | 0.1  | bar
    uint16_t chamber_pressure      : 10;  //       bbbbbbb.bbb          | 0,100    | 0.1  | bar
    uint8_t  chamber_temp          : 7;   //       bbbbbbb              | 0,200    | 2    | °C
#if defined DPR_CONFIG && DPR_CONFIG == DPR_SOLENOID
	uint8_t  solenoid_valves_state : 7;   // binary states of the solenoid valves
#elif defined DPR_CONFIG && DPR_CONFIG ==  DPR_BALL_VALVE
    uint8_t  solenoid_valves_state : 6;   // binary states of the solenoid valves inc. SDPR_N2
	uint8_t  fuel_ball_valve_state : 7;   //       bbbbbbb              | 0,90     | 1    | °
	uint8_t  LOX_ball_valve_state  : 7;   //       bbbbbbb              | 0,90     | 1    | °
#endif /* DPR_CONFIG */
	uint8_t  lpb_voltage  		   : 7;   //           bbb.bbbb         | 0,5      | 0.05 | V
    int8_t   lpb_current           : 8;   //          bbbb.bbbb         | -4,4     | 0.05 | A
    uint8_t  vout_5v_voltage       : 7;   //           bbb.bbbb         | 0,5.5    | 0.05 | V
    uint8_t  vout_5v_current       : 6;   //            bb.bbbb         | 0,3      | 0.05 | A
	uint8_t  hpb_main_voltage      : 8;   //         bbbbb.bbb          | 0,25.6   | 0.1  | V
    int16_t  hpb_main_current      : 9;   //        bbbbbb.bbb          | -5,20    | 0.1  | A
    uint8_t  hpb_backup_voltage    : 8;   //         bbbbb.bbb          | 0,25.6   | 0.1  | V
    int16_t  hpb_backup_current    : 9;   //        bbbbbb.bbb          | -5,20    | 0.1  | A
    uint8_t  vout_24v_voltage      : 8;   //         bbbbb.bbb          | 0,25.6   | 0.1  | V
    uint8_t  vout_24v_current      : 8;   //         bbbbb.bbb          | 0,20     | 0.1  | A
	uint8_t  av_fc_temp 	  	   : 6;   //        bbbbbb              | 0,80     | 2    | °C
	uint8_t  ambient_temp 	  	   : 6;   //        bbbbbb              | 0,80     | 2    | °C
	uint8_t  av_state     		   : 4;   // FSM state
	uint8_t  cam_rec               : 3;   // Cameras recording state
} av_downlink_t;
#ifdef __cplusplus
const uint32_t av_downlink_size = sizeof(av_downlink_t);
#endif

// AV DOWNLINK UNPACKED DATA (NOT SENT BY RADIO)
typedef struct {
	uint32_t packet_nbr;
	uint32_t av_timestamp;
	float 	 gnss_lon;
	float    gnss_lat;
	float    gnss_alt;
	float    vertical_speed; // m/s
	float    absolute_speed; // m/s
	float    agl_altitude;
	float    N2_pressure;
    float    N2_temp;
	float    fuel_pressure;
	float    fuel_temp;
	float    LOX_pressure;
	float    LOX_temp;
#if defined FLS_CONFIG && FLS_CONFIG == FLS_CAPA
	float    fuel_fls_capa;
	float    LOX_fls_capa;
#elif defined FLS_CONFIG && FLS_CONFIG == FLS_DIFF
	float    fuel_fls_diff_bot;
	float    LOX_fls_diff_bot;
#elif FLS_CONFIG == FLS_TEMP
	float    LOX_fls_temp_1;
	float    LOX_fls_temp_2;
	float    LOX_fls_temp_3;
	float    LOX_fls_temp_4;
	float    LOX_fls_temp_5;
	float    LOX_fls_temp_6;
	float    LOX_fls_temp_7;
	float    LOX_fls_temp_8;
#endif /* FLS_CONFIG */
    float    fuel_inj_pressure;
	float    LOX_inj_pressure;
	float    chamber_pressure;
	float    chamber_temp;
	uint8_t  solenoid_valves_state;
#if defined DPR_CONFIG && DPR_CONFIG ==  DPR_BALL_VALVE
	float    fuel_ball_valve_state;
	float    LOX_ball_valve_state;
#endif /* DPR_CONFIG */
	float    lpb_voltage;
	float    lpb_current;
	float    vout_5v_voltage;
	float    vout_5v_current;
	float    hpb_main_voltage;
	float    hpb_main_current;
	float    hpb_backup_voltage;
	float    hpb_backup_current;
	float    vout_24v_voltage;
	float    vout_24v_current;
	float    av_fc_temp;
	float    ambient_temp;
	uint8_t  av_state;
	uint8_t  cam_rec;
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
