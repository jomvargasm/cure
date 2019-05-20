#include "cureclustermodel.h"

unsigned long long CureClusterModel::id_counter = 1;
unsigned long CureClusterModel::numberRepresentativePoints = 5;
unsigned long long CureClusterModel::numberStoredNearests = 10;
arma::uword CureClusterModel::distanceNormP = 2;
double CureClusterModel::shiftFactor = 0.1;

CureClusterModel::CureClusterModel(rowvec point) :
    id(CureClusterModel::id_counter), points({point}), representativePoints({point})
{
    CureClusterModel::id_counter++;
    // this->points.push_back(point);
    // this->representativePoints.push_back(point);
}

double CureClusterModel::calculateDistance(const CureClusterModel & cluster)
{
    double distance = numeric_limits<double>::max();
    double m_distance = 0;
    for (rowvec vectorA : this->representativePoints)
    {
        for (rowvec vectorB : cluster.representativePoints)
        {
            m_distance = norm(vectorA - vectorB, CureClusterModel::distanceNormP);
            if (m_distance < distance)
            {
                distance = m_distance;
            }
        }
    }
    return distance;
}


double CureClusterModel::calculatePointDistance(const rowvec & vectorA)
{
    double distance = numeric_limits<double>::max();
    double m_distance = 0;
    for (rowvec vectorB : this->representativePoints)
    {
        m_distance = norm(vectorA - vectorB, CureClusterModel::distanceNormP);
        if (m_distance < distance)
        {
            distance = m_distance;
        }
    }
    return distance;
}

void CureClusterModel::calculateClusterDistances(map<unsigned long long, CureClusterModel *> & clusters)
{
    if (clusters.size() == 0)
    {
        return;
    }
    vector<ClusterDistance_t> dCluster;
    for (unsigned long i = 0; i < this->id_counter; i++)
    // for (std::pair<unsigned long long, CureClusterModel> clusterPair : clusters)
    {
        if (clusters.count(i))
        {
            if (clusters.at(i)->id != this->id)
            {
                double distance = this->calculateDistance(*clusters.at(i));
                ClusterDistance_t clusterStruct = {clusters.at(i), distance};
                dCluster.push_back(clusterStruct);
            }
        }
    }
    sort(dCluster.begin(), dCluster.end());
    unsigned long long size = dCluster.size();
    if (this->numberStoredNearests < size)
    {
        size = this->numberStoredNearests;
    }
    this->nearestClusters.clear();
    for (unsigned long long i = 0; i < size; i++)
    {
        this->nearestClusters.push_back(dCluster.at(i));
    }
}

double CureClusterModel::getMinimumDistance()
{
    if (this->nearestClusters.size() > 0)
    {
        return this->nearestClusters.at(0).distance;
    }
    return numeric_limits<double>::max();
}

unsigned long long CureClusterModel::getMinimumClusterID()
{
    if (this->nearestClusters.size() > 0)
    {
        return this->nearestClusters.at(0).cluster->id;
    }
    return static_cast<unsigned long long>(0);
}

CureClusterModel * CureClusterModel::mergeClusters (const CureClusterModel * clusterA, const CureClusterModel * clusterB)
{
    CureClusterModel * clusterMerged = new CureClusterModel(clusterA->points.at(0));
    for (unsigned long long i = 1; i < clusterA->points.size(); i++)
    {
        clusterMerged->points.push_back(clusterA->points.at(i));
    }
    for (unsigned long long i = 0; i < clusterB->points.size(); i++)
    {
        clusterMerged->points.push_back(clusterB->points.at(i));
    }
    clusterMerged->calculateRepresentatives();
    map<unsigned long long, CureClusterModel *> clusterMap;
    for (ClusterDistance_t clustDist : clusterA->nearestClusters)
    {
        if (clusterB->id != clustDist.cluster->id)
        {
            clusterMap.insert(std::pair<unsigned long long, CureClusterModel *>(clustDist.cluster->id, clustDist.cluster));
        }
    }
    for (ClusterDistance_t clustDist : clusterB->nearestClusters)
    {
        if (clusterA->id != clustDist.cluster->id)
        {
            bool bClusterFound = false;
            for (std::pair<unsigned long long, CureClusterModel *> cluster : clusterMap)
            {
                if (cluster.first == clustDist.cluster->id)
                {
                    bClusterFound = true;
                    break;
                }
            }
            if (!bClusterFound)
            {
                clusterMap.insert(std::pair<unsigned long long, CureClusterModel *>(clustDist.cluster->id, clustDist.cluster));
            }
        }
    }
    clusterMerged->calculateClusterDistances(clusterMap);
    return clusterMerged;
}

void CureClusterModel::calculateRepresentatives()
{
    this->representativePoints.clear();
    if (this->points.size() <= this->numberRepresentativePoints)
    {
        this->representativePoints = this->points;
        return;
    }
    rowvec centerPoint(this->points.at(0));
    for (unsigned long i = 1; i < this->points.size(); i++)
    {
        centerPoint += this->points.at(i);
    }
    this->representativePoints.push_back(centerPoint / static_cast<double>(this->points.size()));
    vector<rowvec> mPoints = this->points;
    while((this->representativePoints.size() < this->numberRepresentativePoints) && (mPoints.size() > 0))
    {
        unsigned long index = 0;
        double distance = -1.0;
        for (unsigned long i = 0; i < mPoints.size(); i++)
        {
            double distanceAcc = 0.0;
            for (rowvec repPoint : this->representativePoints)
            {
                distanceAcc += norm(repPoint - mPoints.at(i), this->distanceNormP);
            }
            if (distanceAcc > distance)
            {
                distance = distanceAcc;
                index = i;
            }
        }
        this->representativePoints.push_back(mPoints.at(index));
        mPoints.erase(mPoints.begin() + index);
    }
    for (unsigned long i = 1; i < this->representativePoints.size(); i++)
    {
        rowvec diffVector = this->representativePoints.at(0) - this->representativePoints.at(i);
        diffVector = diffVector / norm(diffVector, 2);
        this->representativePoints.at(i) += diffVector * this->shiftFactor;
    }
}

void CureClusterModel::reCalculateClusterDistances(unsigned long long lastClusterID, CureClusterModel * cluster)
{
    bool bClusterFound = false;
    unsigned long index = 0;
    for (unsigned long i = 0; i < this->nearestClusters.size(); i++)
    {
        if (lastClusterID == this->nearestClusters.at(i).cluster->id)
        {
            bClusterFound = true;
            index = i;
            break;
        }
    }
    if (!bClusterFound)
    {
        return;
    }
    this->nearestClusters.erase(this->nearestClusters.begin() + index);
    double distance = this->calculateDistance(*cluster);
    if (distance <= 0.0)
    {
        return;
    }
    ClusterDistance_t clusterStruct = {cluster, distance};
    this->nearestClusters.push_back(clusterStruct);
    sort(this->nearestClusters.begin(), this->nearestClusters.end());
}

void CureClusterModel::setNumberRepresentativePoints(unsigned long newValue)
{
    if (newValue > 1)
    {
        CureClusterModel::numberRepresentativePoints = newValue;
    }
}
