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

class CureClusterData
{
private:
    string fileName;
    int partitions;
    double sampleRate;
    bool shuffleAndPartitionData();

public:
    CureClusterData(int partitions, string fileName, string delimiter = ",");
    void clusterData();
    vector<Mat<double>> dataPartitions;
    CSVDataReader reader;

};

#endif // CURECLUSTER_H
