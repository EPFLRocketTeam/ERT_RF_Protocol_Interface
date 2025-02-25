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


typedef struct __attribute__((__packed__)) {
	uint8_t servo_N2O;
	uint8_t servo_fuel;
	uint8_t vent_N2O;
	uint8_t vent_fuel;
	uint8_t pressurize;
	uint8_t purge;
	uint8_t reserve;
} engine_state_t;
#ifdef __cplusplus
const uint32_t engine_state_size = sizeof(engine_state_t);
#endif

// AV UPLINK PACKET
typedef struct __attribute__((__packed__)) {
	uint32_t prefix;
	uint8_t order_id; // from CMD_ID
	uint8_t order_value;  // only ACTIVE or INACTIVE  	254 other possibilities unconsidered
} av_uplink_t;
#ifdef __cplusplus
const size_t av_uplink_size = sizeof(av_uplink_t);
#endif

// ---------------------- HOPPER PACKET ---------------------- // 
typedef struct __attribute__((__packed__)) {
    uint32_t packet_nbr;      // 32 bits: packet counter (10Hz update rate)
    uint16_t N2O_pressure;    // 16 bits (only 12 bits used from ADC)
    uint16_t ETH_pressure;    // 16 bits
    uint8_t  N2O_temp;        // 8 bits
    struct {
        unsigned int N2O_vent : 1;  // 1 bit
        unsigned int ETH_vent : 1;  // 1 bit
        unsigned int N2_sol : 1;  // 1 bit
    } vents;                    // Together: 2 bits (packed with next field)
    uint8_t  N2O_main;        // 8 bits
    uint8_t  ETH_main;        // 8 bits
    float    gnss_lon;        // 32 bits (IEEE 754)
    float    gnss_lat;        // 32 bits (IEEE 754)
    uint8_t  sat_nbr;         // 8 bits: number of fixed satellites
    int16_t  gyro_x;          // 16 bits: raw gyro values
    int16_t  gyro_y;          // 16 bits
    int16_t  gyro_z;          // 16 bits
    int16_t  acc_x;           // 16 bits: raw accelerometer values
    int16_t  acc_y;           // 16 bits
    int16_t  acc_z;           // 16 bits
    int16_t  baro;            // 16 bits: barometric reading
    int16_t  kalman_pos_x;    // 16 bits: Kalman filter estimated positions
    int16_t  kalman_pos_y;    // 16 bits
    int16_t  kalman_pos_z;    // 16 bits
    int16_t  kalman_yaw;      // 16 bits: Kalman filter estimated angles
    int16_t  kalman_pitch;    // 16 bits
    int16_t  kalman_roll;     // 16 bits
    uint8_t  gimbal_x;        // 8 bits: gimbal position
    uint8_t  gimbal_y;        // 8 bits
    uint8_t  HV_voltage;      // 8 bits: high-voltage measurement
    uint8_t  LV_voltage;      // 8 bits: low-voltage measurement
    uint8_t  AV_temp;         // 8 bits: AV temperature
    uint8_t  ID_config;         // 8 bits: ID config
    uint8_t  AV_state;         // 8 bits: AV State
    // Total: 410 bits (51.25 bytes)
} PacketHopper_downlink;

#ifdef __cplusplus
const uint32_t packetHopper_downlink_size = sizeof(PacketHopper_downlink);
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
