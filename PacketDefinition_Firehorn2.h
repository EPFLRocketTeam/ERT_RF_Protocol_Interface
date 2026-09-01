///////////////////////////////////////////////////////////////////////////////////////////                                                                                                                                      
//  Packet definitions for Firehorn II Project 2025-2026
//
//  Cyprien Lacassagne (SE C-AV C-EGS) & Ikrami Smail (TL C-EGS) & Théo Hollender (TL C-AV)
///////////////////////////////////////////////////////////////////////////////////////////
#ifndef PACKET_DEFINITION_FIREHORN2_H
#define PACKET_DEFINITION_FIREHORN2_H

#include <stdint.h>  // for uint8/16/32_t
#include <stddef.h>  // for size_t

#define ACTIVE 					0xAC // 0xAC for ACtive 
#define INACTIVE 				0xDE // 0xDE for DEsactive

////////////////////////////////////////////////////////////////
// ---------------------- AV MACROS ------------------------  // 
/* FLS Configuration */
#define FLS_CAPA        0
#define FLS_DIFF        1
#define FLS_TEMP        2
#define FLS_CONFIG      FLS_TEMP

/* AV valves state map (0: open, 1: closed) */
#define AV_VALVE_SDPR_LOX   (1 << 5)    // SPO-nc
#define AV_VALVE_SDPR_FUEL  (1 << 4)    // SPE-nc
#define AV_VALVE_VENT_LOX	(1 << 3)    // VO-no
#define AV_VALVE_VENT_FUEL	(1 << 2)    // VE-no
#define AV_VALVE_MAIN_LOX	(1 << 1)    // MO-nc
#define AV_VALVE_MAIN_FUEL	(1 << 0)    // ME-nc

/* Cameras recording map (0: rec OFF, 1: rec ON) */
#define AV_CAMERA_SEPMEC    (1 << 2)
#define AV_CAMERA_AERO_TOP  (1 << 1)
#define AV_CAMERA_AERO_BOT  (1 << 0)

/* Pyro channels map (0: open circuit, 1 : continuity) */
#define AV_PYRO_CH4         (1 << 3)
#define AV_PYRO_CH3         (1 << 2)
#define AV_PYRO_CH2         (1 << 1)
#define AV_PYRO_CH1         (1 << 0)

// ---------------------- GSE MACROS -----------------------  //
/* GSE valves state map (0: open, 1: closed) */
#define GSE_VALVE_GQD1_NC   (1 << 13)   // Quick Disconnect
#define GSE_VALVE_GQD2_NC   (1 << 12)   // Quick Disconnect
#define GSE_VALVE_GQD3_NC   (1 << 11)   // Quick Disconnect
#define GSE_VALVE_GQD4_NC   (1 << 10)   // Quick Disconnect
#define GSE_VALVE_GQD5_NC   (1 << 9)   // Quick Disconnect
#define GSE_VALVE_GQD6_NC   (1 << 8)    // Quick Disconnect
#define GSE_VALVE_GPN_NC    (1 << 7)    // Controls the opening of the high pressure N2 bottle
#define GSE_VALVE_GVN_NO    (1 << 6)    // Vents the N2 tube before disconnect
#define GSE_VALVE_GPA_NC    (1 << 5)    // Controls the activation of the pressure booster
#define GSE_VALVE_GFO_NCC   (1 << 4)    // Controls LOX filling    
#define GSE_VALVE_GDO_NCC   (1 << 3)    // Vents the tube before disconnect
#define GSE_VALVE_GFD_NC    (1 << 2)    // Low mass flow anti-freeze LOX filling line
#define GSE_VALVE_GDD_NC    (1 << 1)    // Low mass flow anti-freeze LOX disconnect
#define GSE_VALVE_GFE_NC    (1 << 0)    // Controls the filling of ethanol along with the pump
////////////////////////////////////////////////////////////////

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
	AV_CMD_DPR_LOX,
	AV_CMD_DPR_FUEL,
	AV_CMD_MAIN_LOX,
	AV_CMD_MAIN_FUEL,
	AV_CMD_VENT_LOX,
	AV_CMD_VENT_FUEL,
    AV_CMD_SDPR_LOX,
    AV_CMD_SDPR_FUEL,
	/* GSE FSM*/
	GSE_CMD_IDLE,
	GSE_CMD_ARM,
	GSE_CMD_CALIBRATE,
	GSE_CMD_PASSIVATE,
	/* GSE Manual */
	GSE_CMD_SERVO_1,
	GSE_CMD_SERVO_2,
	GSE_CMD_TOGGLE_GQD1,
	GSE_CMD_TOGGLE_GQD2,
	GSE_CMD_TOGGLE_GQD3,
	GSE_CMD_TOGGLE_GQD4,
	GSE_CMD_TOGGLE_GQD5,
	GSE_CMD_TOGGLE_GQD6,
	GSE_CMD_TOGGLE_GPN,
	GSE_CMD_TOGGLE_GVN,
	GSE_CMD_TOGGLE_GPA,
	GSE_CMD_TOGGLE_GFO,
	GSE_CMD_TOGGLE_GDO,
	GSE_CMD_TOGGLE_GFD,
	GSE_CMD_TOGGLE_GDD,
	GSE_CMD_TOGGLE_GFE,
	GSE_CMD_TOGGLE_PUMP,
	GSE_CMD_TOGGLE_ALL_GQD,
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
	uint8_t  HPO_pressure          : 8;   //      bbbbbbbb              | 0,450    | 2    | bar
    uint8_t  HPO_temp              : 6;   //         bbbbb              | 0,80     | 2    | °C
   	uint8_t  HPE_pressure          : 8;   //      bbbbbbbb              | 0,450    | 2    | bar
    uint8_t  HPE_temp              : 6;   //         bbbbb              | 0,80     | 2    | °C
	uint16_t fuel_pressure		   : 10;  //        bbbbbb.bbb          | 0,100    | 0.1  | bar
	// uint8_t  fuel_temp          : 6;   //        bbbbbb              | 0,80     | 2    | °C
	uint16_t LOX_pressure 		   : 10;  //        bbbbbb.bbb          | 0,100    | 0.1  | bar
	// int8_t   LOX_temp     	   : 8;   //      bbbbbbbb              | -200,80  | 2    | °C
