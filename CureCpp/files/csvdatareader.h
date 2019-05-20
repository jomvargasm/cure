#ifndef CSVDATAREADER_H
#define CSVDATAREADER_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <armadillo>
#include "utils.h"

using std::ifstream;
using std::string;
using std::vector;
using std::map;
using arma::Mat;
using std::cout;
using std::getline;
using std::stringstream;

class CSVDataReader
{
private:
    string fileName;
    string delimiter;
    ifstream file;
    vector<string> headers;
    vector<map<string, int>> mappersEncoder;
    vector<vector<string>> mappersDecoder;
    vector<DATA_TYPE> types;
    void fillDataDouble(vector<string> & dataLine, arma::uword rows, std::size_t i);
    void fillDataString(vector<string> & dataLine, arma::uword rows, std::size_t i);

public:
    CSVDataReader(string fileName, string delimiter = ",");
    vector<string> getHeaders();
    vector<map<string, int>> getMappers();
    vector<DATA_TYPE> getDataTypes();
    void readData();
    // variables
    long fieldsCount;
    long rowsCount;
    Mat<double> data;
};

#endif // CSVDATAREADER_H
