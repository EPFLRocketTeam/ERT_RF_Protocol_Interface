#ifndef DOWNLINK_COMPRESSION_FIREHORN_2_H
#define DOWNLINK_COMPRESSION_FIREHORN_2_H

#include <cmath>
#include <cstdint>
#include "PacketDefinition_Firehorn2.h"

static struct {
    uint64_t accumulated_sd_fail_count = 0;
} compression_decoder_internal_state;

static struct {
    uint64_t previous_sd_fail_count = 0;
    bool did_first_encode = false;
} compression_encoder_internal_state;

/**
 * @brief Encodes (compresses) AV downlink data to reduce the packet size and meet the 10Hz req.
 * @param unpacked_data 
 * @return The compressed data ready to be sent as a radio packet.
 */
inline void encode_downlink(av_downlink_t* packet, const av_downlink_unpacked_t& unpacked_data) {
    packet->packet_nbr = unpacked_data.packet_nbr;
    
    packet->av_timestamp = unpacked_data.av_timestamp;

    packet->gnss_lon = ((int32_t)unpacked_data.gnss_lon << 11)
                    + abs((int32_t)((unpacked_data.gnss_lon - (int32_t)unpacked_data.gnss_lon) * 2048));

    packet->gnss_lat = ((int32_t)unpacked_data.gnss_lat << 11)
                    + abs((int32_t)((unpacked_data.gnss_lat - (int32_t)unpacked_data.gnss_lat) * 2048));

    packet->gnss_alt = (uint16_t)(unpacked_data.gnss_alt / 10);

    packet->vertical_speed = (int16_t)(unpacked_data.vertical_speed / 2);
    
    packet->absolute_speed = (uint16_t)(unpacked_data.absolute_speed / 2);
    
    packet->agl_altitude = (uint16_t)unpacked_data.agl_altitude;

    packet->HPO_pressure = (uint8_t)(unpacked_data.HPO_pressure / 2);

    packet->HPO_temp = (uint8_t)(unpacked_data.HPO_temp / 2);

    packet->HPE_pressure = (uint8_t)(unpacked_data.HPE_pressure / 2);

    packet->HPE_temp = (uint8_t)(unpacked_data.HPE_temp / 2);

    packet->fuel_pressure = ((uint16_t)unpacked_data.fuel_pressure << 3)
                         + (unpacked_data.fuel_pressure - (uint16_t)unpacked_data.fuel_pressure) * 8;
    
    // packet->fuel_temp = (uint8_t)(unpacked_data.fuel_temp / 2);
            
    packet->LOX_pressure = ((uint16_t)unpacked_data.LOX_pressure << 3)
                        + (unpacked_data.LOX_pressure - (uint16_t)unpacked_data.LOX_pressure) * 8;
                
    // packet->LOX_temp = (int8_t)(unpacked_data.LOX_temp / 2);
    
#if defined FLS_CONFIG && FLS_CONFIG == FLS_CAPA
    packet->LOX_fls_capa = ((uint16_t)unpacked_data.LOX_fls_capa << 1)
                         + (unpacked_data.LOX_fls_capa - (uint16_t)unpacked_data.LOX_fls_capa) * 2;
#elif defined FLS_CONFIG && FLS_CONFIG == FLS_DIFF
    packet->LOX_fls_diff = ((uint16_t)unpacked_data.LOX_fls_diff << 3)
                         + (unpacked_data.LOX_fls_diff - (uint16_t)unpacked_data.LOX_fls_diff) * 8;
#elif FLS_CONFIG == FLS_TEMP
    packet->LOX_fls_temp_1 = (int8_t)(unpacked_data.LOX_fls_temp_1 / 2);
    packet->LOX_fls_temp_2 = (int8_t)(unpacked_data.LOX_fls_temp_2 / 2);
    packet->LOX_fls_temp_3 = (int8_t)(unpacked_data.LOX_fls_temp_3 / 2);
    packet->LOX_fls_temp_4 = (int8_t)(unpacked_data.LOX_fls_temp_4 / 2);
    packet->LOX_fls_temp_5 = (int8_t)(unpacked_data.LOX_fls_temp_5 / 2);
    packet->LOX_fls_temp_6 = (int8_t)(unpacked_data.LOX_fls_temp_6 / 2);
#endif /* FLS_CONFIG */

    packet->fuel_inj_pressure = ((uint16_t)unpacked_data.fuel_inj_pressure << 3)
                         + (unpacked_data.fuel_inj_pressure - (uint16_t)unpacked_data.fuel_inj_pressure) * 8;
    
    packet->LOX_inj_pressure = ((uint16_t)unpacked_data.LOX_inj_pressure << 3)
                         + (unpacked_data.LOX_inj_pressure - (uint16_t)unpacked_data.LOX_inj_pressure) * 8;

    packet->chamber_pressure = ((uint16_t)unpacked_data.chamber_pressure << 3)
                         + (unpacked_data.chamber_pressure - (uint16_t)unpacked_data.chamber_pressure) * 8;

    packet->chamber_temp = (uint8_t)(unpacked_data.chamber_temp / 2);
    
    packet->valves_state = (uint8_t)unpacked_data.valves_state;
    
    packet->valve_dpr_fuel = (uint8_t)unpacked_data.valve_dpr_fuel;
    packet->valve_dpr_LOX = (uint8_t)unpacked_data.valve_dpr_LOX;
    
    packet->lpb1_voltage = ((uint8_t)unpacked_data.lpb1_voltage << 4)
                       + (unpacked_data.lpb1_voltage - (uint8_t)unpacked_data.lpb1_voltage) * 16;

    packet->lpb1_current = ((int8_t)unpacked_data.lpb1_current << 4)
                       + abs((int8_t)((unpacked_data.lpb1_current - (int8_t)unpacked_data.lpb1_current) * 16));
                       
    packet->lpb2_voltage = ((uint8_t)unpacked_data.lpb2_voltage << 4)
                       + (unpacked_data.lpb2_voltage - (uint8_t)unpacked_data.lpb2_voltage) * 16;

    packet->lpb2_current = ((int8_t)unpacked_data.lpb2_current << 4)
                       + abs((int8_t)((unpacked_data.lpb2_current - (int8_t)unpacked_data.lpb2_current) * 16));

    packet->vout_5v_voltage = ((uint8_t)unpacked_data.vout_5v_voltage << 4)
                       + (unpacked_data.vout_5v_voltage - (uint8_t)unpacked_data.vout_5v_voltage) * 16;

    packet->vout_5v_current = ((uint8_t)unpacked_data.vout_5v_current << 4)
                       + (unpacked_data.vout_5v_current - (uint8_t)unpacked_data.vout_5v_current) * 16;
    
    packet->hpb_main_voltage = ((uint8_t)unpacked_data.hpb_main_voltage << 3)
                       + (unpacked_data.hpb_main_voltage - (uint8_t)unpacked_data.hpb_main_voltage) * 8;
    
    packet->hpb_main_current = ((int32_t)unpacked_data.hpb_main_current << 3)
                       + abs((int8_t)((unpacked_data.hpb_main_current - (int32_t)unpacked_data.hpb_main_current) * 8));
    
    packet->hpb_backup_voltage = ((uint8_t)unpacked_data.hpb_backup_voltage << 3)
                       + (unpacked_data.hpb_backup_voltage - (uint8_t)unpacked_data.hpb_backup_voltage) * 8;
    
    packet->hpb_backup_current = ((int8_t)unpacked_data.hpb_backup_current << 3)
                       + abs((int8_t)((unpacked_data.hpb_backup_current - (int8_t)unpacked_data.hpb_backup_current) * 8));
    
    packet->vout_24v_voltage = ((uint8_t)unpacked_data.vout_24v_voltage << 3)
                       + (unpacked_data.vout_24v_voltage - (uint8_t)unpacked_data.vout_24v_voltage) * 8;
    
    packet->vout_24v_current = ((uint8_t)unpacked_data.vout_24v_current << 3)
                       + (unpacked_data.vout_24v_current - (uint8_t)unpacked_data.vout_24v_current) * 8;

    packet->av_fc_temp = (uint8_t)(unpacked_data.av_fc_temp / 2);

    packet->ambient_temp = (uint8_t)(unpacked_data.ambient_temp / 2);

    packet->av_state = unpacked_data.av_state;

    packet->cam_rec = unpacked_data.cam_rec;

    packet->rail_cable_status = unpacked_data.rail_cable_status;

    packet->pyro_status = unpacked_data.pyro_status;

    // Hz to 100 Hz batch
    if (unpacked_data.average_imu_rate >= 25500.f) {
        packet->average_imu_rate = 255;
    } else {
        packet->average_imu_rate = (uint8_t) (unpacked_data.average_imu_rate / 100.f);
    }

    if (!compression_encoder_internal_state.did_first_encode) {
        compression_encoder_internal_state.did_first_encode = true;
        packet->sd_fail_count_dt_log2 = 0;
    } else if (compression_encoder_internal_state.previous_sd_fail_count >= unpacked_data.sd_fail_count) {
        packet->sd_fail_count_dt_log2 = 1;
    } else {
        uint64_t delta = unpacked_data.sd_fail_count - compression_encoder_internal_state.previous_sd_fail_count;
        uint64_t encoded_delta = 1;

        uint8_t delta_log2 = 2;
        while (delta_log2 != 31 && delta > encoded_delta) {
            delta_log2 ++;
            encoded_delta *= 2;
        }

        packet->sd_fail_count_dt_log2 = delta_log2;
    }

    compression_encoder_internal_state.previous_sd_fail_count = unpacked_data.sd_fail_count;

    packet->remaining_disk_size = (uint16_t) (unpacked_data.remaining_disk_size >> 25);
}


