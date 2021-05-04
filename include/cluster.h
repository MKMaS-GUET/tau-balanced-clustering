//
// Created by InnoFang on 2021/4/22.
//

#ifndef T_BALANCE_CLUSTERING_CLUSTER_H
#define T_BALANCE_CLUSTERING_CLUSTER_H


struct MaxHeap {
    bool operator()(const std::pair<int, double> &a, const std::pair<int, double> &b) {
        return a.second == b.second ? a.first < b.first : a.second < b.second;
    }
};

struct ClusterCenter {
//public:
//    ClusterCenter(size_t dims)
//            : value(dims),  {}
//
//private:
    int label;
    // 簇中心数据值
    std::vector<double> value;
    // pair<数据索引，与簇中心的距离>
    std::priority_queue<std::pair<int, double>, std::vector<std::pair<int, double>>, MaxHeap> cluster;
//    bool operator==(const ClusterCenter& other) const {
//        return value == other.value && label == other.label;
//    }
//    ClusterCenter(const ClusterCenter& obj) {
//        label = obj.label;
//        value = obj.value;
//        cluster = obj.cluster;
//    }
//    ClusterCenter(): value(), cluster() {};
//    ClusterCenter(size_t dims) : value(dims) {
//        cluster = {};
//    };
};

#endif //T_BALANCE_CLUSTERING_CLUSTER_H
