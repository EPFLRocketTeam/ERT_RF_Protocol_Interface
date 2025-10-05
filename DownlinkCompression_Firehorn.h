#ifndef DOWNLINK_COMPRESSION_FIREHORN_H
#define DOWNLINK_COMPRESSION_FIREHORN_H

#include <cmath>
#include "PacketDefinition_Firehorn.h"

/**
 * @brief Encodes (compresses) AV downlink data to reduce the packet size and meet the 10Hz req.
 * @param unpacked_data 
 * @return The compressed data ready to be sent as a radio packet.
 */
inline av_downlink_t encode_downlink(const av_downlink_unpacked_t& unpacked_data) {
    av_downlink_t packet;

    packet.packet_nbr = unpacked_data.packet_nbr;

    packet.gnss_lon = ((int32_t)unpacked_data.gnss_lon << 11)
                    + abs((unpacked_data.gnss_lon - (int32_t)unpacked_data.gnss_lon) * 2048);

    packet.gnss_lat = ((int32_t)unpacked_data.gnss_lat << 11)
                    + abs((unpacked_data.gnss_lat - (int32_t)unpacked_data.gnss_lat) * 2000);

    packet.gnss_alt = (uint16_t)unpacked_data.gnss_alt / 10;

    packet.gnss_vertical_speed = (int16_t)unpacked_data.gnss_vertical_speed / 2;

    packet.N2_pressure = unpacked_data.N2_pressure / 2;

    packet.N2_temp = unpacked_data.N2_temp / 2;
    packet.N2_PT1000_temp = unpacked_data.N2_PT1000_temp / 2;

    packet.fuel_pressure = ((uint16_t)unpacked_data.fuel_pressure << 3)
                         + (unpacked_data.fuel_pressure - (uint16_t)unpacked_data.fuel_pressure) * 8;
            
    packet.LOX_pressure = ((uint16_t)unpacked_data.LOX_pressure << 3)
                        + (unpacked_data.LOX_pressure - (uint16_t)unpacked_data.LOX_pressure) * 8;
                
    packet.LOX_temp = unpacked_data.LOX_temp / 2;

    packet.LOX_inj_pressure = ((uint16_t)unpacked_data.LOX_inj_pressure << 3)
                         + (unpacked_data.LOX_inj_pressure - (uint16_t)unpacked_data.LOX_inj_pressure) * 8;

    packet.fuel_inj_pressure = ((uint16_t)unpacked_data.fuel_inj_pressure << 3)
                         + (unpacked_data.fuel_inj_pressure - (uint16_t)unpacked_data.fuel_inj_pressure) * 8;

    packet.chamber_pressure = ((uint16_t)unpacked_data.chamber_pressure << 3)
                         + (unpacked_data.chamber_pressure - (uint16_t)unpacked_data.chamber_pressure) * 8;

    packet.engine_state = unpacked_data.engine_state;

    packet.lpb_voltage = ((uint8_t)unpacked_data.lpb_voltage << 3)
                       + (unpacked_data.lpb_voltage - (uint8_t)unpacked_data.lpb_voltage) * 8;

    packet.lpb_current = ((uint8_t)unpacked_data.lpb_current << 3)
                       + (unpacked_data.lpb_current - (uint8_t)unpacked_data.lpb_current) * 8;

    packet.hpb_voltage = ((uint8_t)unpacked_data.hpb_voltage << 3)
                       + (unpacked_data.hpb_voltage - (uint8_t)unpacked_data.hpb_voltage) * 8;

    packet.hpb_current = ((uint8_t)unpacked_data.hpb_current << 1)
                       + (unpacked_data.hpb_current - (uint8_t)unpacked_data.hpb_current) * 2;

    packet.av_fc_temp = unpacked_data.av_fc_temp / 2;

    packet.ambient_temp = unpacked_data.ambient_temp / 2;

    packet.av_state = unpacked_data.av_state;

    packet.cam_rec = unpacked_data.cam_rec;

    packet.LOX_cap_fls_0 =  unpacked_data.LOX_cap_fls_0 / 2;	
    packet.LOX_fls_10 = unpacked_data.LOX_fls_10 / 2;	
    packet.LOX_fls_50 = unpacked_data.LOX_fls_50 / 2;
    packet.LOX_fls_80 = unpacked_data.LOX_fls_80 / 2;	
    packet.LOX_fls_90 = unpacked_data.LOX_fls_90 / 2;	    

    return packet;
}


