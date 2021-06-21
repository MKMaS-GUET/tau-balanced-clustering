#include "t_balanced_clustering.h"

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
    for (int i = 1;; ++i) {
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
    std::vector<int> indices(size_);
    for (size_t i = 0; i < size_; ++i) {
        indices[i] = i;
    }
    std::shuffle(indices.begin(), indices.end(), std::mt19937(random_engine_));

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
    // 1. 先取得 ClusterCenter.clusters 的长度
    // 2. 再对 ClusterCenter.clusters 每个维度的值求和
    // 3. 再除以数据长度并更新 ClusterCenter.value
    for (auto& cluster_center: cluster_centers_) {
        // 1.
        size_t clusterSize = cluster_center.cluster.size();
        // 2.
        std::vector<double> newClusterValue(dims_);
        while (!cluster_center.cluster.empty()) {
            std::pair<double, int> distIdxPair = cluster_center.cluster.top();
            std::vector<double> dataValue = data_[distIdxPair.second];
            for (int i = 0; i < dims_; ++i) {
                newClusterValue[i] += dataValue[i];
            }
            cluster_center.cluster.pop();
        }
        // 3.
        if (clusterSize > 0) { // !!! 一定要判断，否则会出现 -nan(ind)
            for (int i = 0; i < dims_; ++i) {
                cluster_center.value[i] = newClusterValue[i] / double(clusterSize);
            }
        }
    }
}

void TBC::assignToNearestCenter(int dataIndex, std::vector<double> &dataItem) {

    /**
     * 每个簇中心维护一个最大堆，堆顶元素即簇边界
     * 计算每个簇与自己中心点的距离，使用
     * priority_queue<pair<数据索引, 与簇中心的距离>>
     *
     * 计算每个数据点计算与每个簇中心的距离，使用最小堆存储数据点对 <数据索引, 与簇中心的距离>
     * 然后从小到大判断数据点是否可以加入簇（条件为簇未满）
     * 如果簇已经达到节点个数上限，则并与簇的边界比较，如过比边界小则提出原来的边界，加入当前节点
     */

//    std::priority_queue<std::pair<double, ClusterCenter>, std::vector<std::pair<double, ClusterCenter>>, MinHeap> nearestClusters;
    // 每次使用 make_pair 好像都会拷贝 CLusterCenter，但是里面的 vector 和 priority_queue 无法直接拷贝，
    // 特别因为 priority_queue 的特性，拷贝可能会需要清空原对象的 cluster，而这里存储 ClusterCenter 的指针也不可以，
    // 所以考虑将存储格式更改为 <数据与簇中心的距离，ClusterCenter 的索引 id>
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> nearestClusters;

    for (int i = 0; i < cluster_centers_.size(); ++ i) {
        // 计算数据点与簇中心的距离
        double dist = distance(cluster_centers_[i].value, dataItem);
        // 将数据点与簇中心构成的 <与簇中心距离, 簇中心> 对加入最小堆
//        std::pair<double, ClusterCenter> dist2cluster(dist, clusterCenter);
//        nearestClusters.push( dist2cluster );
        nearestClusters.push(std::make_pair(dist, i));
    }
    // 检查 nearestClusters 是不是空的 (通过实现 ClusterCenter 的拷贝构造函数解决)
    // 为什么 push 过后，cluster 仍然是空的
    while (!nearestClusters.empty()) {
        // 每次取得最小的点
        double dist = nearestClusters.top().first;
        int nearestIdx = nearestClusters.top().second;
        ClusterCenter &nearest = cluster_centers_[nearestIdx];
//        std::cout << "cluster size: " << nearest.cluster.size() << std::endl;
        // 如果距离最近的簇还未满，则将该点加入到该簇中
        if (nearest.cluster.size() <= bound_) {
            nearest.cluster.push(std::make_pair(dist, dataIndex));
            assignments_[dataIndex] = nearest.label;
            return;
//            return std::vector<double>();
        } else {
            // 如果中心点所包含的节点个数达到上限
            // 检查边界节点 和 当前节点距离中心点的距离

            // 取得边界点的索引和与簇中心的距离
            std::pair<double, int> boundary = nearest.cluster.top();

            // 如果边界点比当前点远，
            if (boundary.first > dist) {
                // 则将边界点踢出当前簇
                nearest.cluster.pop();
                // 并加入新的点
                nearest.cluster.push(std::make_pair(dist, dataIndex));
                assignments_[dataIndex] = nearest.label;
                // 返回被踢出的数据点，一遍进行
                // !! 感觉根本不用返回，
//            return data_[boundary.first];
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
