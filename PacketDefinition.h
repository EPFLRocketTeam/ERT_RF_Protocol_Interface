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
//  Charlotte Heibig & Lionel Isoz & Yohan Hadji
//  25.03.2023 
///////////////////////////////////////////////////////////////////////////////////////
#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

// The line under has been commented by Yohan to make sure that the prefix should not be used anywhere
// because Capsule is already taking care of prefixes so using another prefixe would either be useless or dangerous. 
//#define RF_PREFIX 					'Y' //0b01011001

#define IGNITION_CODE 				0X434C //CL

#define ACTIVE 					0xAC // 0xAC for ACtive 
#define INACTIVE 				0xDE // 0xDE for DEsactive

///////////////////////////////////////////////////////////////////////////////////////

// /!\ Flash again the MCU Mainboard
enum CAPSULE_ID {
    // Packet to AV
    AV_TELEMETRY = 8,
    GSE_TELEMETRY,
    ABORT,
    IGNITION,

    //////////////////////////////////
    BEGIN_AV_UP_ID,

    AV_CMD_VALVE_N2O,
    AV_CMD_VALVE_FUEL,

    END_AV_UP_ID,
    //////////////////////////////////
    BEGIN_GSE_UP_ID,

    GSE_FILLING_N2O,
    GSE_VENT,

    END_GSE_UP_ID,
	//////////////////////////////////
	BINOC_ATTITUDE,
	BINOC_POSITION,
	BINOC_STATUS,
	BINOC_GLOBAL_STATUS,
	//////////////////////////////////
	TRACKER_CMD,
	//////////////////////////////////
	CALIBRATE_TELEMETRY


};

// TODO: clean everything once AV data to send are defined

// Test: text written from CLion from ERT2023GS repo toto
// I think it is working !
// And this message is written from STM32Cube IDE !!

///////////////////////////////////////////////////////////////////////////////////////

// Here is a template for writing new packet structures 
typedef struct __attribute__((__packed__)) {
	uint8_t data1;
	uint8_t data2;
	uint16_t data3;
} PacketTemplate;
const uint32_t packetTemplateSize = sizeof(PacketTemplate);

// ---------------------- GSE PACKETS ---------------------- // 

typedef struct __attribute__((__packed__)) {
	unsigned int fillingN2O = INACTIVE;
	unsigned int vent = ACTIVE;
} GSE_cmd_status;
const uint32_t GSE_cmd_status_size = sizeof(GSE_cmd_status);

typedef struct __attribute__((__packed__)) {
	// TODO: @Avioncis update for Nordend 2023 Mission
	int32_t tankPressure;
	float tankTemperature;
	int32_t fillingTemperature;

    GSE_cmd_status status;
    
} PacketGSE_downlink;
const uint32_t packetGSE_downlink_size = sizeof(PacketGSE_downlink);

// ---------------------- AV PACKETS ---------------------- // 

typedef struct __attribute__((__packed__)) {
	uint8_t ventN20 		= ACTIVE;
	uint8_t ventEthanol 	= ACTIVE;
	uint8_t servoN20 		= INACTIVE;
	uint8_t servoEthanol 	= INACTIVE;
	uint8_t pressurization 	= INACTIVE;
	uint8_t abort 			= INACTIVE;
	uint8_t error 			= INACTIVE;
	uint8_t other 			= INACTIVE;
} AV_cmd_status;
const uint32_t AV_cmd_status_size = sizeof(AV_cmd_status);

typedef struct __attribute__((__packed__)) {
//	uint8_t prefix;
	uint8_t value;
} Packet_cmd;
const uint32_t packet_cmd_size = sizeof(Packet_cmd);

// The following definition was useless because there is a already a packet for commands
// typedef struct __attribute__((__packed__)) {
// 	uint16_t cmd_ignition;
// } Packet_ignition;

// const uint32_t packet_ignition_size = sizeof(Packet_ignition);

typedef struct __attribute__((__packed__)) {
	// TODO: @Avioncis update for Nordend 2023 Mission
	uint32_t prefix;
	uint32_t timestamp;
	int32_t acc_z;
	int32_t baro_press;
	int16_t baro_temp;
	int32_t kalman_z;
	int32_t kalman_v;
//	int32_t kalman_a;
	int32_t kalman_sigma_z;
//	float	gnss_hdop;
	float	gnss_lon;
	float	gnss_lat;
	int32_t	gnss_alt;
	uint8_t av_state;
    uint32_t packet_nbr;
    int32_t baro_alt;
    AV_cmd_status engine_state;
    
} PacketAV_downlink;
const uint32_t packetAV_downlink_size = sizeof(PacketAV_downlink);

// old packet, it will be deleted soon
typedef struct __attribute__((__packed__)) radio_packet { 
	uint32_t prefix;
	uint32_t timestamp;
	int32_t acc_z;
	int32_t baro_press;
	int16_t baro_temp;
	int32_t kalman_z;
	int32_t kalman_v;
//	int32_t kalman_a;
	int32_t kalman_sigma_z;
//	float	gnss_hdop;
	float	gnss_lon;
	float	gnss_lat;
	int32_t	gnss_alt;
	uint8_t av_state;
    uint32_t packet_nbr;
    int32_t baro_alt;
} radio_packet_t;
const uint32_t radio_packet_size = sizeof(radio_packet_t);

// ---------------------- BINOC PACKETS ---------------------- // 

typedef struct __attribute__((__packed__)) {
	float azm;
	float elv;
} PacketBinocAttitude;
const uint32_t packetBinocAttitudeSize = sizeof(PacketBinocAttitude);

typedef struct __attribute__((__packed__)) {
	float lon;
	float lat;
	float alt;
} PacketBinocPosition;
const uint32_t packetBinocPositionSize = sizeof(PacketBinocPosition);

typedef struct __attribute__((__packed__)) {
	bool isInView;
	bool isCalibrated;
} PacketBinocStatus;
const uint32_t packetBinocStatusSize = sizeof(PacketBinocStatus);

typedef struct __attribute__((__packed__)) {
	PacketBinocAttitude attitude;
    PacketBinocPosition position;
    PacketBinocStatus status;
} PacketBinocGlobalStatus;
const uint32_t packetBinocGlobalStatusSize = sizeof(PacketBinocGlobalStatus);

// ---------------------- TRACKER PACKETS ---------------------- // 

typedef struct __attribute__((__packed__)) {
	float azm;
	float elv;
	int mode;
} PacketTrackerCmd;
const uint32_t packetTrackerCmdSize = sizeof(PacketTrackerCmd);


#endif /* PACKET_H */
