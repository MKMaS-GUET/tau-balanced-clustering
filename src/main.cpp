#include <map>
#include <vector>
#include <chrono>
#include <sstream>
#include <fstream>
#include <iostream>
#include "tau_balanced_clustering.hpp"

static const auto io_speed_up = [] {
    std::stringstream::sync_with_stdio(false);
    std::fstream::sync_with_stdio(false);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    return 0;
} ();

std::vector<std::vector<double>> readCSV(const std::string &filepath) {
    std::vector<std::vector<double>> data;
    std::ifstream reader(filepath);
    std::string line;
    while (reader >> line) {
        std::stringstream raw(line);
        std::vector<double> each;
        std::string attr;
        while (std::getline(raw, attr, ',')) {
            each.emplace_back(stod(attr));
        }
        data.emplace_back(each);
    }
    return data;
}

void writeAssignment(const std::string &filepath, const std::vector<int> &assignments) {
    std::ofstream file(filepath);
    for (auto &assignment : assignments) {
        file << assignment << '\n';
    }
}

int main(int argc, char *argv[]) {
    /* ' ./tbc <input> <output> <k> <tau> [seed] ' */
    if (argc < 5 || argc > 6) {
        std::cerr << "./tbc <input> <output> <k> <tau> [seed]" << std::endl;
        exit(0);
    }

    std::string filepath = argv[1];
    std::string outputPath = argv[2];
    int cluster = std::strtol(argv[3], nullptr, 10);
    int tau = std::strtol(argv[4], nullptr, 10);
    unsigned int seed = (argc == 6) ? std::strtol(argv[5], nullptr, 10)
                                    : std::chrono::system_clock::now().time_since_epoch().count();

    auto data = readCSV(filepath);
    if (data.empty()) {
        std::cerr << "After loading, the data is empty, please check the 'filepath' you input" << std::endl;
        exit(0);
    }

    TBC *tbc = new TBC(data, cluster, tau, seed);

    auto start_time = std::chrono::high_resolution_clock::now();

    tbc->start();

    double used_time = std::chrono::duration_cast<std::chrono::duration<double>>(
            std::chrono::high_resolution_clock::now() - start_time)
            .count();

    writeAssignment(outputPath, tbc->getAssignments());

    std::cout << filepath << " , used time: " << used_time << std::endl;

    delete tbc;
    return 0;
}
