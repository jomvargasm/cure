#ifndef CURECLUSTERMODEL_H
#define CURECLUSTERMODEL_H

#include <armadillo>
#include <iostream>
#include <vector>
#include <limits>

using std::numeric_limits;
using std::cout;
using arma::Mat;
using arma::vec;
using std::vector;
using arma::norm;

struct _ClusterDistance_t;


class CureClusterModel
{
private:
    // PRIVATE FUNCTIONS
    double calculateDistance(CureClusterModel & cluster);
    // PRIVATE VARIABLES
    static int numberStoredNearests;
    double minimumDistance;
    unsigned long long nearestCluster;
    static unsigned long long id_counter;
    static int numberRepresentativePoints;

public:
    // PUBLIC FUNCTIONS
    CureClusterModel();
    CureClusterModel(vec point);
    static void setNumberRepresentativePoints(unsigned long long newValue);
    static CureClusterModel mergeClusters (const CureClusterModel & clusterA, const CureClusterModel & clusterB);
    void calculateRepresentatives();
    void calculateClusterDistances(const vector<CureClusterModel> & clusters);
    void reCalculateClusterDistances(const CureClusterModel & clusters);
    CureClusterModel & getNearestCluster();
    // PUBLIC VARIABLES
    static int distanceNormP;
    const unsigned long long id;
    vector<vec> points;
    vector<vec> representativePoints;
    vector<_ClusterDistance_t> nearestClusters;


};

#endif // CURECLUSTERMODEL_H

