#include <iostream>
#include "array.hpp"

using namespace std;

class Proxy_node
{
public:
    Array *reference = nullptr;
    Proxy_node *next = nullptr;
    Proxy_node *prev = nullptr;
    void clean()
    {
        delete this->reference;
    }
};

class Matrix
{
public:
    int matrix_size[2] = {0, 0};
    Proxy_node *first_array = nullptr;
    Proxy_node *last_array = nullptr;
    int padded_size=0;

    void add_row(Array *new_row)
    {
        if (this->matrix_size[1] == 0)
        {
            Proxy_node *new_node = new Proxy_node;
            new_node->reference = new_row;
            this->first_array = new_node;
            this->last_array = new_node;
            this->matrix_size[0] = new_row->size;
            this->matrix_size[1] += 1;
            return;
        }
        else
        {
            if (this->matrix_size[0] != 0 && new_row->size != this->matrix_size[0])
                throw std::invalid_argument("new array length not match to matrix length");

            Proxy_node *new_proxy = new Proxy_node;
            new_proxy->reference = new_row;
            new_proxy->prev = this->last_array;
            this->last_array->next = new_proxy;
            this->last_array = new_proxy;
            this->matrix_size[1] += 1;
        }
    }

    void remove(int array_index = -1)
    {
        if (this->matrix_size[1] == 1)
        {
            this->reset_matrix();
            return;
        }
        if (this->matrix_size[1] == 0)
            throw std::invalid_argument("matrix is empty");

        if (array_index >= this->matrix_size[1])
            array_index = array_index % this->matrix_size[1];

        if (array_index < 0)
            array_index = this->matrix_size[1] - ((-1 * array_index) % this->matrix_size[1]);

        if (array_index == 0)
            this->remove_first();

        if (0 < array_index && array_index < this->matrix_size[1])
            this->remove_an_index(array_index);
    }
    void pop()
    {
        Proxy_node *temp_proxy = this->last_array;
        this->last_array = this->last_array->prev;
        this->last_array->next = nullptr;
        temp_proxy->clean();
        delete temp_proxy;
        this->matrix_size[1] -= 1;
    }
    void remove_first()
    {
        if (this->matrix_size[1] == 1)
        {
            this->reset_matrix();
            return;
        }
        Proxy_node *temp_proxy = this->first_array;
        this->first_array = this->first_array->next;
        this->first_array->prev = nullptr;
        temp_proxy->clean();
        delete temp_proxy;
        this->matrix_size[1] -= 1;
    }
    void remove_an_index(int array_index = 0)
    {
        if (this->matrix_size[0] == 0 || this->matrix_size[1] == 0)
            throw std::invalid_argument("matrix is empty");

        if (array_index >= this->matrix_size[1])
            throw std::invalid_argument("index is overlap");

        Proxy_node *target_proxy = this->first_array;
        for (int i = 0; i < array_index; i++)
        {
            target_proxy = target_proxy->next;
        }
        target_proxy->prev->next = target_proxy->next;
        target_proxy->next->prev = target_proxy->prev;
        target_proxy->clean();
        delete target_proxy;
        this->matrix_size[1] -= 1;
    }
    void reset_matrix()
    {
    }

    double get(int x = 0, int y = 0)
    {
        if (this->matrix_size[0] == 0 || this->matrix_size[1] == 0)
            throw std::invalid_argument("matrix is empty");

        if (x >= this->matrix_size[0])
            throw std::invalid_argument("x index is overlap");

        if (y >= this->matrix_size[1])
            throw std::invalid_argument("y index is overlap");

        Proxy_node *temp_proxy = this->first_array;
        for (int i = 0; i < y; i++)
        {
            temp_proxy = temp_proxy->next;
        }
        return temp_proxy->reference->get(x);
    }

    void size()
    {
        cout << "[" << this->matrix_size[0] << "," << this->matrix_size[1] << "]";
    }

    void show()
    {
        Proxy_node *pointer = this->first_array;
        for (int i = 0; i < this->matrix_size[1]; i++)
        {
            pointer->reference->show();
            cout << "\n";
            pointer = pointer->next;
        }
    }

    void padding(int pad_value = 0, int pad_size = 0)
    {
        Proxy_node *pointer = this->first_array;
        for (int i = 0; i < this->matrix_size[1]; i++)
        {
            for (int j = 0; j < pad_size; j++)
            {
                pointer->reference->push(pad_value);
                pointer->reference->add_to_first(pad_value);
            }
            pointer = pointer->next;
        }
        this->matrix_size[0] += (2 * pad_size);
        for (int i = 0; i < pad_size; i++)
        {
            Array *new_row = fill(this->matrix_size[0],pad_value);
            this->add_to_first(new_row);
            this->add_row(new_row);
        }
        this->padded_size=pad_size;
    }

private:
    void add_to_first(Array *new_row)
    {
        Proxy_node *new_node = new Proxy_node;
        new_node->reference = new_row;
        new_node->next = this->first_array;
        this->first_array->prev = new_node;
        this->first_array = new_node;
        this->matrix_size[1] += 1;
    }
};
