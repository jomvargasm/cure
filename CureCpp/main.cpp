#include <iostream>
#include "cure/curetest.h"
#include "cureAlgorithm/curemanager.h"

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    CureManager cure("../CureCpp/data_set_cluster.csv", 15, 1, 0.4);
    cure.saveResult("guardar.csv");
    return 0;
}
