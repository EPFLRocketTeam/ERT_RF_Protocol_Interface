#ifndef RADIO_PACKET_H
#define RADIO_PACKET_H

#include <stdint.h>

#define IGNITION_CODE 0X434C //CL
#define CMD_ACTIVE 0b1111
#define CMD_INACTIVE 0b0000
#define RF_PREFIX 'Y' //0b01011001

//aim for 100 bytes
typedef struct __attribute__((__packed__)) radio_packet { //change name to PacketAV_downlink
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
    //rf_cmd engine_state;


} radio_packet_t;

const uint32_t radio_packet_size = sizeof(radio_packet_t);
//const uint32_t PacketAV_downlink_size = sizeof(PacketAV_downlink);

//must be 32 bits
typedef struct __attribute__((__packed__)) rf_cmd {
	unsigned int ventN20 : 4;
	unsigned int ventEthanol : 4;
	unsigned int servoN20 : 4;
	unsigned int servoEthanol : 4;
	unsigned int pressurization : 4;
	unsigned int abort : 4;
	unsigned int error : 4;
	unsigned int other : 4;
} rf_cmd_t;


typedef struct __attribute__((__packed__)) PacketAV_uplink {
	uint8_t prefix;
	uint8_t cmd_counter;
	rf_cmd_t cmd;
	uint16_t cmd_ignition;
} PacketAV_uplink_t;

const uint32_t PacketAV_uplink_size = sizeof(PacketAV_uplink_t);


// reception de cmd exemple
// if opcode data correct blabla
/*
PacketAV_uplink_t packetAV_uplink;
uint8_t* byte_array; // flux de byte de Martin
memcpy(&packetAV_uplink, byte_array, PacketAV_uplink_size);
printf("%u", packetAV_uplink.ventN2O);
if (packetAV_uplink.ventN2O == CMD_ACTIVE )
*/

const uint8_t radio_packet_opcode = 0x00;




#endif /* RADIO_PACKET_H */
