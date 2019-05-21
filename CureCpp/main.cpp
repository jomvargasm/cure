#include <iostream>
#include "cure/curetest.h"
#include "cureAlgorithm/curemanager.h"

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    CureManager cure("../CureCpp/data_set_cluster_k8.csv", 8, 1, 0.7);
    cure.saveResult("guardar.csv");
    return 0;
}
