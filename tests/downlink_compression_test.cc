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
    data.gnss_alt = 2394;
    data.absolute_speed = 350;
    data.N2_pressure = 322;
    data.N2_temp = 45;
    data.fuel_pressure = 59.451;
    data.LOX_temp = -197;
    data.solenoid_valves_state = 0b01001101;
    data.lpb_voltage = 3.8756;
    data.lpb_current = -1.2;
    data.hpb_main_voltage = 25.08;
    data.hpb_main_current = 19.33;

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

              << "N2_pressure:\t\t" << data.N2_pressure << "\t\t" << (int)packet.N2_pressure << "\t\t"
              << result.N2_pressure << "\t\t" << (result.N2_pressure - data.N2_pressure) / (float)data.N2_pressure * 100 << "\n"

              << "N2_temp:\t\t" << data.N2_temp << "\t\t" << packet.N2_temp << "\t\t"
              << result.N2_temp << "\t\t" << (result.N2_temp - data.N2_temp) / (float)data.N2_temp * 100 << "\n"

              << "fuel_pressure:\t\t" << data.fuel_pressure << "\t\t" << packet.fuel_pressure << "\t\t"
              << result.fuel_pressure << "\t\t" << (result.fuel_pressure - data.fuel_pressure) / (float)data.fuel_pressure * 100 << "\n"

              << "LOX_temp:\t\t" << data.LOX_temp << "\t\t" << packet.LOX_temp << "\t\t"
              << result.LOX_temp << "\t\t" << (result.LOX_temp - data.LOX_temp) / (float)data.LOX_temp * 100 << "\n"

              << "solenoid_valves_state:\t" << (int)data.solenoid_valves_state << "\t\t" << (int)packet.solenoid_valves_state << "\t\t"
              << (int)result.solenoid_valves_state << "\t\t" << (result.solenoid_valves_state - data.solenoid_valves_state) / (float)data.solenoid_valves_state * 100 << "\n"

              << "lpb_voltage:\t\t" << data.lpb_voltage << "\t\t" << packet.lpb_voltage << "\t\t"
              << result.lpb_voltage << "\t\t" << (result.lpb_voltage - data.lpb_voltage) / (float)data.lpb_voltage * 100 << "\n"

              << "lpb_current:\t\t" << data.lpb_current << "\t\t" << packet.lpb_current << "\t\t"
              << result.lpb_current << "\t\t" << (result.lpb_current - data.lpb_current) / (float)data.lpb_current * 100 << "\n"

              << "hpb_main_voltage:\t" << data.hpb_main_voltage << "\t\t" << packet.hpb_main_voltage << "\t\t"
              << result.hpb_main_voltage << "\t\t" << (result.hpb_main_voltage - data.hpb_main_voltage) / (float)data.hpb_main_voltage * 100 << "\n"

              << "hpb_main_current:\t" << data.hpb_main_current << "\t\t" << packet.hpb_main_current << "\t\t"
              << result.hpb_main_current << "\t\t" << (result.hpb_main_current - data.hpb_main_current) / (float)data.hpb_main_current * 100 << "\n";

    const unsigned initial_size(sizeof(data));
    const unsigned compressed_size(sizeof(packet));
    std::cout << "Compression ratio (" << initial_size << "/" << compressed_size << "): "
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
