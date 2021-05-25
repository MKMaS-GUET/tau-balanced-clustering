#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
#include <chrono>
#include "t_balanced_clustering.h"

static const auto io_speed_up = [] {
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
    // tbc.exe <input> <cluster> <seed> <output>
    if (argc < 5) {
        std::cerr << "./tbc <input> <cluster> <seed> <output>" << std::endl;
        exit(0);
    }
    std::string filepath = argv[1];
    int cluster = atoi(argv[2]);
    unsigned int seed = atoi(argv[3]);
    std::string outputPath = argv[4];

    auto data = readCSV(filepath);
    TBC *tbc = new TBC(data, cluster, seed);

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