#if defined FLS_CONFIG && FLS_CONFIG == FLS_CAPA
	uint16_t LOX_fls_capa          : 11;  //    bbbbbbbbbb.b            | 0,1000   | 0.5  | pF
#elif defined FLS_CONFIG && FLS_CONFIG == FLS_DIFF
	uint16_t LOX_fls_diff_bot      : 10;  //       bbbbbbb.bbb          | 0,100    | 0.1  | bar
#elif FLS_CONFIG == FLS_TEMP
	int8_t   LOX_fls_temp_1		   : 8;   //      bbbbbbbb              | -200,80  | 2    | °C
	int8_t   LOX_fls_temp_2		   : 8;   //      bbbbbbbb              | -200,80  | 2    | °C
	int8_t   LOX_fls_temp_3		   : 8;   //      bbbbbbbb              | -200,80  | 2    | °C
	int8_t   LOX_fls_temp_4		   : 8;   //      bbbbbbbb              | -200,80  | 2    | °C
	int8_t   LOX_fls_temp_5		   : 8;   //      bbbbbbbb              | -200,80  | 2    | °C
	int8_t   LOX_fls_temp_6	       : 8;   //      bbbbbbbb              | -200,80  | 2    | °C
#endif /* FLS_CONFIG */
	uint16_t fuel_inj_pressure     : 10;  //       bbbbbbb.bbb          | 0,100    | 0.1  | bar
    uint16_t LOX_inj_pressure      : 10;  //       bbbbbbb.bbb          | 0,100    | 0.1  | bar
    uint16_t chamber_pressure      : 10;  //       bbbbbbb.bbb          | 0,100    | 0.1  | bar
    uint8_t  chamber_temp          : 7;   //       bbbbbbb              | 0,200    | 2    | °C
    uint8_t  valves_state          : 6;   // binary states of the solenoid valves inc. SDPR_XXX
	uint8_t  valve_dpr_fuel        : 7;   //       bbbbbbb              | 0,90     | 1    | °
	uint8_t  valve_dpr_LOX         : 7;   //       bbbbbbb              | 0,90     | 1    | °
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
	uint8_t  av_state     		   : 4;   // FC FSM state
	uint8_t  cam_rec               : 3;   // Cameras recording state
	uint8_t  pyro_status           : 4;   // Pyro channels continuity status
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
	float    HPO_pressure;
    float    HPO_temp;
   	float    HPE_pressure;
    float    HPE_temp;
	float    fuel_pressure;
	// float fuel_temp;
	float    LOX_pressure;
	// float LOX_temp;
#if defined FLS_CONFIG && FLS_CONFIG == FLS_CAPA
	float    LOX_fls_capa;
#elif defined FLS_CONFIG && FLS_CONFIG == FLS_DIFF
	float    LOX_fls_diff_bot;
#elif FLS_CONFIG == FLS_TEMP
	float    LOX_fls_temp_1;
	float    LOX_fls_temp_2;
	float    LOX_fls_temp_3;
	float    LOX_fls_temp_4;
	float    LOX_fls_temp_5;
	float    LOX_fls_temp_6;
#endif /* FLS_CONFIG */
    float    fuel_inj_pressure;
	float    LOX_inj_pressure;
	float    chamber_pressure;
	float    chamber_temp;
	uint8_t  valves_state;
	float    valve_dpr_fuel;
	float    valve_dpr_LOX;
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
	uint8_t  pyro_status;
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
	uint16_t valves_state; // binary state of the valves
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
