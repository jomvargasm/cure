#include "files/filestest.h"
#include<iomanip>

using arma::uword;
using std::setw;
using std::setprecision;
using std::fixed;

CSVDataReader csvHeadersTest()
{
    CSVDataReader reader("../CureCpp/files/data_test.csv");
    for (string header : reader.getHeaders())
    {
        cout << header << " - ";
    }
    cout << "\n";
    for (DATA_TYPE type : reader.getDataTypes())
    {
        if (type == DT_STRING)
        {
            cout << "String - ";
        }
        else if (type == DT_DOUBLE)
        {
            cout << "Double - ";
        }
    }
    cout << "\n";
    cout << "Líneas: " << reader.rowsCount << "\n";
    return reader;
}

CSVDataReader csvDataReadTest()
{
    CSVDataReader reader = csvHeadersTest();
    reader.readData();
    cout << "------------------\n";
    vector<DATA_TYPE> types = reader.getDataTypes();
    uword k = 0;
    int width = 12;
    for (string header : reader.getHeaders())
    {
        string type = header + ":string";
        if (types.at(k) == DT_DOUBLE)
        {
            type = header + ":double";
        }
        cout << setw(width) << type << setw(width) << " |\t";
        k++;
    }
    cout << "\n";
    for(uword i = 0; i < reader.data.n_rows; i++)
    {
        for(uword j = 0; j < reader.data.n_cols; j++)
        {
            cout << setw(width) << setprecision(3) << fixed << reader.data(i, j) << setw(width) << " |\t";
        }
        cout << "\n";
    }
    return reader;
}
