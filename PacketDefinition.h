///////////////////////////////////////////////////////////////////////////////////////
//
//                                                                                                                                                            
//	RRRRRRRRRRRRRRRRR   FFFFFFFFFFFFFFFFFFFFFFBBBBBBBBBBBBBBBBB           GGGGGGGGGGGGG
//	R::::::::::::::::R  F::::::::::::::::::::FB::::::::::::::::B       GGG::::::::::::G
//	R::::::RRRRRR:::::R F::::::::::::::::::::FB::::::BBBBBB:::::B    GG:::::::::::::::G
//	RR:::::R     R:::::RFF::::::FFFFFFFFF::::FBB:::::B     B:::::B  G:::::GGGGGGGG::::G
//	  R::::R     R:::::R  F:::::F       FFFFFF  B::::B     B:::::B G:::::G       GGGGGG
//	  R::::R     R:::::R  F:::::F               B::::B     B:::::BG:::::G              
//	  R::::RRRRRR:::::R   F::::::FFFFFFFFFF     B::::BBBBBB:::::B G:u:::G              
//	  R::::o::::::::RR    F:::::::::::::::F     B:::::::::::::BB  G:::::G    GGGGGGGGGG
//	  R::::RRRRRR:::::R   F::::::a::::::::F     B::::BBBBBB:::::B G:::::G    G::::::::G
//	  R::::R     R:::::R  F::::::FFFFFFFFFF     B::::B     B:::::BG:::::G    GGGGG::::G
//	  R::::R     R:::::R  F:::::F               B::::B     B:::::BG:::::G        G::::G
//	  R::::R     R:::::R  F:::::F               B::::B     B:::::B G:::::G       G::::G
//	RR:::::R     R:::::RFF:::::::FF           BB:::::BBBBBB::::::B  G:::::GGGGGGGG::::G
//	R::::::R     R:::::RF::::::::FF           B::::::s::::::::::B    GG:::::::::::::::G
//	R::::::R     R:::::RF::::::::FF           B::::::::::::::::B       GGG::::::GGG:::G
//	RRRRRRRR     RRRRRRRFFFFFFFFFFF           BBBBBBBBBBBBBBBBB           GGGGGG   GGGG
//                                                                                                                                                                   
//  Interface header file for communication protocol 
//
//  EPFL Rocket Team
//
//  Created by
//  	Charlotte Heibig & Lionel Isoz & Yohan Hadji & Iacopo Sprenger
//  for Nordend project, on 26.07.2023
//
//  Updated for Firehorn project
///////////////////////////////////////////////////////////////////////////////////////
#ifndef PACKET_H
#define PACKET_H

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

// AV FSM states
enum class FLIGHTMODE {
	INIT = 0,
	CALIBRATION,
	ERROR_GROUND,
	MANUAL,
	ARMED,
	THRUST_SEQUENCE,
	ASCENT,
	DESCENT,
	LANDED,
	ERROR_FLIGHT
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
	uint8_t igniter_LOX;    // V3
	uint8_t igniter_fuel;   // V4
	uint8_t main_LOX;       // V5
	uint8_t main_fuel;      // V6
	uint8_t vent_LOX;       // V1
	uint8_t vent_fuel;      // V2
	/* Commented out because didn't know what to do with it
	uint8_t pressurize;
	uint8_t purge;
	uint8_t reserve;
	*/
} engine_state_t;
#ifdef __cplusplus
const uint32_t engine_state_size = sizeof(engine_state_t);
#endif

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
	uint32_t packet_nbr;
	uint32_t timestamp;
	float	 gnss_lon;   // dd.dddddd
	float	 gnss_lat;   // dd.dddddd
	float	 gnss_alt;   // m
	float	 gnss_lon_r; // dd.dddddd
	float	 gnss_lat_r; // dd.dddddd
	float	 gnss_alt_r; // m
	float 	 gnss_vertical_speed; // m/s
	/* Engine sensors */
	float    N2_pressure;       // P1
	float    fuel_pressure;     // P2
	float    LOX_pressure;      // P3
	float    fuel_level;        // L1
	float    LOX_level;         // L2
	float    engine_temp;       // T1
	float    igniter_pressure;  // P4
	float    LOX_inj_pressure;  // P5
	float    fuel_inj_pressure; // P6
	float    chamber_pressure;  // P7
	engine_state_t engine_state; // binary states of the valves
	uint8_t  av_state; // flightmode
	uint8_t  gnss_choice;
} av_downlink_t;
#ifdef __cplusplus
const uint32_t av_downlink_size = sizeof(av_downlink_t);
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
	int32_t loadcellRaw;
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

#endif /* PACKET_H */


//////////////////////////////////////////////////////////////////////
// ---------------------- FirehornGUI Server ---------------------- // 

