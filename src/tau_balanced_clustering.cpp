#include "tau_balanced_clustering.hpp"

TBC::TBC(const std::vector<std::vector<double>> &data, int k, int tau, unsigned int seed)
        : data_(data),
          k_(k),
          size_(data.size()),
          dims_(data.front().size()),
          bound_((size_ - tau) / k_ + tau),
          random_engine_(seed) {}

TBC::~TBC() = default;

double TBC::start() {
    initClusterCenters();
    for (int i = 1;; ++ i) {
        std::vector<int> oldAssignment(assignments_);
        reassign();
        if (oldAssignment == assignments_ || i > 20) {
            break;
        }
        updateClusterCenters();
    }
    return 0;
}

void TBC::initClusterCenters() {
    std::vector<size_t> indices(size_);
    for (size_t i = 0; i < size_; ++i) {
        indices[i] = i;
    }
    std::shuffle(indices.begin(), indices.end(), random_engine_);

    cluster_centers_.resize(k_);
    for (int i = 0; i < k_; ++i) {
        cluster_centers_[i].label = i;
        auto ith = data_[indices[i]];
        cluster_centers_[i].value.resize(dims_);
        std::copy(ith.begin(), ith.end(), cluster_centers_[i].value.begin());
    }
}

void TBC::reassign() {
    assignments_.assign(size_, 0);
    for (int i = 0; i < size_; ++i) {
        assignToNearestCenter(i, data_[i]);
    }
}

void TBC::updateClusterCenters() {
    for (auto& cluster_center: cluster_centers_) {
        // 1. get the data size in the cluster
        size_t clusterSize = cluster_center.cluster.size();
        // 2. sum the values of each dimension in the cluster
        std::vector<double> newClusterValue(dims_);
        while (!cluster_center.cluster.empty()) {
            tbc::pdi distIdxPair = cluster_center.cluster.top();
            std::vector<double> dataValue = data_[distIdxPair.second];
            for (int i = 0; i < dims_; ++i) {
                newClusterValue[i] += dataValue[i];
            }
            cluster_center.cluster.pop();
        }
        // 3. divide by the length of the data and update the value of the cluster
        if (clusterSize > 0) { // check the size of the cluster is essential, or '-nan(ind)' will be occurred
            for (int i = 0; i < dims_; ++i) {
                cluster_center.value[i] = newClusterValue[i] / double(clusterSize);
            }
        }
    }
}

void TBC::assignToNearestCenter(int dataIndex, std::vector<double> &dataItem) {
    // Use min-heap to store pairs ( <distance from the cluster center, data> )
    // can easily get the nearest cluster center for the current data
    std::priority_queue<tbc::pdi, std::vector<tbc::pdi>, std::greater<tbc::pdi>> nearestClusters;

    for (int i = 0; i < cluster_centers_.size(); ++ i) {
        // Calculate the distance between data and cluster center
        double dist = distance(cluster_centers_[i].value, dataItem);
        // Push the pair <distance from cluster center, data> into min-heap
        nearestClusters.push( {dist, i} );
    }
    while (!nearestClusters.empty()) {
        double dist = nearestClusters.top().first;
        int nearestIdx = nearestClusters.top().second;
        tbc::ClusterCenter &nearest = cluster_centers_[nearestIdx];
        // If the nearest cluster is not full, the data is added to the cluster
        if (nearest.cluster.size() <= bound_) {
            nearest.cluster.push(std::make_pair(dist, dataIndex));
            assignments_[dataIndex] = nearest.label;
            return;
        } else {
            // If the number of data in the cluster reaches the upper limit of the boundary,
            // compare the distance between the boundary data and the center
            // and the distance between the current data and the center

            // Firstly, get boundary data's index and distance from the cluster center
            tbc::pdi boundary = nearest.cluster.top();

            // If the boundary data is farther than the current data
            if (boundary.first > dist) {
                // kick out the boundary data from the cluster
                nearest.cluster.pop();
                // and add the current data
                nearest.cluster.push(std::make_pair(dist, dataIndex));
                assignments_[dataIndex] = nearest.label;
                assignToNearestCenter(boundary.second, data_[boundary.second]);
                return;
            }
        }
        nearestClusters.pop();
    }
}

double TBC::distance(const std::vector<double> &data1, const std::vector<double> &data2) const {
    double result = 0.0;
    for (int i = 0; i < dims_; ++i) {
        result += (data1[i] - data2[i]) * (data1[i] - data2[i]);
    }
    return result;
}

std::vector<int> TBC::getAssignments() const {
    return assignments_;
}
