#ifndef DOWNLINK_COMPRESSION_FIREHORN_H
#define DOWNLINK_COMPRESSION_FIREHORN_H

#include <cmath>
#include "PacketDefinition_Firehorn.h"
#include "cmath"

/**
 * @brief Encodes (compresses) AV downlink data to reduce the packet size and meet the 10Hz req.
 * @param unpacked_data 
 * @return The compressed data ready to be sent as a radio packet.
 */
inline av_downlink_t encode_downlink(const av_downlink_unpacked& unpacked_data) {
    av_downlink_t packet;

    packet.packet_nbr = unpacked_data.packet_nbr;

    packet.gnss_lon = ((int32_t)unpacked_data.gnss_lon << 10)
                    + abs((unpacked_data.gnss_lon - (int32_t)unpacked_data.gnss_lon) * 1000);

    packet.gnss_lat = ((int32_t)unpacked_data.gnss_lat << 10)
                    + abs((unpacked_data.gnss_lat - (int32_t)unpacked_data.gnss_lat) * 1000);

    packet.gnss_alt = (uint16_t)unpacked_data.gnss_alt / 10;

    packet.gnss_vertical_speed = (int8_t)unpacked_data.gnss_vertical_speed / 10;

    packet.N2_pressure = ((uint16_t)unpacked_data.N2_pressure << 1)
                       + (unpacked_data.N2_pressure - (uint16_t)unpacked_data.N2_pressure) * 2;

    packet.fuel_pressure = ((uint16_t)unpacked_data.fuel_pressure << 1)
                        + (unpacked_data.fuel_pressure - (uint16_t)unpacked_data.fuel_pressure) * 2;
            
    packet.LOX_pressure = ((uint16_t)unpacked_data.LOX_pressure << 1)
                        + (unpacked_data.LOX_pressure - (uint16_t)unpacked_data.LOX_pressure) * 2;
                
    packet.fuel_level = ((uint8_t)unpacked_data.fuel_level << 2)
                      + (unpacked_data.fuel_level - (uint8_t)unpacked_data.fuel_level) * 4;

    packet.LOX_level = ((uint8_t)unpacked_data.LOX_level << 2)
                     + (unpacked_data.LOX_level - (uint8_t)unpacked_data.LOX_level) * 4;
                
    packet.N2_temp = (int16_t)unpacked_data.N2_temp;

    packet.LOX_temp = (int16_t)unpacked_data.LOX_temp;

    packet.LOX_inj_temp = (int16_t)unpacked_data.LOX_inj_temp;

    packet.lpb_voltage = ((uint8_t)unpacked_data.lpb_voltage << 2)
                       + (unpacked_data.lpb_voltage - (uint8_t)unpacked_data.lpb_voltage) * 4;

    packet.hpb_voltage = ((uint8_t)unpacked_data.hpb_voltage << 2)
                       + (unpacked_data.hpb_voltage - (uint8_t)unpacked_data.hpb_voltage) * 4;

    packet.av_fc_temp = (int16_t)unpacked_data.av_fc_temp;

    packet.ambient_temp = (int16_t)unpacked_data.ambient_temp;

    packet.engine_state = unpacked_data.engine_state;

    packet.av_state = unpacked_data.av_state;

    packet.cam_rec = unpacked_data.cam_rec;

    return packet;
}


/**
 * @brief Decodes (uncompresses) AV downlink packet to a readable format.
 * @param packet 
 */
inline av_downlink_unpacked decode_downlink(const av_downlink_t& packet) {
    av_downlink_unpacked unpacked_data;

    unpacked_data.packet_nbr = packet.packet_nbr;

    unpacked_data.gnss_lon = (packet.gnss_lon >> 10)
                           + (1 - 2 * (packet.gnss_lon < 0)) * (packet.gnss_lon & 0x03FF) / 1000.0;

    unpacked_data.gnss_lat = (packet.gnss_lat >> 10)
                           + (1 - 2 * (packet.gnss_lat < 0)) * (packet.gnss_lat & 0x03FF) / 1000.0;

    unpacked_data.gnss_alt = packet.gnss_alt * 10;

    unpacked_data.gnss_vertical_speed = packet.gnss_vertical_speed * 10;

    unpacked_data.N2_pressure = (packet.N2_pressure >> 1)
                              + (packet.N2_pressure & 0x01) * 0.5;
                            
    unpacked_data.fuel_pressure = (packet.fuel_pressure >> 1)
                                + (packet.fuel_pressure & 0x01) * 0.5;

    unpacked_data.LOX_pressure = (packet.LOX_pressure >> 1)
                               + (packet.LOX_pressure & 0x01) * 0.5;

    unpacked_data.fuel_level = (packet.fuel_level >> 2)
                             + (packet.fuel_level & 0x03) * 0.25;

    unpacked_data.LOX_level = (packet.LOX_level >> 2)
                            + (packet.LOX_level & 0x03) * 0.25;

    unpacked_data.N2_temp = packet.N2_temp;

    unpacked_data.LOX_temp = packet.LOX_temp;

    unpacked_data.LOX_inj_temp = packet.LOX_inj_temp;

    unpacked_data.lpb_voltage = (packet.lpb_voltage >> 2)
                              + (packet.lpb_voltage & 0x03) * 0.25;
                        
    unpacked_data.hpb_voltage = (packet.hpb_voltage >> 2)
                              + (packet.hpb_voltage & 0x03) * 0.25;

    unpacked_data.av_fc_temp = packet.av_fc_temp;

    unpacked_data.engine_state = packet.engine_state;

    unpacked_data.av_state = packet.av_state;

    unpacked_data.cam_rec = packet.cam_rec;

    return unpacked_data;
}

#endif /* DOWNLINK_COMPRESSION_FIREHORN_H */