#ifndef PACKET_DEFINITION_H
#define PACKET_DEFINITION_H


enum GUI_CMD {
	GET_SERIAL_STATUS = 0,
};


enum GUI_FIELD {
	IGNITER_LOX,
	IGNITER_FUEL,
	MAIN_LOX,
	MAIN_FUEL,
	VENT_LOX,
	VENT_FUEL,
	ORDER_ID,
	ORDER_VALUE,
	PACKET_NBR,
	TIMESTAMP,
	GNSS_LON,
	GNSS_LAT,
	GNSS_ALT,
	GNSS_LON_R,
	GNSS_LAT_R,
	GNSS_ALT_R,
	GNSS_VERTICAL_SPEED,
	N2_PRESSURE,
	FUEL_PRESSURE,
	LOX_PRESSURE,
	FUEL_LEVEL,
	LOX_LEVEL,
	ENGINE_TEMP,
	IGNITER_PRESSURE,
	LOX_INJ_PRESSURE,
	FUEL_INJ_PRESSURE,
	CHAMBER_PRESSURE,
	AV_STATE,
	GNSS_CHOICE,
	FILLINGN2O,
	VENT_GSE,
	TANK_PRESSURE,
	TANK_TEMPERATURE,
	FILLING_PRESSURE,
	DISCONNECT_ACTIVE,
	LOADCELL_RAW,
	ENGINE_STATE,
	GSE_VENT,
	GSE_CMD_STATUS,
	DOWNRANGE,
	SERIAL_STATUS,
	SERIAL_NAME_USE,
	AV_TIMER,
	GSE_TIMER,
	CMD_1,
	CMD_2,
	CMD_3,
	GUI_CMD_SET_SERIAL_STATUS,
	GUI_CMD_CALIBRATE,
	GUI_CMD_RECOVER,
	GUI_CMD_ARM,
	GUI_CMD_IGNITION,
	GUI_CMD_ABORT,
	GUI_CMD_MANUAL_DEPLOY,
	GUI_CMD_IGNITER_LOX,
	GUI_CMD_IGNITER_FUEL,
	GUI_CMD_MAIN_LOX,
	GUI_CMD_MAIN_FUEL,
	GUI_CMD_VENT_LOX,
	GUI_CMD_VENT_FUEL,
	GUI_CMD_PRESSURIZE,
	GUI_CMD_FILLING_LOX,
	GUI_CMD_VENT,
	GUI_CMD_DISCONNECT,
	HOPPER_PACKET_NBR,      // 32 bits: packet counter (10Hz update)
	HOPPER_N2O_PRESSURE,    // 16 bits (12-bit ADC value)
	HOPPER_ETH_PRESSURE,    // 16 bits
	HOPPER_N2O_TEMP,        // 8 bits
	HOPPER_N2O_VENT,        // 1 bit
	HOPPER_ETH_VENT,        // 1 bit
	HOPPER_N2O_MAIN,        // 8 bits
	HOPPER_ETH_MAIN,        // 8 bits
	HOPPER_GNSS_LON,        // 32 bits (float)
	HOPPER_GNSS_LAT,        // 32 bits (float)
	HOPPER_SAT_NBR,         // 8 bits (number of fixed satellites)
	HOPPER_GYRO_X,          // 16 bits (raw gyro)
	HOPPER_GYRO_Y,          // 16 bits
	HOPPER_GYRO_Z,          // 16 bits
	HOPPER_ACC_X,           // 16 bits (raw accelerometer)
	HOPPER_ACC_Y,           // 16 bits
	HOPPER_ACC_Z,           // 16 bits
	HOPPER_BARO,            // 16 bits (barometric reading)
	HOPPER_KALMAN_POS_X,    // 16 bits (Kalman filter position estimate)
	HOPPER_KALMAN_POS_Y,    // 16 bits
	HOPPER_KALMAN_POS_Z,    // 16 bits
	HOPPER_KALMAN_YAW,      // 16 bits (Kalman filter angle)
	HOPPER_KALMAN_PITCH,    // 16 bits
	HOPPER_KALMAN_ROLL,     // 16 bits
	HOPPER_GIMBAL_X,        // 8 bits (gimbal position)
	HOPPER_GIMBAL_Y,        // 8 bits
	HOPPER_HV_VOLTAGE,      // 8 bits (high-voltage measurement)
	HOPPER_LV_VOLTAGE,      // 8 bits (low-voltage measurement)
	HOPPER_AV_TEMP,         // 8 bits (AV temperature)
	HOPPER_ID_CONFIG,       // 8 bits (configuration ID: PID, flight mode, etc.)
	HOPPER_AV_STATE,        // 8 bits (AV state)
	HOPPER_FIELD_COUNT,      // Total number of Hopper packet fields
	UNKNOWN
};

#endif // PACKET_DEFINITION_H
