#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <armadillo>

using std::ifstream;
using std::string;
using std::vector;
using std::map;
using arma::Mat;

enum DATA_TYPE {
    DT_STRING, DT_DOUBLE
};

vector<string> split(string text, string separator);
vector<DATA_TYPE> convertToDataTypes(vector<string> & line);

#endif // UTILS_H
