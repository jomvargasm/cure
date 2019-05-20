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
    vector<double> meanDistancesVector;
    unsigned long numberComparedDistances = 4;
    double maxDistanceStepAllowedFactor = 5.0;
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
        if (meanDistancesVector.size() >= numberComparedDistances)
        {
            double meanDistancesValue = std::accumulate(meanDistancesVector.begin(), meanDistancesVector.end(), 0.0) / static_cast<double>(meanDistancesVector.size());
            if (minDistance > (maxDistanceStepAllowedFactor * meanDistancesValue))
            {
                break;
            }
        }
        meanDistancesVector.push_back(minDistance);
        CureClusterModel * newCluster = CureClusterModel::mergeClusters(this->mapData.at(index), this->mapData.at(indexNearest));
        for (ClusterDistance_t nearCluster : this->mapData.at(index)->nearestClusters)
        {
            nearCluster.cluster->reCalculateClusterDistances(index, newCluster);
        }
        for (ClusterDistance_t nearCluster : this->mapData.at(indexNearest)->nearestClusters)
        {
            nearCluster.cluster->reCalculateClusterDistances(indexNearest, newCluster);
        }
        deleteIndexPointerFromMapData(indexNearest);
        deleteIndexPointerFromMapData(index);
        this->mapData.insert(std::pair<unsigned long long, CureClusterModel *>(newCluster->id, newCluster));
        count++;
    }
    if (this->mapData.size() > this->numberClusters)
    {
        vector<vector<unsigned long long>> indexPointSizes;
        for (std::pair<unsigned long long, CureClusterModel *> clustPair : this->mapData)
        {
            vector<unsigned long long> m_size({clustPair.first, clustPair.second->points.size()});
            indexPointSizes.push_back(m_size);
        }
        std::sort(indexPointSizes.begin(), indexPointSizes.end(),
                  [](const std::vector<unsigned long long>& a, const std::vector<unsigned long long>& b) {
                    return a[2] > b[2];
                  });
        for (unsigned long i = this->numberClusters; i < indexPointSizes.size(); i++)
        {
            deleteIndexPointerFromMapData(indexPointSizes.at(i).at(0));
        }
    }
}

void CureAlgorithm::deleteIndexPointerFromMapData(unsigned long long index)
{
    if (this->mapData.count(index))
    {
        delete this->mapData.at(index);
        this->mapData.at(index) = nullptr;
        this->mapData.erase(index);
    }
}
