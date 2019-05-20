#ifndef CUREMANAGER_H
#define CUREMANAGER_H

#include <iostream>
#include <armadillo>
#include <map>
#include <vector>
#include <string>
#include "cureclustermodel.h"
#include "curealgorithm.h"
#include "cure/cureclusterdata.h"

using std::string;
using std::cout;
using std::vector;
using std::map;
using arma::Mat;
using arma::rowvec;
using std::ofstream;

class CureManager
{
private:
    CureClusterData clusterData;
    int numClusters;
    void cluster();
    bool bClustered;
    map<unsigned long long, CureClusterModel *> clustersMap;
public:
    CureManager(string fileName, int numberClusters, int numberPartitions = 5, double shiftFactor = 0.1);
    void saveResult(string fileName);
};

#endif // CUREMANAGER_H
