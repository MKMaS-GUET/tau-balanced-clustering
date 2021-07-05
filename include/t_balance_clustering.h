//
// Created by InnoFang on 2021/4/7.
//

#ifndef TBC__T_BALANCED_CLUSTERING_H
#define TBC__T_BALANCED_CLUSTERING_H

#include <queue>
#include <string>
#include <vector>
#include <random>
#include <climits>
#include <iostream>
#include <algorithm>
#include <functional>
#include <unordered_map>

struct ClusterCenter {
    int label;
    // 簇中心数据值
    std::vector<double> value;
    // pair<数据索引，与簇中心的距离>
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::less<std::pair<double, int>>> cluster;
};

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
    int k_;                                       // k个簇中心
    size_t size_;                                 // 数据数量
    size_t dims_;                                 // 数据维度
    size_t bound_;                                // 每个簇的数据量有多少
    std::vector<int> assignments_;                // 每条数据预测的类别
    std::vector<std::vector<double>> data_;       // 原始数据
    std::vector<ClusterCenter> cluster_centers_;  // 簇中心的数据
    std::default_random_engine random_engine_;    // 随机引擎
};

#endif //TBC__T_BALANCED_CLUSTERING_H