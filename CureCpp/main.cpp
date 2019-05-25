#include <iostream>
#include "cure/curetest.h"
#include "cureAlgorithm/curemanager.h"
#include <QApplication>
#include "cureui.h"

using namespace std;

int main(int argc, char *argv[])
{
    /*
    cout << "Hello World!" << endl;
    CureManager cure("../CureCpp/data_set_cluster.csv", 15, 1, 0.9);
    cure.saveResult("guardar.csv");
    return 0;
    */
    QApplication a(argc, argv);
    CureUI w;
    w.show();
    return a.exec();
}
