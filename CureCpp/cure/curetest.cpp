#include "cure/curetest.h"

void testShuffleDataCure()
{
    CureClusterData cluster(3, "../CureCpp/files/data_test.csv");
    cluster.clusterData();
    for (Mat<double> data : cluster.dataPartitions)
    {
        cout << data << "\n ********************** \n";
    }
}
