#include <iostream>
#include <chrono>
#include "Protocol.h"

using namespace std::chrono;

int main() {
    std::cout << "DOWNLINK PACKET (DE)COMPRESSION TEST\n\n";

    av_downlink_unpacked data;

    data.packet_nbr = 384400;
    data.gnss_lon = -78.9537;
    data.gnss_alt = 150.6504;
    data.N2_pressure = 200.3847;
    data.fuel_level = 10.4536;
    data.N2_temp = -100.3245;

    av_downlink_t packet(encode_downlink(data));
    av_downlink_unpacked result(decode_downlink(packet));

    std::cout << "\t\tInitial\t\tCompressed\tDecompressed\tDelta (%)\n"
              << "packet_nbr:\t" << data.packet_nbr << "\t\t" << packet.packet_nbr << "\t\t"
              << result.packet_nbr << "\t\t" << (result.packet_nbr - data.packet_nbr) / data.packet_nbr * 100 << "\n"
              << "gnss_lon:\t" << data.gnss_lon << "\t" << packet.gnss_lon << "\t\t"
              << result.gnss_lon << "\t\t" << (result.gnss_lon - data.gnss_lon) / data.gnss_lon * 100 << "\n"
              << "gnss_alt:\t" << data.gnss_alt << "\t\t" << packet.gnss_alt << "\t\t"
              << result.gnss_alt << "\t\t" << (result.gnss_alt - data.gnss_alt) / data.gnss_alt * 100 << "\n"
              << "N2_pressure:\t" << data.N2_pressure << "\t\t" << packet.N2_pressure
              << "\t\t" << result.N2_pressure << "\t\t" << (result.N2_pressure - data.N2_pressure) / data.N2_pressure * 100 << "\n"
              << "fuel_level:\t" << data.fuel_level << "\t\t" << packet.fuel_level << "\t\t"
              << result.fuel_level << "\t\t" << (result.fuel_level - data.fuel_level) / data.fuel_level * 100 << "\n"
              << "N2_temp:\t" << data.N2_temp << "\t\t" << packet.N2_temp << "\t\t"
              << result.N2_temp << "\t\t" << (result.N2_temp - data.N2_temp) / data.N2_temp * 100 << "\n\n";

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
        av_downlink_unpacked result(decode_downlink(packet));
        end = steady_clock::now();

        decoding_time += duration_cast<microseconds>(end - start).count();
    }

    encoding_time /= iterations;
    decoding_time /= iterations;

    std::cout << "Encoding time: " << encoding_time << " us\n"
              << "Decoding time: " << decoding_time << " us\n";
}
