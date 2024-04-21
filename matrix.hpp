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
    int padded_size = 0;

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
            Array *new_row = fill(this->matrix_size[0], pad_value);
            this->add_to_first(new_row);
            this->add_row(new_row);
        }
        this->padded_size = pad_size;
    }

    void set(int x, int y, double new_value)
    {
        if (this->matrix_size[0] == 0 || this->matrix_size[1] == 0)
            throw std::invalid_argument("matrix is empty");

        if (x >= this->matrix_size[0])
            throw std::invalid_argument("x index is overlap");

        if (y >= this->matrix_size[1])
            throw std::invalid_argument("y index is overlap");

        Proxy_node *temp_proxy = this->first_array;
        for (int j = 0; j < y; j++)
        {
            temp_proxy = temp_proxy->next;
        }
        temp_proxy->reference->set(new_value, x);
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

Matrix *zero_matrix(int x = 0, int y = 0)
{
    Matrix *result = new Matrix;
    Array *zero_array = zeros(x);

    for (int i = 0; i < y; i++)
        result->add_row(zero_array);

    return result;
}

Matrix *filled_matrix(int x = 0, int y = 0, double value = 0)
{
    Matrix *result = new Matrix;
    Array *zero_array = fill(x, value);

    for (int i = 0; i < y; i++)
        result->add_row(zero_array);

    return result;
}

double convolve(Matrix *main_matrix, Matrix *kernel, int center_i = 0, int center_j = 0, int target_i = 0, int target_j = 0)
{
    double result;
    int addition_i = target_i - center_i,
        addition_j = target_j - center_j;

    for (int j = 0; j < kernel->matrix_size[1]; j++)
    {
        for (int i = 0; i < kernel->matrix_size[0]; i++)
        {
            result += kernel->get(i, j) * main_matrix->get(addition_i + i, addition_j + j);
        }
    }

    return result;
}

Matrix *conv(Matrix *main_matrix, Matrix *kernel, int padding_size = 0, double padding_value = 0, int center_i = 0, int center_j = 0)
{

    int main_matrix_x = main_matrix->matrix_size[0],
        main_matrix_y = main_matrix->matrix_size[1];

    main_matrix->padding(padding_value, padding_size);

    Matrix *result =new Matrix;

    for (int i = 0; i < main_matrix_x; i++)
    {
        Array* temp_array=new Array;
        for (int j = 0; j < main_matrix_y; j++)
        {
            double convolve_value = convolve(main_matrix, kernel, center_i, center_j, i + padding_size, j + padding_size);
            temp_array->push(convolve_value);
        }
        result->add_row(temp_array);
    }

    return result;
}