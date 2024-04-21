#include <iostream>
#include "matrix.hpp"

using namespace std;

int main()
{

    // create new array
    Array *array1 = new Array;

    // read value of array from string
    array1 = read_array("[114.184, 1965.18149, 135.16526, 1556.1456,]");

    // show entity of a created array
    array1->show();

    // calculate some statistics of array data
    cout << "standard deviation :\n";
    cout << array1->std() << "\n";

    cout << "variance :\n";
    cout << array1->variance() << "\n";

    cout << "mean :\n";
    cout << array1->mean() << "\n";

    cout << "median :\n";
    cout << array1->median() << "\n";

    Array *array2 = new Array;
    array2 = read_array("[11,12,13,14]");

    // create new matrix
    Matrix *mat1 = new Matrix;

    // add created array's as row to matrix
    mat1->add_row(array1);
    mat1->add_row(array2);

    // show size of matrix
    mat1->size();

    // you can padding matrix
    cout << "original matrix :\n";
    mat1->show();

    mat1->padding(1, 2);

    cout << "after padding :\n";
    mat1->show();
}