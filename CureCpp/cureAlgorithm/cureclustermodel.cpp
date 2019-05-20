#include "cureclustermodel.h"

unsigned long long CureClusterModel::id_counter = 1;
int CureClusterModel::numberRepresentativePoints = 5;
int CureClusterModel::numberStoredNearests = 10;
int CureClusterModel::distanceNormP = 2;

struct _ClusterDistance_t
{
    CureClusterModel & cluster;
    double distance;

    bool operator < (_ClusterDistance_t & clu) const
    {
        return (distance < clu.distance);
    }
};

CureClusterModel::CureClusterModel() :
    id(CureClusterModel::id_counter)
{
    CureClusterModel::id_counter++;
}


CureClusterModel::CureClusterModel(vec point) :
    id(CureClusterModel::id_counter)
{
    CureClusterModel::id_counter++;
    this->points.push_back(point);
    this->representativePoints.push_back(point);
}

double CureClusterModel::calculateDistance(CureClusterModel & cluster)
{
    double distance = numeric_limits<double>::max();
    double m_distance = 0;
    for (vec vectorA : this->representativePoints)
    {
        for (vec vectorB : cluster.representativePoints)
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

void CureClusterModel::calculateClusterDistances(const vector<CureClusterModel> & clusters)
{
    vector<_ClusterDistance_t> dCluster;
    for (CureClusterModel cluster : clusters)
    {
        double distance = this->calculateDistance(cluster);
        // _ClusterDistance_t clusterStruct(distance, cluster);
        // dCluster.push_back(clusterStruct);
    }
    sort(dCluster.begin(), dCluster.end(), std::greater<_ClusterDistance_t>());

}

void CureClusterModel::setNumberRepresentativePoints(unsigned long long newValue)
{
    if (newValue > 1)
    {
        CureClusterModel::numberRepresentativePoints = newValue;
    }
}
