#include <iostream>
#include "cure/curetest.h"
#include "cureAlgorithm/curemanager.h"
#include <QApplication>
#include "cureui.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;
    CureManager cure("housing.csv", 5, 1, 0.1);
    cure.saveResult("guardar_housing.csv");
    return 0;
    /*
    QApplication a(argc, argv);
    CureUI w;
    w.show();
    return a.exec();
    */
}
