#include <iostream>
#include "array.hpp"

using namespace std;

class Proxy_node
{
public:
    Array *reference = nullptr;
    Proxy_node *next = nullptr;
    Proxy_node *prev = nullptr;
};

class Matrix
{
public:
    int matrix_size[2] = {0, 0};
    Proxy_node *first_array = nullptr;
    Proxy_node *last_array = nullptr;

    void add_row(Array *new_row)
    {
        if (this->matrix_size[0] != 0 && new_row->size != this->matrix_size[0])
            throw std::invalid_argument("new array length not match to matrix length");

        Proxy_node *new_node = new Proxy_node;
        new_node->reference = new_row;
        this->first_array = new_node;
        this->last_array = new_node;
        this->matrix_size[0] = new_row->size;
        this->matrix_size[1] += 1;
    }

    void remove_row(int array_index = 0)
    {
    }

    void get() {}

    void size()
    {
        cout << "[" << this->matrix_size[0] << "," << this->matrix_size[1] << "]";
    }
};
