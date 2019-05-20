#ifndef CURECLUSTER_H
#define CURECLUSTER_H

#include <string>
#include "files/csvdatareader.h"
#include <armadillo>
#include <algorithm>
#include <vector>

using arma::Mat;
using std::string;
using std::vector;

class CureCluster
{
private:
    string fileName;
    CSVDataReader reader;
    int clusters;
    int partitions;
    double sampleRate;
    bool shuffleAndPartitionData();

public:
    CureCluster(int clusters, int partitions, string fileName, string delimiter = ",");
    void clusterData();
    vector<Mat<double>> dataPartitions;

};

#endif // CURECLUSTER_H
