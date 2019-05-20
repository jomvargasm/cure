#include "curealgorithm.h"

CureAlgorithm::CureAlgorithm(Mat<double> & points, unsigned long k) :
    numberClusters(k)
{
    for (arma::uword i = 0; i < points.n_rows; i++)
    {
        CureClusterModel * cluster = new CureClusterModel(points.row(i));
        this->mapData.insert(std::pair<unsigned long long, CureClusterModel *>(cluster->id, cluster));
    }
    for (std::pair<unsigned long long, CureClusterModel *> cluster : this->mapData)
    {
        cluster.second->calculateClusterDistances(this->mapData);
    }
}

CureAlgorithm::CureAlgorithm(map<unsigned long long, CureClusterModel *> & data, unsigned long k) :
    numberClusters(k), mapData(data)
{
    for (std::pair<unsigned long long, CureClusterModel *> cluster : this->mapData)
    {
        cluster.second->calculateClusterDistances(this->mapData);
    }
}

void CureAlgorithm::clusterData()
{
    long count = 0;
    long max_count = static_cast<long>(10 * this->mapData.size());
    while (this->mapData.size() > this->numberClusters && count < (max_count))
    {
        unsigned long long index = 0;
        double minDistance = numeric_limits<double>::max();
        for (std::pair<unsigned long long, CureClusterModel *> cluster : this->mapData)
        {
            if (cluster.second->getMinimumDistance() < minDistance)
            {
                minDistance = cluster.second->getMinimumDistance();
                index = cluster.first;
            }
        }
        unsigned long long indexNearest = this->mapData.at(index)->getMinimumClusterID();
        if (!this->mapData.count(indexNearest))
        {
            this->mapData.at(index)->calculateClusterDistances(this->mapData);
            continue;
        }
        CureClusterModel * newCluster = CureClusterModel::mergeClusters(this->mapData.at(index), this->mapData.at(indexNearest));
        for (ClusterDistance_t nearCluster : this->mapData.at(index)->nearestClusters)
        {
            nearCluster.cluster->reCalculateClusterDistances(index, newCluster);
        }
        for (ClusterDistance_t nearCluster : this->mapData.at(indexNearest)->nearestClusters)
        {
            nearCluster.cluster->reCalculateClusterDistances(indexNearest, newCluster);
        }
        if (this->mapData.count(indexNearest))
        {
            this->mapData.erase(indexNearest);
        }
        if (this->mapData.count(index))
        {
            this->mapData.erase(index);
        }
        this->mapData.insert(std::pair<unsigned long long, CureClusterModel *>(newCluster->id, newCluster));
        count++;
    }
}
