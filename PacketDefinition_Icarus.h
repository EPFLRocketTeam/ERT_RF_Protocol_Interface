
///////////////////////////////////////////////////////////////////////////////////////                                                                                                                                             
//  Packet definitions for Nordend Project 2023
//
//  Charlotte Heibig & Lionel Isoz & Yohan Hadji & Iacopo Sprenger
///////////////////////////////////////////////////////////////////////////////////////
#ifndef PACKET_NORDEND_H
#define PACKET_NORDEND_H

#include <stdint.h> // for uint8_t
#include <stddef.h> // for size_t
#include <stdbool.h>


#define ERT_PREFIX 				((uint32_t) ('R' << 24 | 'F' << 16 | 'B' << 8 | 'G'))

#define ACTIVE 					0xAC // 0xAC for ACtive 
#define INACTIVE 				0xDE // 0xDE for DEsactive

#define IGNITION_CODE 	0x42  // "66"  // in order_value


///////////////////////////////////////////////////////////////////////////////////////

// /!\ Flash again the MCU Mainboard
enum CAPSULE_ID {
	//////////////////////////////////
	// Rocket & GSE
	AV_TELEMETRY = 8,
	GSE_TELEMETRY,
	HOPPER_DOWNLINK,
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
	HOPPER_CMD_SERVO_N2O = 3,
	HOPPER_CMD_SERVO_FUEL,
	HOPPER_CMD_VENT_N2O,
	HOPPER_CMD_VENT_FUEL,
	HOPPER_CMD_N2_SOL,
	HOPPER_CMD_GIMBALL_X,
	HOPPER_CMD_GIMBALL_Y,
	HOPPER_CMD_CALIBRATE,
	HOPPER_CMD_PRESSURIZE,
	HOPPER_CMD_ARM,
	HOPPER_CMD_LAUNCH,
	HOPPER_CMD_ABORT,
	HOPPER_CMD_ID_CONFIG,
	

	GSE_CMD_FILLING_N2O,
	GSE_CMD_VENT,
	GSE_CMD_DISCONNECT,
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
    uint8_t N2O_temp;
    uint8_t N2O_vent;
    uint8_t ETH_vent;
    uint8_t N2_solenoid;
    uint8_t N2O_main;
    uint8_t ETH_main;
    
	uint32_t gnss_lon; //Data with RTK correction
    uint32_t gnss_lat; //Data with RTK correction
    uint8_t sat_nbr; //gnss : number of fixed satellite
    uint16_t gyro_x; //raw gyro
    uint16_t gyro_y; //raw gyro
    uint16_t gyro_z; //raw gyro
    uint16_t acc_x; //raw acc
    uint16_t acc_y; //raw acc
    uint16_t acc_z; //raw acc
    uint16_t baro;
    uint16_t kalman_pos_x;
    uint16_t kalman_pos_y;
    uint16_t kalman_pos_z;
    uint16_t kalman_yaw;
    uint16_t kalman_pitch;
    uint16_t kalman_roll;
    uint8_t gimbal_x;
    uint8_t gimbal_y;
    
	uint8_t HV_voltage;
    uint8_t LV_voltage;
    uint8_t AV_temp;
    uint8_t ID_config;
    uint8_t AV_state; // AV Power-up / Idle / Initialisation / Pressurization / Armed / Motor Fire-up / Automatic Flight / Forced Landing / ABORT
    uint8_t Fire_up_state; // Allumage Igniter / Check Igniter / Allumage Chambres / Check Chambres / Ready to Fly 
} AV_downlink_packet;

#ifdef __cplusplus
const uint32_t AV_downlink_packet_size = sizeof(AV_downlink_packet);
#endif


// AV UPLINK PACKET
typedef struct __attribute__((__packed__)) {
    
	uint32_t packet_nbr;
    
	uint8_t gimbal_x;
    uint8_t gimbal_y;
	uint8_t N2O_vent;
    uint8_t ETH_vent;
    uint8_t N2_solenoid;
	uint8_t N2O_main;
    uint8_t ETH_main;

    uint8_t ID_config; // ID de la config de vol voulue
    
	uint8_t cmd_init;
    uint8_t cmd_pressurize;
    uint8_t cmd_arm;
    uint8_t cmd_fire;
    uint8_t cmd_end;
    uint8_t cmd_forced;
    uint8_t cmd_abort;

    uint8_t hopper_wet_mass_launch; // (From Launch Pad -> Filling Station -> Telem)

} AV_uplink_packet;

#ifdef __cplusplus
const uint32_t AV_uplink_packet_size = sizeof(AV_uplink_packet);
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
} PacketGSE_downlink;
#ifdef __cplusplus
const uint32_t packetGSE_downlink_size = sizeof(PacketGSE_downlink);
#endif

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

#endif /* PACKET_NORDEND_H */