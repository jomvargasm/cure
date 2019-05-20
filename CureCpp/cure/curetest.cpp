#include "cure/curetest.h"

void testShuffleDataCure()
{
    CureCluster cluster(5, 3, "../CureCpp/files/data_test.csv");
    cluster.clusterData();
    for (Mat<double> data : cluster.dataPartitions)
    {
        cout << data << "\n ********************** \n";
    }
}