/**
 * @brief Decodes (uncompresses) AV downlink packet to a readable format.
 * @param packet 
 */
inline av_downlink_unpacked_t decode_downlink(const av_downlink_t& packet) {
    av_downlink_unpacked_t unpacked_data;

    unpacked_data.packet_nbr = packet.packet_nbr;
    
    unpacked_data.av_timestamp = packet.av_timestamp;

    unpacked_data.gnss_lon = (packet.gnss_lon >> 11)
                           + (1 - 2 * (packet.gnss_lon < 0)) * (packet.gnss_lon & 0x07FF) / 2048.0;

    unpacked_data.gnss_lat = (packet.gnss_lat >> 11)
                           + (1 - 2 * (packet.gnss_lat < 0)) * (packet.gnss_lat & 0x07FF) / 2048.0;

    unpacked_data.gnss_alt = packet.gnss_alt * 10;

    unpacked_data.vertical_speed = packet.vertical_speed * 2;
    
    unpacked_data.absolute_speed = packet.absolute_speed * 2;
    
    unpacked_data.agl_altitude = packet.agl_altitude;

    unpacked_data.HPO_pressure = packet.HPO_pressure * 2;
    
    unpacked_data.HPO_temp = packet.HPO_temp * 2;

    unpacked_data.HPE_pressure = packet.HPE_pressure * 2;
    
    unpacked_data.HPE_temp = packet.HPE_temp * 2;
                            
    unpacked_data.fuel_pressure = (packet.fuel_pressure >> 3)
                                + (packet.fuel_pressure & 0x07) * 0.125;
    unpacked_data.fuel_pressure = round(unpacked_data.fuel_pressure * 10.0) / 10.0;
    
    // unpacked_data.fuel_temp = packet.fuel_temp * 2;

    unpacked_data.LOX_pressure = (packet.LOX_pressure >> 3)
                               + (packet.LOX_pressure & 0x07) * 0.125;
    unpacked_data.LOX_pressure = round(unpacked_data.LOX_pressure * 10.0) / 10.0;
    
    // unpacked_data.LOX_temp = packet.LOX_temp * 2;
    
#if defined FLS_CONFIG && FLS_CONFIG == FLS_CAPA
    unpacked_data.LOX_fls_capa = (packet.LOX_fls_capa >> 1)
                               + (packet.LOX_fls_capa & 0x01) * 0.5;
#elif defined FLS_CONFIG && FLS_CONFIG == FLS_DIFF
    unpacked_data.LOX_fls_diff = (packet.LOX_fls_diff >> 3)
                               + (packet.LOX_fls_diff & 0x07) * 0.125;
#elif FLS_CONFIG == FLS_TEMP
    unpacked_data.LOX_fls_temp_1 = packet.LOX_fls_temp_1 * 2;
    unpacked_data.LOX_fls_temp_2 = packet.LOX_fls_temp_2 * 2;
    unpacked_data.LOX_fls_temp_3 = packet.LOX_fls_temp_3 * 2;
    unpacked_data.LOX_fls_temp_4 = packet.LOX_fls_temp_4 * 2;
    unpacked_data.LOX_fls_temp_5 = packet.LOX_fls_temp_5 * 2;
    unpacked_data.LOX_fls_temp_6 = packet.LOX_fls_temp_6 * 2;
#endif /* FLS_CONFIG */

    unpacked_data.fuel_inj_pressure = (packet.fuel_inj_pressure >> 3)
                               + (packet.fuel_inj_pressure & 0x07) * 0.125;
    unpacked_data.fuel_inj_pressure = round(unpacked_data.fuel_inj_pressure * 10.0) / 10.0;

    unpacked_data.LOX_inj_pressure = (packet.LOX_inj_pressure >> 3)
                               + (packet.LOX_inj_pressure & 0x07) * 0.125;
    unpacked_data.LOX_inj_pressure = round(unpacked_data.LOX_inj_pressure * 10.0) / 10.0;

    unpacked_data.chamber_pressure = (packet.chamber_pressure >> 3)
                               + (packet.chamber_pressure & 0x07) * 0.125;
    unpacked_data.chamber_pressure = round(unpacked_data.chamber_pressure * 10.0) / 10.0;

    unpacked_data.valves_state = packet.valves_state;

    unpacked_data.valve_dpr_fuel = packet.valve_dpr_fuel;
    unpacked_data.valve_dpr_LOX = packet.valve_dpr_LOX;

    unpacked_data.lpb1_voltage = (packet.lpb1_voltage >> 4)
                           + (packet.lpb1_voltage & 0x0F) * 0.0625;
    unpacked_data.lpb1_voltage = round(unpacked_data.lpb1_voltage * 100.0) / 100.0;
    
    unpacked_data.lpb1_current = (packet.lpb1_current >> 4)
                           + (1 - 2 * (packet.lpb1_current < 0)) * (packet.lpb1_current & 0x0F) * 0.0625;
    unpacked_data.lpb1_current = round(unpacked_data.lpb1_current * 100.0) / 100.0;
    
    unpacked_data.lpb2_voltage = (packet.lpb2_voltage >> 4)
                           + (packet.lpb2_voltage & 0x0F) * 0.0625;
    unpacked_data.lpb2_voltage = round(unpacked_data.lpb2_voltage * 100.0) / 100.0;
    
    unpacked_data.lpb2_current = (packet.lpb2_current >> 4)
                           + (1 - 2 * (packet.lpb2_current < 0)) * (packet.lpb2_current & 0x0F) * 0.0625;
    unpacked_data.lpb2_current = round(unpacked_data.lpb2_current * 100.0) / 100.0;
    
    unpacked_data.vout_5v_voltage = (packet.vout_5v_voltage >> 4)
                           + (packet.vout_5v_voltage & 0x0F) * 0.0625;
    unpacked_data.vout_5v_voltage = round(unpacked_data.vout_5v_voltage * 100.0) / 100.0;
    
    unpacked_data.vout_5v_current = (packet.vout_5v_current >> 4)
                           + (packet.vout_5v_current & 0x0F) * 0.0625;
    unpacked_data.vout_5v_current = round(unpacked_data.vout_5v_current * 100.0) / 100.0;
    
    unpacked_data.hpb_main_voltage = (packet.hpb_main_voltage >> 3)
                           + (packet.hpb_main_voltage & 0x07) * 0.125;
    unpacked_data.hpb_main_voltage = round(unpacked_data.hpb_main_voltage * 10.0) / 10.0;
    
    unpacked_data.hpb_main_current = (packet.hpb_main_current >> 3)
                           + (1 - 2 * (packet.hpb_main_current < 0)) * (packet.hpb_main_current & 0x07) * 0.125;
    unpacked_data.hpb_main_current = round(unpacked_data.hpb_main_current * 10.0) / 10.0;
    
    unpacked_data.hpb_backup_voltage = (packet.hpb_backup_voltage >> 3)
                           + (packet.hpb_backup_voltage & 0x07) * 0.125;
    unpacked_data.hpb_backup_voltage = round(unpacked_data.hpb_backup_voltage * 10.0) / 10.0;
    
    unpacked_data.hpb_backup_current = (packet.hpb_backup_current >> 3)
                           + (1 - 2 * (packet.hpb_backup_current < 0)) * (packet.hpb_backup_current & 0x07) * 0.125;
    unpacked_data.hpb_backup_current = round(unpacked_data.hpb_backup_current * 10.0) / 10.0;
    
    unpacked_data.vout_24v_voltage = (packet.vout_24v_voltage >> 3)
                           + (packet.vout_24v_voltage & 0x07) * 0.125;
    unpacked_data.vout_24v_voltage = round(unpacked_data.vout_24v_voltage * 10.0) / 10.0;
    
    unpacked_data.vout_24v_current = (packet.vout_24v_current >> 3)
                           + (packet.vout_24v_current & 0x07) * 0.125;
    unpacked_data.vout_24v_current = round(unpacked_data.vout_24v_current * 10.0) / 10.0;

    unpacked_data.av_fc_temp = packet.av_fc_temp * 2;
    
    unpacked_data.ambient_temp = packet.ambient_temp * 2;

    unpacked_data.av_state = packet.av_state;

    unpacked_data.cam_rec = packet.cam_rec;

    unpacked_data.rail_cable_status = packet.rail_cable_status;

    unpacked_data.pyro_status = packet.pyro_status;

    unpacked_data.average_imu_rate = 100 * packet.average_imu_rate;

    if (packet.sd_fail_count_dt_log2 == 0) {
        compression_decoder_internal_state.accumulated_sd_fail_count = 0;
    } else {
        compression_decoder_internal_state.accumulated_sd_fail_count
         += (((uint64_t) 1) << (packet.sd_fail_count_dt_log2 - 1)) >> 1;
    }

    unpacked_data.sd_fail_count = compression_decoder_internal_state.accumulated_sd_fail_count;

    unpacked_data.remaining_disk_size = ((uint64_t) packet.remaining_disk_size) << ((uint64_t) 25);

    return unpacked_data;
}

#endif /* DOWNLINK_COMPRESSION_FIREHORN_2_H */

