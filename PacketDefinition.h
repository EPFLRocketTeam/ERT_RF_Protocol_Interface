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
//  EPFL Rocket Team - Nordend Project 2023
//
//  Charlotte Heibig & Lionel Isoz & Yohan Hadji & Iacopo Sprenger
//  26.07.2023 
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
	AV_CMD_SERVO_N2O = 3,
    AV_CMD_SERVO_FUEL,
	AV_CMD_VENT_N2O,
	AV_CMD_VENT_FUEL,
	GSE_FILLING_N2O,
    GSE_VENT,
	AV_CMD_DISCONNECT,
	AV_CMD_ARM,
	AV_CMD_PRESSURIZE,
	AV_CMD_ABORT,
	AV_CMD_RECOVER,
	AV_CMD_IGNITION
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
	uint8_t servo_N2O  :1;
	uint8_t servo_fuel :1;
	uint8_t vent_N2O   :1;
	uint8_t vent_fuel  :1;
	uint8_t pressurize :1;
	uint8_t purge      :1;
	uint8_t reserve    :2;
} engine_state_t;
#ifdef __cplusplus
const uint32_t engine_state_size = sizeof(engine_state_t);
#endif

// AV UPLINK PACKET


typedef struct __attribute__((__packed__)) {
#ifdef __cplusplus
	uint16_t prefix = ERT_PREFIX;
#else
	uint16_t prefix;
#endif
	uint8_t order_id; // from CMD_ID
	uint8_t order_value;  // only ACTIVE or INACTIVE  	254 other possibilities unconsidered
} av_uplink_t;
#ifdef __cplusplus
const size_t av_uplink_size = sizeof(av_uplink_t);
#endif

// AV DOWNLINK PACKET

typedef struct __attribute__((__packed__)) {
	// TODO: @Avioncis update for Nordend 2023 Mission
#ifdef __cplusplus
	uint32_t prefix = ERT_PREFIX;
#else
	uint32_t prefix;
#endif
    uint32_t packet_nbr;
	uint32_t timestamp;
	float acc_z; // g
	float acc_hg_z; // g (high g)
	float baro_press; //hPa
	float baro_temp; //C
	float	gnss_lon; //dd.dddddd
	float	gnss_lat; //dd.dddddd
	float	gnss_alt; //m
	uint16_t N2O_pressure;
    uint16_t N2O_temp;
    uint16_t fuel_pressure;
    uint16_t chamber_pressure;
	uint8_t av_state; //enum
	engine_state_t engine_state; //binaries states of the valves
    //AV_cmd_status engine_state;
} av_downlink_t;
#ifdef __cplusplus
const uint32_t av_downlink_size = sizeof(av_downlink_t);
#endif

typedef enum {
	MIAOU_RF = 0x65,
	MIAOU_GNSS = 0x69
}miaou_transfer_type;

typedef struct __attribute__((__packed__)) {
	float	gnss_lon;
	float	gnss_lat;
	float	gnss_alt;
	float   gnss_speed;
	float 	gnss_hdop;
	uint32_t time;
} av_miaou_gnss_t;
#ifdef __cplusplus
const size_t av_miaou_gnss_size = sizeof(av_miaou_gnss_t);
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
	// TODO: @Avioncis update for Nordend 2023 Mission
	int32_t tankPressure;
	float tankTemperature;
	int32_t fillingPressure;
    GSE_cmd_status status;
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
} PacketTrackerCmd;
#ifdef __cplusplus
const uint32_t packetTrackerCmdSize = sizeof(PacketTrackerCmd);
#endif

#endif /* PACKET_H */
