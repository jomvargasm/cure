#include "cureclustermodel.h"

unsigned long long CureClusterModel::id_counter = 1;
unsigned long CureClusterModel::numberRepresentativePoints = 15;
unsigned long long CureClusterModel::numberStoredNearests = 30;
arma::uword CureClusterModel::distanceNormP = 2;
double CureClusterModel::shiftFactor = 0.1;
double CureClusterModel::maxDistanceStepAllowedFactor = 3.0;

CureClusterModel::CureClusterModel(rowvec point) :
    id(CureClusterModel::id_counter), points({point}), representativePoints({point})
{
    CureClusterModel::id_counter++;
    // this->points.push_back(point);
    // this->representativePoints.push_back(point);
}

double CureClusterModel::calculateDistance(const CureClusterModel & cluster) const
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


double CureClusterModel::calculatePointDistance(rowvec & vectorA)
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

bool CureClusterModel::checkOutlier(rowvec point)
{
    if (this->points.size() < CureClusterModel::numberRepresentativePoints)
    {
        return true;
    }
    rowvec nearestRepresentative(this->representativePoints.at(0));
    double nearestDistance = norm(nearestRepresentative - point, this->distanceNormP);
    unsigned long nearestIndex = 0;
    for (unsigned long i = 1; i < this->representativePoints.size(); i++)
    {
        double newNearestDistance = norm(this->representativePoints.at(i) - point, this->distanceNormP);
        if (newNearestDistance < nearestDistance)
        {
            nearestDistance = newNearestDistance;
            nearestIndex = i;
        }
    }
    nearestRepresentative = this->representativePoints.at(nearestIndex);
    rowvec centerPoint(this->points.at(0));
    for (unsigned long i = 1; i < this->points.size(); i++)
    {
        centerPoint += this->points.at(i);
    }
    centerPoint = centerPoint / static_cast<double>(this->points.size());
    double representativeCenterDistance = norm(centerPoint - nearestRepresentative, this->distanceNormP);
    double pointCenterDistance = norm(centerPoint - point, this->distanceNormP);
    if (pointCenterDistance < representativeCenterDistance)
    {
        return true;
    }
    if (pointCenterDistance > representativeCenterDistance + (1 + 0.2))
    {
        return false;
    }
    return true;
}

CureClusterModel * CureClusterModel::mergeClusters (CureClusterModel * clusterA, CureClusterModel * clusterB)
{
    bool bEliminateOutlier = false;
    CureClusterModel * clusterBig = clusterA;
    CureClusterModel * clusterSmall = clusterB;
    if (clusterA->points.size() >= CureClusterModel::numberRepresentativePoints || clusterB->points.size() >= CureClusterModel::numberRepresentativePoints)
    {
        // revisar si se debe hacer mergue o simplemente eliminar outlier
        if (clusterA->points.size() > clusterB->points.size())
        {
            clusterBig = clusterA;
            clusterSmall = clusterB;
        }
        else
        {
            clusterBig = clusterB;
            clusterSmall = clusterA;
        }
        rowvec nearestRepresentative(clusterSmall->representativePoints.at(0));
        double nearestDistance = clusterBig->calculatePointDistance(nearestRepresentative);
        unsigned long nearestIndex = 0;
        for (unsigned long i = 1; i < clusterSmall->representativePoints.size(); i++)
        {
            double newNearestDistance = clusterBig->calculatePointDistance(clusterSmall->representativePoints.at(i));
            if (newNearestDistance < nearestDistance)
            {
                nearestDistance = newNearestDistance;
                nearestIndex = i;
            }
        }
        nearestRepresentative = clusterSmall->representativePoints.at(nearestIndex);
        bEliminateOutlier = clusterBig->checkOutlier(nearestRepresentative);
    }
    CureClusterModel * clusterMerged = new CureClusterModel(clusterBig->points.at(0));
    for (unsigned long long i = 1; i < clusterBig->points.size(); i++)
    {
        clusterMerged->points.push_back(clusterBig->points.at(i));
    }
    if (!bEliminateOutlier)
    {
        for (unsigned long long i = 0; i < clusterSmall->points.size(); i++)
        {
            clusterMerged->points.push_back(clusterSmall->points.at(i));
        }
    }
    clusterMerged->calculateRepresentatives();
    map<unsigned long long, CureClusterModel *> clusterMap;
    for (ClusterDistance_t clustDist : clusterBig->nearestClusters)
    {
        if (clusterSmall->id != clustDist.cluster->id)
        {
            clusterMap.insert(std::pair<unsigned long long, CureClusterModel *>(clustDist.cluster->id, clustDist.cluster));
        }
    }
    if (!bEliminateOutlier)
    {
        for (ClusterDistance_t clustDist : clusterSmall->nearestClusters)
        {
            if (clusterBig->id != clustDist.cluster->id)
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
    centerPoint = centerPoint / static_cast<double>(this->points.size());
    unsigned long nearestCenterPointIndex = 0;
    double centerPointDistance = -1;
    for (unsigned long i = 0; i < this->points.size(); i++)
    {
        double newCenterPointDistance = arma::norm(centerPoint - this->points.at(i), CureClusterModel::distanceNormP);
        if (i == 0 || newCenterPointDistance > centerPointDistance)
        {
            centerPointDistance = newCenterPointDistance;
            nearestCenterPointIndex = i;
        }
    }
    this->representativePoints.push_back(this->points.at(nearestCenterPointIndex));
    vector<rowvec> mPoints = this->points;
    while((this->representativePoints.size() < this->numberRepresentativePoints) && (mPoints.size() > 0))
    {
        unsigned long index = 0;
        double distance = -1.0;
        for (unsigned long i = 0; i < mPoints.size(); i++)
        {
            double distanceAcc = norm(mPoints.at(i) - centerPoint, this->distanceNormP);
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
