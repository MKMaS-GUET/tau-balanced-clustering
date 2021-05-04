#include <iostream>
#include <ctime>
#include <vector>
#include <sstream>
#include <fstream>
#include "t_balanced_clustering.h"
#include <map>
#include <chrono>

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

// TEST:
// ./build/Release/tbc ./data/A/A.csv 35 348684 ./A.csv

/*
int main() {
    unsigned int seed = 666;
//    std::string filepath = "../data/A/A.csv";
//    int cluster = 35;
    std::unordered_map<std::string, int> datasets = {
            std::make_pair("../data/Wine/Wine.csv", 3),
            std::make_pair("../data/Ionosphere/Ionosphere.csv", 2),
            std::make_pair("../data/Cryotherapy/Cryotherapy.csv", 2),
            std::make_pair("../data/UserModel/UserModel.csv", 4),
            std::make_pair("../data/Statlog/Statlog.csv", 4),
            std::make_pair("../data/A/A.csv", 35),
            std::make_pair("../data/S/S.csv", 15),
            std::make_pair("../data/Dim/Dim.csv", 16),
            std::make_pair("../data/G/G.csv", 2),
            std::make_pair("../data/Unbalance/Unbalance.csv", 8),
    };

    for (const auto &item : datasets) {
        auto data = readCSV(item.first);
        TBC* tbc = new TBC(data, item.second, seed);

        time_t start = clock();
        tbc->start();
        time_t finish = clock();


        std::cout << filepath <<  " , used time: " << std::difftime(finish, start) / CLOCKS_PER_SEC << std::endl;
    }

    return 0;
}
*/
