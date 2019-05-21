#ifndef CURECLUSTERMODEL_H
#define CURECLUSTERMODEL_H

#include <armadillo>
#include <iostream>
#include <vector>
#include <limits>
#include <map>

using std::numeric_limits;
using std::cout;
using arma::Mat;
using arma::rowvec;
using std::vector;
using arma::norm;
using std::map;

struct ClusterDistance_t;

class CureClusterModel
{
private:
    // PRIVATE FUNCTIONS
    double calculateDistance(const CureClusterModel & cluster) const;
    // PRIVATE VARIABLES
    static unsigned long long numberStoredNearests;
    static unsigned long long id_counter;
    static unsigned long numberRepresentativePoints;

public:
    // PUBLIC FUNCTIONS
    CureClusterModel(rowvec point);
    static void setNumberRepresentativePoints(unsigned long newValue);
    static CureClusterModel * mergeClusters (const CureClusterModel * clusterA, const CureClusterModel * clusterB);
    void calculateRepresentatives();
    void calculateClusterDistances(map<unsigned long long, CureClusterModel *> & clusters);
    void reCalculateClusterDistances(unsigned long long lastClusterID, CureClusterModel * cluster);
    double getMinimumDistance();
    unsigned long long getMinimumClusterID();
    double calculatePointDistance(const rowvec & vectorA);
    // PUBLIC VARIABLES
    static double shiftFactor;
    static arma::uword distanceNormP;
    const unsigned long long id;
    vector<rowvec> points;
    vector<rowvec> representativePoints;
    vector<ClusterDistance_t> nearestClusters;
};

struct ClusterDistance_t
{
    CureClusterModel * cluster;
    double distance;

    bool operator < (ClusterDistance_t & clu) const
    {
        return (distance < clu.distance);
    }

    bool operator > (ClusterDistance_t & clu) const
    {
        return (distance > clu.distance);
    }
};

#endif // CURECLUSTERMODEL_H

