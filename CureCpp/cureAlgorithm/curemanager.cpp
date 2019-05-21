#include "curemanager.h"

CureManager::CureManager(string fileName, int numberClusters, int numberPartitions, double shiftFactor) :
    clusterData(numberPartitions, fileName)
{
    this->numClusters = std::max(2, numberClusters);
    if (0.0 < shiftFactor && shiftFactor < 1.0)
    {
        CureClusterModel::shiftFactor = shiftFactor;
    }
    this->clusterData.clusterData();
    this->bClustered = false;
}

void CureManager::cluster()
{
    unsigned long kPartition = static_cast<unsigned long>(this->numClusters);
    vector<CureAlgorithm> clusterPartition;
    for (Mat<double> points : this->clusterData.dataPartitions)
    {
        CureAlgorithm cureAlg(points, kPartition);
        cureAlg.clusterData();
        clusterPartition.push_back(cureAlg);
    }
    cout << "Terminan clusters de particiones \n";
    map<unsigned long long, CureClusterModel *> data (clusterPartition.at(0).mapData);
    for (unsigned long i = 1; i < clusterPartition.size(); i++)
    {
        for (std::pair<unsigned long long, CureClusterModel *> clust : clusterPartition.at(i).mapData)
        {
            data.insert(clust);
        }
    }
    CureAlgorithm cureAlgor(data, static_cast<unsigned long>(this->numClusters));
    cureAlgor.clusterData();
    this->clustersMap = cureAlgor.mapData;
    cout << "Terminan clusters total \n";
}

void CureManager::saveResult(string fileName)
{
    cluster();
    ofstream file;
    file.open(fileName);
    if (file.is_open())
    {
        string headers = "";
        for (string header : this->clusterData.reader.headers)
        {
            headers += header + this->clusterData.reader.delimiter;
        }
        file << headers + "cluster\n";
        for(arma::uword i = 0; i < this->clusterData.reader.data.n_rows; i++)
        {
            rowvec rowVector = this->clusterData.reader.data.row(i);
            double minDistance = numeric_limits<double>::max();
            unsigned long index = 0, m = 0;
            for(std::pair<unsigned long long, CureClusterModel *> clusterPair : this->clustersMap)
            {
                double m_distance = clusterPair.second->calculatePointDistance(rowVector);
                if (m_distance < minDistance)
                {
                    index = m;
                    minDistance = m_distance;
                }
                m++;
            }
            for (double valueField : rowVector)
            {
                file << valueField << this->clusterData.reader.delimiter;
            }
            file << index << "\n";
        }
        file.close();
    }
    else
    {
        cout << "Error opening file" << fileName << "\n";
    }
}

