#include "csvdatareader.h"


using std::make_pair;

CSVDataReader::CSVDataReader(string fileName, string delimiter)
{
    this->fileName = fileName;
    this->delimiter = delimiter;
    this->file.open(this->fileName);
    if (!this->file.good())
    {
        cout << "No pudo abrir el archivo \n";
        return;
    }
    string headerLine;
    if (!getline(this->file, headerLine))
    {
        cout << "No hay línea de encabezado \n";
        return;
    }
    this->headers = split(headerLine, this->delimiter);
    this->fieldsCount = static_cast<long>(this->headers.size());
    string lineText;
    if (!getline(this->file, lineText))
    {
        cout << "No hay líneas de datos \n";
        return;
    }
    vector<string> firstLine = split(lineText, this->delimiter);
    this->types = convertToDataTypes(firstLine);
    this->rowsCount = 1;
    while (getline(this->file, lineText))
    {
        if (lineText.length() > 1)
        {
            this->rowsCount++;
        }
    }
    this->file.close();
    this->file.clear();
}

void CSVDataReader::readData()
{
    this->file.open(this->fileName);
    if (!this->file.good())
    {
        cout << "No pudo abrir el archivo \n";
        return;
    }
    string lineText;
    if (!getline(this->file, lineText))
    {
        cout << "No hay línea de encabezado \n";
        return;
    }
    this->data = Mat<double>(static_cast<arma::uword>(this->rowsCount), static_cast<arma::uword>(this->fieldsCount));
    this->mappersEncoder = vector<map<string, int>>();
    this->mappersDecoder = vector<vector<string>>();
    for (int j = 0; j < this->fieldsCount; j++)
    {
        map<string, int> mapper;
        this->mappersEncoder.push_back(mapper);
        vector<string> encoder;
        this->mappersDecoder.push_back(encoder);
    }
    arma::uword rows = 0;
    while(getline(this->file, lineText))
    {
        if (static_cast<long>(rows) > this->rowsCount)
        {
            cout << "Se encontraron más líneas \n";
            break;
        }
        vector<string> dataLine = split(lineText, this->delimiter);
        if (static_cast<long>(dataLine.size()) != this->fieldsCount)
        {
            cout << "Se encontró una línea con diferente cantidad de atributos #" << rows << " :" << lineText;
            break;
        }
        for (std::size_t i = 0; i < dataLine.size(); i++)
        {
            if (this->types.at(i) == DT_DOUBLE)
            {
                fillDataDouble(dataLine, rows, i);
            }
            else
            {
                fillDataString(dataLine, rows, i);
            }
        }
        rows++;
    }
    this->file.close();
    this->file.clear();
}

void CSVDataReader::fillDataDouble(vector<string> & dataLine, arma::uword rows, std::size_t i)
{
    stringstream sstr2(dataLine.at(i));
    double data;
    sstr2 >> data;
    if (!sstr2.fail() && sstr2.eof())
    {
        this->data(rows, i) = data;
    }
    else
    {
        cout << "Se encontró una línea con diferente tipo de dato que el encabezado f#" << rows << " c#" << i << " :" << dataLine.at(i) << "\n";
        this->data(rows, i) = -1.0;
    }
}

void CSVDataReader::fillDataString(vector<string> & dataLine, arma::uword rows, std::size_t i)
{
    int discreteIndex;
    map<string, int>::iterator searchResult = this->mappersEncoder.at(i).find(dataLine.at(i));
    if (searchResult == this->mappersEncoder.at(i).end())
    {
        discreteIndex = static_cast<int>(this->mappersDecoder.at(i).size());
        this->mappersEncoder.at(i).insert(make_pair(dataLine.at(i), discreteIndex));
        this->mappersDecoder.at(i).push_back(dataLine.at(i));
    }
    else
    {
        discreteIndex = searchResult->second;
    }
    this->data(rows, i) = static_cast<double>(discreteIndex);
}

vector<DATA_TYPE> CSVDataReader::getDataTypes()
{
    return this->types;
}

vector<string> CSVDataReader::getHeaders()
{
    return this->headers;
}

