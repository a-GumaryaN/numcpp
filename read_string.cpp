#include <iostream>
#include <cmath>
#include "array.hpp"

using namespace std;

bool isdigit(char ch)
{
    if (int(ch) >= 48 && int(ch) <= 57)
        return true;
    return false;
}

int to_digit(char ch)
{
    return int(ch) - 48;
}

Array *read_array(std::string str)
{
    Array *new_array = new Array;
    bool isNumber = false;
    bool isFloat = false;
    int depth = 0;
    double temp_number = 0;
    int decimal_number = 0;
    for (char c : str)
    {
        if (isdigit(c))
        {
            isNumber = true;
            if (isFloat)
            {
                temp_number = temp_number * 10;
                temp_number += to_digit(c);
                decimal_number += 1;
                continue;
            }
            else
            {
                temp_number = temp_number * 10;
                temp_number += to_digit(c);
                continue;
            }
        }
        if (c == '.')
        {
            isFloat = true;
            continue;
        }
        if (c == '[')
        {
            depth += 1;
            continue;
        }
        if (c == ']')
        {
            depth -= 1;
        }
        if ((c == ',' || c == ']') && isNumber == true)
        {
            temp_number = temp_number / pow(10, decimal_number);
            new_array->add(temp_number);
            temp_number = 0;
            decimal_number = 0;
            isFloat = false;
            isNumber = false;
            continue;
        }
    }

    return new_array;
}

int main()
{

    Array *new_array = nullptr;

    new_array = read_array("[ 11.5 , 12.36 , ,,,,;;fsnjlnk,,,        12.525   ,     168.21522,2485.31554],");

    new_array->show();

    cout << "standard deviation is:\n";
    cout << new_array->std() << "\n";

    cout << "mean of data is:\n";
    cout << new_array->mean() << "\n";

    cout << "median of dataset is:\n";
    cout << new_array->median() << "\n";

    return 0;
}
