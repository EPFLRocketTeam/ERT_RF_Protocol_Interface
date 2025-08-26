
///////////////////////////////////////////////////////////////////////////////////////
//  Packet definitions for Nordend Project 2023
//
//  Charlotte Heibig & Lionel Isoz & Yohan Hadji & Iacopo Sprenger
///////////////////////////////////////////////////////////////////////////////////////
#ifndef PACKET_ICARUS_H
#define PACKET_ICARUS_H

#include <stdbool.h>
#include <stddef.h> // for size_t
#include <stdint.h> // for uint8_t

#define ERT_PREFIX ((uint32_t)('R' << 24 | 'F' << 16 | 'B' << 8 | 'G'))

#define ACTIVE 0xAC   // 0xAC for ACtive
#define INACTIVE 0xDE // 0xDE for DEsactive

#define IGNITION_CODE 0x42 // "66"  // in order_value

///////////////////////////////////////////////////////////////////////////////////////

enum CAPSULE_ID {
	GSC_CMD = 8, 			// uplink from GSC
	GSC_INTERNAL = 9,		// internal ping for GSC HW		
	GSE_TELEMETRY = 10, 	// downlink from GSE

	HOPPER_TELEMETRY = 12,		// downlink form AV
	ABORT_BOARD = 13,		// abort commands
};


enum CMD_ID {
  HOPPER_CMD_MAIN_N2O = 3,
  HOPPER_CMD_MAIN_FUEL,
  HOPPER_CMD_VENT_N2O,
  HOPPER_CMD_VENT_FUEL,
  HOPPER_CMD_N2_SOL,
  HOPPER_CMD_N2O_SOL,
  HOPPER_CMD_FUEL_SOL,
  HOPPER_CMD_GIMBALL_X,
  HOPPER_CMD_GIMBALL_Y,
  HOPPER_CMD_CALIBRATE,
  HOPPER_CMD_ARM,
  HOPPER_CMD_DISARM,
  HOPPER_CMD_LAUNCH,
  HOPPER_CMD_ABORT,
  HOPPER_CMD_IDLE,
  HOPPER_CMD_ID_CONFIG,
  HOPPER_CMD_TARE,
  HOPPER_CMD_HOMING_GIMBAL,
  HOPPER_CMD_HOMING_MAIN_VALVES,
  HOPPER_CMD_IGNITER,
  HOPPER_CMD_IGNITER_VALVE_FUEL,
  HOPPER_CMD_IGNITER_VALVE_N2O,

  GSE_CMD_FILLING_N2O,
  GSE_CMD_VENT,
  GSE_CMD_DISCONNECT,


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

// AV DOWNLINK PACKET
typedef struct __attribute__((__packed__)) {
  uint32_t packet_nbr;

  uint16_t N2O_pressure;
  uint16_t ETH_pressure;
  uint16_t N2O_temp;
  uint8_t N2O_vent;
  uint8_t ETH_vent;
  uint8_t N2_solenoid;
  uint8_t N2O_main;
  uint8_t ETH_main;
  uint8_t N2O_sol;
  uint8_t ETH_sol;
  uint8_t ETH_igniter;
  uint8_t N2O_igniter;
  uint8_t igniter;
  uint16_t chamber_pressure;

  uint32_t gnss_lon; // Data with RTK correction
  uint32_t gnss_lat; // Data with RTK correction
  uint8_t sat_nbr;   // gnss : number of fixed satellite
  uint16_t gyro_x;   // raw gyro
  uint16_t gyro_y;   // raw gyro
  uint16_t gyro_z;   // raw gyro
  uint16_t acc_x;    // raw acc
  uint16_t acc_y;    // raw acc
  uint16_t acc_z;    // raw acc
  uint16_t baro;
  uint16_t kalman_pos_x;
  uint16_t kalman_pos_y;
  uint16_t kalman_pos_z;
  uint16_t kalman_yaw;
  uint16_t kalman_pitch;
  uint16_t kalman_roll;
  uint8_t gimbal_x;
  uint8_t gimbal_y;
  uint8_t wet_mass;

  uint8_t HV_voltage;
  uint8_t LV_voltage;
  uint8_t AV_temp;
  uint8_t ID_config;
  uint8_t
      AV_state; // AV Power-up / Idle / Initialisation / Pressurization / Armed
                // / Motor Fire-up / Automatic Flight / Forced Landing / ABORT

} av_downlink_t;

#ifdef __cplusplus
const uint32_t av_downlink_size = sizeof(av_downlink_t);
#endif

typedef struct __attribute__((__packed__)) {
  uint8_t order_id;    // from CMD_ID
  uint8_t order_value; // only ACTIVE or INACTIVE  	254 other possibilities
                       // unconsidered
} av_uplink_t;
#ifdef __cplusplus
const size_t av_uplink_size = sizeof(av_uplink_t);
#endif

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
////////// FROM HERE, GS stuff, please don't touch //////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

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
  uint32_t loadcell1;
  uint32_t loadcell2;
  uint32_t loadcell3;
  uint32_t loadcell4;
} fs_downlink_t;
#ifdef __cplusplus
const size_t fs_downlink_size = sizeof(fs_downlink_t);
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


/////////////////////////////////////////////////////////////////
// ---------------------- BINOC PACKETS ---------------------- //

/*typedef struct __attribute__((__packed__)) {*/
/*  float azm;*/
/*  float elv;*/
/*} PacketBinocAttitude;*/
/*#ifdef __cplusplus*/
/*const uint32_t packetBinocAttitudeSize = sizeof(PacketBinocAttitude);*/
/*#endif*/
/**/
/*typedef struct __attribute__((__packed__)) {*/
/*  float lon;*/
/*  float lat;*/
/*  float alt;*/
/*} PacketBinocPosition;*/
/*#ifdef __cplusplus*/
/*const uint32_t packetBinocPositionSize = sizeof(PacketBinocPosition);*/
/*#endif*/
/**/
/*typedef struct __attribute__((__packed__)) {*/
/*  bool isInView;*/
/*  bool isCalibrated;*/
/*} PacketBinocStatus;*/
/*#ifdef __cplusplus*/
/*const uint32_t packetBinocStatusSize = sizeof(PacketBinocStatus);*/
/*#endif*/
/**/
/*typedef struct __attribute__((__packed__)) {*/
/*  PacketBinocAttitude attitude;*/
/*  PacketBinocPosition position;*/
/*  PacketBinocStatus status;*/
/*} PacketBinocGlobalStatus;*/
/*#ifdef __cplusplus*/
/*const uint32_t packetBinocGlobalStatusSize = sizeof(PacketBinocGlobalStatus);*/
/*#endif*/
/**/
/////////////////////////////////////////////////////////////////
// ---------------------- TRACKER PACKETS ---------------------- //

/*typedef struct __attribute__((__packed__)) {*/
/*  float azm;*/
/*  float elv;*/
/*  int mode;*/
/*  float cutoffFreq;*/
/*  unsigned maxTimeWindow;*/
/*  unsigned timeStamp;*/
/*} PacketTrackerCmd;*/
/*#ifdef __cplusplus*/
/*const uint32_t packetTrackerCmdSize = sizeof(PacketTrackerCmd);*/
/*#endif*/
/**/
#endif /* PACKET_NORDEND_H */