/**
 * @brief Decodes (uncompresses) AV downlink packet to a readable format.
 * @param packet 
 */
inline av_downlink_unpacked_t decode_downlink(const av_downlink_t& packet) {
    av_downlink_unpacked_t unpacked_data;

    unpacked_data.packet_nbr = packet.packet_nbr;

    unpacked_data.gnss_lon = (packet.gnss_lon >> 11)
                           + (1 - 2 * (packet.gnss_lon < 0)) * (packet.gnss_lon & 0x07FF) / 2048.0;

    unpacked_data.gnss_lat = (packet.gnss_lat >> 11)
                           + (1 - 2 * (packet.gnss_lat < 0)) * (packet.gnss_lat & 0x07FF) / 2000.0;

    unpacked_data.gnss_alt = packet.gnss_alt * 10;

    unpacked_data.gnss_vertical_speed = packet.gnss_vertical_speed * 2;

    unpacked_data.N2_pressure = packet.N2_pressure * 2;
                            
    unpacked_data.fuel_pressure = (packet.fuel_pressure >> 3)
                                + (packet.fuel_pressure & 0x07) * 0.125;
    unpacked_data.fuel_pressure = round(unpacked_data.fuel_pressure * 10.0) / 10.0;

    unpacked_data.LOX_pressure = (packet.LOX_pressure >> 3)
                               + (packet.LOX_pressure & 0x07) * 0.125;
    unpacked_data.LOX_pressure = round(unpacked_data.LOX_pressure * 10.0) / 10.0;

    unpacked_data.N2_temp = packet.N2_temp * 2;
    unpacked_data.N2_PT1000_temp = packet.N2_PT1000_temp * 2;

    unpacked_data.fuel_pressure = packet.fuel_pressure * 2;

    unpacked_data.LOX_inj_pressure = (packet.LOX_inj_pressure >> 3)
                               + (packet.LOX_inj_pressure & 0x07) * 0.125;
    unpacked_data.LOX_inj_pressure = round(unpacked_data.LOX_inj_pressure * 10.0) / 10.0;

    unpacked_data.LOX_temp = packet.LOX_temp * 2;

    unpacked_data.fuel_inj_pressure = (packet.fuel_inj_pressure >> 3)
                               + (packet.fuel_inj_pressure & 0x07) * 0.125;
    unpacked_data.fuel_inj_pressure = round(unpacked_data.fuel_inj_pressure * 10.0) / 10.0;

    unpacked_data.chamber_pressure = (packet.chamber_pressure >> 3)
                               + (packet.chamber_pressure & 0x07) * 0.125;
    unpacked_data.chamber_pressure = round(unpacked_data.chamber_pressure * 10.0) / 10.0;

    unpacked_data.engine_state = packet.engine_state;

    unpacked_data.lpb_voltage = (packet.lpb_voltage >> 3)
                              + (packet.lpb_voltage & 0x07) * 0.125;

    unpacked_data.lpb_current = (packet.lpb_current >> 3)
                              + (packet.lpb_current & 0x07) * 0.125;
                        
    unpacked_data.hpb_voltage = (packet.hpb_voltage >> 3)
                              + (packet.hpb_voltage & 0x07) * 0.125;

    unpacked_data.hpb_current = (packet.hpb_current >> 1)
                              + (packet.hpb_current & 0x01) * 0.5;

    unpacked_data.av_fc_temp = packet.av_fc_temp;

    unpacked_data.av_state = packet.av_state;

    unpacked_data.cam_rec = packet.cam_rec;

    unpacked_data.LOX_cap_fls_0 =  packet.LOX_cap_fls_0 * 2;	
    unpacked_data.LOX_fls_10 = packet.LOX_fls_10 * 2;	
    unpacked_data.LOX_fls_50 = packet.LOX_fls_50 * 2;
    unpacked_data.LOX_fls_80 = packet.LOX_fls_80 * 2;	
    unpacked_data.LOX_fls_90 = packet.LOX_fls_90 * 2;	

    return unpacked_data;
}

#endif /* DOWNLINK_COMPRESSION_FIREHORN_H */
