#include "cureclusterdata.h"

using std::cout;

CureClusterData::CureClusterData(int partitions, string fileName, string delimiter) :
    fileName(fileName),
    sampleRate(0.75),
    reader(fileName, delimiter)
{
    this->partitions = std::max(2, partitions);
    arma::arma_rng::set_seed_random();
}


bool CureClusterData::shuffleAndPartitionData()
{
    unsigned long sample_length = static_cast<unsigned long>(std::max(this->sampleRate * this->reader.rowsCount, 2.0));
    arma::uvec shuffled_rows = arma::shuffle(arma::linspace<arma::uvec>(0, static_cast<unsigned long>(this->reader.rowsCount - 1), static_cast<unsigned long>(this->reader.rowsCount)));
    shuffled_rows = shuffled_rows.rows(0, sample_length);
    arma::uvec column_indices = arma::linspace<arma::uvec>(0, static_cast<unsigned long>(this->reader.fieldsCount - 1), static_cast<unsigned long>(this->reader.fieldsCount));
    unsigned long stepSize = static_cast<unsigned long>(std::floor(static_cast<double>(sample_length) / this->partitions));
    if (stepSize == 0)
    {
        cout << "No hay cantidad suficiente de datos para muestrear a la tasa " << this->sampleRate << " y tomar " << this->partitions << " particiones\n";
        return false;
    }
    for (unsigned long i = 0; i < static_cast<unsigned long>(this->partitions); i++)
    {
        this->dataPartitions.push_back(this->reader.data.submat(shuffled_rows.rows(i * stepSize, (i + 1) * stepSize - 1), column_indices));
    }
    return true;
}

void CureClusterData::clusterData()
{
    this->reader.readData();
    if (!shuffleAndPartitionData())
    {
        return;
    }
}

