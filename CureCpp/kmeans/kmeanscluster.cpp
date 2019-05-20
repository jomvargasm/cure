#include "kmeanscluster.h"

/*
 *  REF: http://eric-yuan.me/k-means/
 *
*/

KMeansCluster::KMeansCluster(Mat<double> & data, int k) :
    clusterData(data),
    clustersNumber(k),
    pNormExponent(2)
{

}

void KMeansCluster::setPNormaExponent(int pExp)
{
    if (pExp < 1)
    {
        cout << "El valor expecificado no es válido \n";
        return;
    }
    this->pNormExponent = static_cast<arma::uword>(pExp);
}

double KMeansCluster::calculateDistance(vec vector1, vec vector2)
{
    return arma::norm(vector1 - vector2, this->pNormExponent);
}

unsigned long KMeansCluster::getNearestCentroidIndex(vector<vec> & centroids, vec vector1)
{
    double minDistance = 0.0;
    unsigned long minIndex = 0;
    for (unsigned long i = 0; i < centroids.size(); i++)
    {
        double tempDistance = calculateDistance(vector1, centroids.at(i));
        if (i == 0 || tempDistance < minDistance)
        {
            minDistance = tempDistance;
            minIndex = i;
        }
    }
    return minIndex;
}

vector<vec> KMeansCluster::createCentroidsRandomly()
{
    vector<vec> centroids;
    arma::uvec randIndexVector = arma::shuffle(arma::linspace<arma::uvec>(0, this->clusterData.n_rows - 1, this->clusterData.n_rows));
    for(arma::uword i = 0; i < static_cast<arma::uword>(this->clustersNumber); i++)
    {
        centroids.push_back(this->clusterData.row(i));
    }
    return centroids;
}

vector<vector<long>> KMeansCluster::createClusterVectors()
{
    vector<vector<long>> clustersVector;
    for(arma::uword i = 0; i < static_cast<arma::uword>(this->clustersNumber); i++)
    {
        vector<long> tempVector;
        clustersVector.push_back(tempVector);
    }
    return clustersVector;
}

void KMeansCluster::cluster()
{
    vector<vec> centroids (createCentroidsRandomly());
    vector<vector<long>> clustersVector(createClusterVectors());
    long m;
    for (m = 0; m < this->MAX_CLUSTER_ITERATIONS; m++)
    {
        bool bConverged = true;
        for (arma::uword k = 0; k < static_cast<arma::uword>(this->clustersNumber); k++)
        {
            clustersVector.at(k).clear();
        }
        for (arma::uword j = 0; j < this->clusterData.n_rows; j++)
        {
            unsigned long minIndex = getNearestCentroidIndex(centroids, this->clusterData.row(j));
            clustersVector.at(minIndex).push_back(static_cast<long>(j));
        }
        for(arma::uword k = 0; k < static_cast<arma::uword>(this->clustersNumber); k++)
        {
            unsigned long long clusterSize = clustersVector.at(k).size();
            double clusterSizeDouble = static_cast<double>(clusterSize);
            vec tempCentroidVector = arma::zeros<vec>(this->clusterData.n_cols);
            for (arma::uword i = 0; i < clusterSize; i++)
            {
                tempCentroidVector += this->clusterData.row(static_cast<arma::uword>(clustersVector.at(k).at(i))) / clusterSizeDouble;
            }
            if (calculateDistance(tempCentroidVector, centroids.at(k)) > 1e-6)
            {
                centroids.assign(k, tempCentroidVector);
                bConverged = false;
            }
        }
        if (bConverged)
        {
            cout << "Se creán los cluster satisfactoriamente \n";
            break;
        }
    }
    if (m >= this->MAX_CLUSTER_ITERATIONS)
    {
        cout << "No fue posible converger los clsuter satisfactoriamente, superó las iteraciones máximas " << this->MAX_CLUSTER_ITERATIONS << "\n";
    }
}
