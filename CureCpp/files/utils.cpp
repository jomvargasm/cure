#include "utils.h"

#include <map>
#include <iostream>
#include <iostream>

using std::cout;
using std::getline;
using std::stringstream;


vector<string> split(string text, string separator)
{
    std::replace(text.begin(), text.end(), separator.at(0), ' ');
    stringstream ss(text);
    vector<string> array;
    string temp;
    while(ss >> temp)
    {
        array.push_back(temp);
    }
    return array;
}

vector<DATA_TYPE> convertToDataTypes(vector<string> & line)
{
    vector<DATA_TYPE> types;
    for (string field : line)
    {
        stringstream sstr2(field);
        double d = -1.0;
        sstr2 >> d;
        if (!sstr2.fail() && sstr2.eof())
        {
            types.push_back(static_cast<DATA_TYPE>(DT_DOUBLE));
        }
        else
        {
            types.push_back(static_cast<DATA_TYPE>(DT_STRING));
        }
    }
    return types;
}
