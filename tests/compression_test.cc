#include <iostream>
#include <chrono>
#include "Protocol.h"

using namespace std::chrono;

int main() {
    std::cout << "DOWNLINK PACKET (DE)COMPRESSION TEST\n\n";

    av_downlink_unpacked_t data;

    data.packet_nbr = 384461;
    data.gnss_lon = -9.138279;
    data.gnss_lat = 38.713138;
    data.gnss_alt = 2394;
    data.N2_pressure = 322;
    data.N2_temp = 45;
    data.fuel_pressure = 59;
    data.LOX_temp = -197;
    data.engine_state = 0b11001101;
    data.lpb_voltage = 3.8756;
    data.lpb_current = 2.83;
    data.hpb_voltage = 25.08;
    data.hpb_current = 20.33;

    av_downlink_t packet(encode_downlink(data));
    av_downlink_unpacked_t result(decode_downlink(packet));

    std::cout << "\t\tInitial\t\tCompressed\tDecompressed\tDelta (%)\n"
              << "packet_nbr:\t" << data.packet_nbr << "\t\t" << packet.packet_nbr << "\t\t"
              << result.packet_nbr << "\t\t" << (result.packet_nbr - data.packet_nbr) / data.packet_nbr * 100 << "\n"

              << "gnss_lon:\t" << data.gnss_lon << "\t" << packet.gnss_lon << "\t\t"
              << result.gnss_lon << "\t" << (result.gnss_lon - data.gnss_lon) / (float)data.gnss_lon * 100 << "\n"

              << "gnss_lat:\t" << data.gnss_lat << "\t\t" << packet.gnss_lat << "\t\t"
              << result.gnss_lat << "\t\t" << (result.gnss_lat - data.gnss_lat) / (float)data.gnss_lat * 100 << "\n"

              << "gnss_alt:\t" << data.gnss_alt << "\t\t" << packet.gnss_alt << "\t\t"
              << result.gnss_alt << "\t\t" << (result.gnss_alt - data.gnss_alt) / (float)data.gnss_alt * 100 << "\n"

              << "N2_pressure:\t" << data.N2_pressure << "\t\t" << packet.N2_pressure << "\t\t"
              << result.N2_pressure << "\t\t" << (result.N2_pressure - data.N2_pressure) / (float)data.N2_pressure * 100 << "\n"

              << "N2_temp:\t" << (int)data.N2_temp << "\t\t" << packet.N2_temp << "\t\t"
              << (int)result.N2_temp << "\t\t" << (result.N2_temp - data.N2_temp) / (float)data.N2_temp * 100 << "\n"

              << "fuel_pressure:\t" << (int)data.fuel_pressure << "\t\t" << packet.fuel_pressure << "\t\t"
              << (int)result.fuel_pressure << "\t\t" << (result.fuel_pressure - data.fuel_pressure) / (float)data.fuel_pressure * 100 << "\n"

              << "LOX_temp:\t" << data.LOX_temp << "\t\t" << packet.LOX_temp << "\t\t"
              << result.LOX_temp << "\t\t" << (result.LOX_temp - data.LOX_temp) / (float)data.LOX_temp * 100 << "\n"

              << "engine_state:\t" << data.engine_state << "\t\t" << packet.engine_state << "\t\t"
              << result.engine_state << "\t\t" << (result.engine_state - data.engine_state) / (float)data.engine_state * 100 << "\n"

              << "lpb_voltage:\t" << data.lpb_voltage << "\t\t" << packet.lpb_voltage << "\t\t"
              << result.lpb_voltage << "\t\t" << (result.lpb_voltage - data.lpb_voltage) / (float)data.lpb_voltage * 100 << "\n"

              << "lpb_current:\t" << data.lpb_current << "\t\t" << packet.lpb_current << "\t\t"
              << result.lpb_current << "\t\t" << (result.lpb_current - data.lpb_current) / (float)data.lpb_current * 100 << "\n"

              << "hpb_voltage:\t" << data.hpb_voltage << "\t\t" << packet.hpb_voltage << "\t\t"
              << result.hpb_voltage << "\t\t" << (result.hpb_voltage - data.hpb_voltage) / (float)data.lpb_voltage * 100 << "\n"

              << "hpb_current:\t" << data.hpb_current << "\t\t" << packet.hpb_current << "\t\t"
              << result.hpb_current << "\t\t" << (result.hpb_current - data.hpb_current) / (float)data.hpb_current * 100 << "\n";

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
