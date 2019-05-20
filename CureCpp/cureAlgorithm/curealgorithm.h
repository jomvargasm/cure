#ifndef CUREALGORITHM_H
#define CUREALGORITHM_H

#include <vector>
#include <map>
#include <armadillo>
#include <iostream>
#include <limits>
#include "cureclustermodel.h"

using std::numeric_limits;
using std::vector;
using arma::Mat;
using arma::vec;
using std::map;
using std::cout;

class CureAlgorithm
{
private:
    //PRIVATE FUNCTIONS

    //PRIVATE VARIABLES
    unsigned long numberClusters;

public:
    //PUBLIC FUNCTIONS
    CureAlgorithm(Mat<double> & points, unsigned long k);
    CureAlgorithm(map<unsigned long long, CureClusterModel *> & data, unsigned long k);
    void clusterData();
    //PUBLIC VARIABLES
    map<unsigned long long, CureClusterModel *> mapData;

};

#endif // CUREALGORITHM_H
