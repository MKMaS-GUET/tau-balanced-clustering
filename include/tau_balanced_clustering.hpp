#ifndef TBC__TAU_BALANCED_CLUSTERING_H
#define TBC__TAU_BALANCED_CLUSTERING_H

#include <queue>
#include <string>
#include <vector>
#include <random>
#include <climits>
#include <iostream>
#include <algorithm>
#include <functional>
#include <unordered_map>

namespace tbc {
    using pdi = std::pair<double, int>;

    struct ClusterCenter {
        int label;
        // the value of cluster center
        std::vector<double> value;
        // pair<data index, the distance between data and cluster center>
        // Use max-heap to store the pairs can easily get the boundary data of this cluster
        std::priority_queue<pdi, std::vector<pdi>, std::less<pdi>> cluster;
    };
}

class TBC {
public:
    TBC(const std::vector<std::vector<double>> &data, int k, int tau, unsigned int seed = std::random_device()());
    ~TBC();
    double start();
    void initClusterCenters();
    void updateClusterCenters();
    void reassign();
    void assignToNearestCenter(int dataIndex, std::vector<double> &center);
    double distance(const std::vector<double> &data1, const std::vector<double> &data2) const;
    std::vector<int> getAssignments() const;

private:
    int k_;                                             // k clusters
    size_t size_;                                       // data size
    size_t dims_;                                       // data attribution dimension
    size_t bound_;                                      // the max bound of cluster
    std::vector<int> assignments_;                      // the cluster assignment of all data
    std::vector<std::vector<double>> data_;             // raw data
    std::vector<tbc::ClusterCenter> cluster_centers_;   // store the k cluster center
    std::default_random_engine random_engine_;          // for shuffle raw data
};

#endif //TBC__TAU_BALANCED_CLUSTERING_H
