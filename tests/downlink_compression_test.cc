#include <iostream>
#include <chrono>
#include "Protocol.h"

using namespace std::chrono;

int main() {
    std::cout << "FIREHORN 2 AV DOWNLINK PACKET (DE)COMPRESSION TEST\n\n";

    av_downlink_unpacked_t data;

    data.packet_nbr = 384461;
    data.gnss_lon = -9.138279;
    data.gnss_lat = 38.713138;
    data.gnss_alt = 2394.633520;
    data.absolute_speed = 111.111;
    data.N2_pressure_1 = 322.347;
    data.N2_temp_1 = 45.981;
    data.fuel_pressure = 59.19;
    data.LOX_temp = -197.12;
#if defined FLS_CONFIG && FLS_CONFIG == FLS_CAPA
    data.LOX_fls_capa = 994.7345;
#elif defined FLS_CONFIG && FLS_CONFIG == FLS_DIFF
    data.fuel_fls_diff = 65.27
#elif FLS_CONFIG == FLS_TEMP
    data.LOX_fls_temp_1 = -159.74;
#endif
    data.valves_state = 0b01001101;
#if defined DPR_CONFIG && DPR_CONFIG ==  DPR_BALL_VALVE
    data.valves_state &= ~(1 << 6);
    data.valves_state |= AV_VALVE_SDPR_LOX;
    data.valves_state &= ~(1 << 5);
    data.valves_state |= AV_VALVE_SDPR_FUEL;
    data.valve_dpr_fuel = 80.1;
#endif
    data.lpb_voltage = 3.74;
    data.lpb_current = -1.17;
    data.hpb_main_voltage = 25.2;
    data.hpb_main_current = -4.33;
    data.hpb_backup_current = 17.33;
    data.pyro_status = AV_PYRO_CH1 | AV_PYRO_CH4;

    av_downlink_t packet(encode_downlink(data));
    av_downlink_unpacked_t result(decode_downlink(packet));

    std::cout << "\t\t\tInitial\t\tCompressed\tDecompressed\tDelta (%)\n"
              << "packet_nbr:\t\t" << data.packet_nbr << "\t\t" << packet.packet_nbr << "\t\t"
              << result.packet_nbr << "\t\t" << (result.packet_nbr - data.packet_nbr) / data.packet_nbr * 100 << "\n"

              << "gnss_lon:\t\t" << data.gnss_lon << "\t" << packet.gnss_lon << "\t\t"
              << result.gnss_lon << "\t" << (result.gnss_lon - data.gnss_lon) / (float)data.gnss_lon * 100 << "\n"

              << "gnss_lat:\t\t" << data.gnss_lat << "\t\t" << packet.gnss_lat << "\t\t"
              << result.gnss_lat << "\t\t" << (result.gnss_lat - data.gnss_lat) / (float)data.gnss_lat * 100 << "\n"

              << "gnss_alt:\t\t" << data.gnss_alt << "\t\t" << packet.gnss_alt << "\t\t"
              << result.gnss_alt << "\t\t" << (result.gnss_alt - data.gnss_alt) / (float)data.gnss_alt * 100 << "\n"
              
              << "absolute_speed:\t\t" << data.absolute_speed << "\t\t" << packet.absolute_speed << "\t\t"
              << result.absolute_speed << "\t\t" << (result.absolute_speed - data.absolute_speed) / (float)data.absolute_speed * 100 << "\n"

              << "N2_pressure_1:\t\t" << data.N2_pressure_1 << "\t\t" << (int)packet.N2_pressure_1 << "\t\t"
              << result.N2_pressure_1 << "\t\t" << (result.N2_pressure_1 - data.N2_pressure_1) / (float)data.N2_pressure_1 * 100 << "\n"

              << "N2_temp_1:\t\t" << data.N2_temp_1 << "\t\t" << packet.N2_temp_1 << "\t\t"
              << result.N2_temp_1 << "\t\t" << (result.N2_temp_1 - data.N2_temp_1) / (float)data.N2_temp_1 * 100 << "\n"

              << "fuel_pressure:\t\t" << data.fuel_pressure << "\t\t" << packet.fuel_pressure << "\t\t"
              << result.fuel_pressure << "\t\t" << (result.fuel_pressure - data.fuel_pressure) / (float)data.fuel_pressure * 100 << "\n"

              << "LOX_temp:\t\t" << data.LOX_temp << "\t\t" << packet.LOX_temp << "\t\t"
              << result.LOX_temp << "\t\t" << (result.LOX_temp - data.LOX_temp) / (float)data.LOX_temp * 100 << "\n"

#if defined FLS_CONFIG && FLS_CONFIG == FLS_CAPA
              << "LOX_fls_capa:\t\t" << data.LOX_fls_capa << "\t\t" << packet.LOX_fls_capa << "\t\t"
              << result.LOX_fls_capa << "\t\t" << (result.LOX_fls_capa - data.LOX_fls_capa) / (float)data.LOX_fls_capa * 100 << "\n"
#elif defined FLS_CONFIG && FLS_CONFIG == FLS_DIFF
              << "fuel_fls_diff:\t\t" << data.fuel_fls_diff << "\t\t" << packet.fuel_fls_diff << "\t\t"
              << result.fuel_fls_diff << "\t\t" << (result.fuel_fls_diff - data.fuel_fls_diff) / (float)data.fuel_fls_diff * 100 << "\n"
#elif FLS_CONFIG == FLS_TEMP
              << "LOX_fls_temp_1:\t\t" << data.LOX_fls_temp_1 << "\t\t" << packet.LOX_fls_temp_1 << "\t\t"
              << result.LOX_fls_temp_1 << "\t\t" << (result.LOX_fls_temp_1 - data.LOX_fls_temp_1) / (float)data.LOX_fls_temp_1 * 100 << "\n"
#endif
              
              << "valves_state:\t\t" << (int)data.valves_state << "\t\t" << (int)packet.valves_state << "\t\t"
              << (int)result.valves_state << "\t\t" << (result.valves_state - data.valves_state) / (float)data.valves_state * 100 << "\n"

#if defined DPR_CONFIG && DPR_CONFIG ==  DPR_BALL_VALVE
              << "valve_dpr_fuel:\t\t" << data.valve_dpr_fuel << "\t\t" << (int)packet.valve_dpr_fuel << "\t\t"
              << (int)result.valve_dpr_fuel << "\t\t" << (result.valve_dpr_fuel - data.valve_dpr_fuel) / (float)data.valve_dpr_fuel * 100 << "\n"
#endif
              
              << "lpb_voltage:\t\t" << data.lpb_voltage << "\t\t" << (int)packet.lpb_voltage << "\t\t"
              << result.lpb_voltage << "\t\t" << (result.lpb_voltage - data.lpb_voltage) / (float)data.lpb_voltage * 100 << "\n"

              << "lpb_current:\t\t" << data.lpb_current << "\t\t" << (int)packet.lpb_current << "\t\t"
              << result.lpb_current << "\t\t" << (result.lpb_current - data.lpb_current) / (float)data.lpb_current * 100 << "\n"

              << "hpb_main_voltage:\t" << data.hpb_main_voltage << "\t\t" << (int)packet.hpb_main_voltage << "\t\t"
              << result.hpb_main_voltage << "\t\t" << (result.hpb_main_voltage - data.hpb_main_voltage) / (float)data.hpb_main_voltage * 100 << "\n"

              << "hpb_main_current:\t" << data.hpb_main_current << "\t\t" << (int)packet.hpb_main_current << "\t\t"
              << result.hpb_main_current << "\t\t" << (result.hpb_main_current - data.hpb_main_current) / (float)data.hpb_main_current * 100 << "\n"
              
              << "hpb_backup_current:\t" << data.hpb_backup_current << "\t\t" << (int)packet.hpb_backup_current << "\t\t"
              << result.hpb_backup_current << "\t\t" << (result.hpb_backup_current - data.hpb_backup_current) / (float)data.hpb_backup_current * 100 << "\n"

              << "pyro_status:\t\t" << (int)data.pyro_status << "\t\t" << (int)packet.pyro_status << "\t\t"
              << (int)result.pyro_status << "\t\t" << (result.pyro_status - data.pyro_status) / (float)data.pyro_status * 100 << "\n";

    const unsigned initial_size(sizeof(data));
    const unsigned compressed_size(sizeof(packet));
    std::cout << "\nCompression ratio (" << initial_size << "/" << compressed_size << "): "
              << (initial_size -compressed_size) / (double)initial_size * 100 << "%\n\n";

    double encoding_time(0);
    double decoding_time(0);

    const unsigned iterations(1e6);
    std::cout << "Profiling over " << iterations << " iterations:\n";
    for (unsigned i(0); i < iterations; ++i) {
        auto start(steady_clock::now());
        av_downlink_t packet(encode_downlink(data));
        auto end(steady_clock::now());

        encoding_time += duration_cast<microseconds>(end - start).count();

        start = steady_clock::now();
        av_downlink_unpacked_t result(decode_downlink(packet));
        end = steady_clock::now();

        decoding_time += duration_cast<microseconds>(end - start).count();
    }

    encoding_time /= iterations;
    decoding_time /= iterations;

    std::cout << "Encoding time: " << encoding_time << " us\n"
              << "Decoding time: " << decoding_time << " us\n";
}
