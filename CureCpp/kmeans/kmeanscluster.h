#ifndef KMEANSCLUSTER_H
#define KMEANSCLUSTER_H

#include <armadillo>
#include <vector>
#include <iostream>

using std::vector;
using arma::vec;
using std::cout;
using arma::Mat;

class KMeansCluster
{
private:
    // PRIVATE FUNCTIONES
    double calculateDistance(vec vector1, vec vector2);
    unsigned long getNearestCentroidIndex(vector<vec> & centroids, vec vector1);
    vector<vec> createCentroidsRandomly();
    vector<vector<long>> createClusterVectors();
    // PRIVATE VARIABLES
    const long MAX_CLUSTER_ITERATIONS = 500;
    Mat<double> & clusterData;
    int clustersNumber;
    arma::uword pNormExponent;

public:
    // PUBLIC FUNCTIONS
    KMeansCluster(Mat<double> & data, int k);
    void setPNormaExponent(int pExp);
    void cluster();
    //PUBLIC VARIABLES
};

#endif // KMEANSCLUSTER_H
