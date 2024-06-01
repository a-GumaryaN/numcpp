#include <iostream>
#include "array.hpp"

template <typename Value_type>
class Proxy_node
{
public:
    Array<Value_type> *reference = nullptr;
    Proxy_node *next = nullptr;
    Proxy_node *prev = nullptr;
    void clean()
    {
        delete this->reference;
    }
};

template <typename Value_type>
class Matrix
{
public:
    int matrix_size[2] = {0, 0};
    Proxy_node<Value_type> *first_array = nullptr;
    Proxy_node<Value_type> *last_array = nullptr;
    int padded_size = 0;

    void add_row(Array<Value_type> *new_row)
    {
        if (this->matrix_size[1] == 0)
        {
            Proxy_node<Value_type> *new_node = new Proxy_node<Value_type>;
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

            Proxy_node<Value_type> *new_proxy = new Proxy_node<Value_type>;
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
        Proxy_node<Value_type> *temp_proxy = this->last_array;
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
        Proxy_node<Value_type> *temp_proxy = this->first_array;
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

        Proxy_node<Value_type> *target_proxy = this->first_array;
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

    Value_type get(int x = 0, int y = 0)
    {
        if (this->matrix_size[0] == 0 || this->matrix_size[1] == 0)
            throw std::invalid_argument("matrix is empty");

        if (x >= this->matrix_size[0])
            throw std::invalid_argument("x index is overlap");

        if (y >= this->matrix_size[1])
            throw std::invalid_argument("y index is overlap");

        Proxy_node<Value_type> *temp_proxy = this->first_array;
        for (int i = 0; i < y; i++)
        {
            temp_proxy = temp_proxy->next;
        }
        return temp_proxy->reference->get(x);
    }

    void size()
    {
        std::cout << "[" << this->matrix_size[0] << "," << this->matrix_size[1] << "]";
    }

    void show()
    {
        Proxy_node<Value_type> *pointer = this->first_array;
        for (int i = 0; i < this->matrix_size[1]; i++)
        {
            pointer->reference->show();
            std::cout << "\n";
            pointer = pointer->next;
        }
    }

    void padding(int pad_value = 0, int pad_size = 0)
    {
        Proxy_node<Value_type> *pointer = this->first_array;
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
            Array<Value_type> *new_row = fill(this->matrix_size[0], pad_value);
            this->add_to_first(new_row);
            this->add_row(new_row);
        }
        this->padded_size = pad_size;
    }

    void set(int x, int y, Value_type new_value)
    {
        if (this->matrix_size[0] == 0 || this->matrix_size[1] == 0)
            throw std::invalid_argument("matrix is empty");

        if (x >= this->matrix_size[0])
            throw std::invalid_argument("x index is overlap");

        if (y >= this->matrix_size[1])
            throw std::invalid_argument("y index is overlap");

        Proxy_node<Value_type> *temp_proxy = this->first_array;
        for (int j = 0; j < y; j++)
        {
            temp_proxy = temp_proxy->next;
        }
        temp_proxy->reference->set(new_value, x);
    }

private:
    void add_to_first(Array<Value_type> *new_row)
    {
        Proxy_node<Value_type> *new_node = new Proxy_node<Value_type>;
        new_node->reference = new_row;
        new_node->next = this->first_array;
        this->first_array->prev = new_node;
        this->first_array = new_node;
        this->matrix_size[1] += 1;
    }
};

template <typename Value_type>
Matrix<Value_type> *read_matrix(std::string str)
{
    bool is_number = false,
         is_float = false;
    int decimal_number = 0,
        depth = 0;
    double temp_number = 0;
    Array<Value_type> *temp_array = new Array<Value_type>;
    Matrix<Value_type> *result = new Matrix<Value_type>;

    for (char c : str)
    {
        if (c == '[')
        {
            depth += 1;
            continue;
        }
        if (isdigit(c))
        {
            is_number = true;
            if (is_float)
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
            is_float = true;
            continue;
        }
        if ((c == ',' && depth >= 2) || c == ']')
        {
            temp_number = temp_number / pow(10, decimal_number);
            temp_array->add(temp_number);
            temp_number = 0;
            decimal_number = 0;
            is_float = false;
            is_number = false;
        }
        if (c == ']')
        {
            if (depth == 2)
            {
                result->add_row(temp_array);
                temp_array = new Array<Value_type>;
            }

            depth -= 1;
            continue;
        }
    }
    return result;
}

template <typename Value_type>
Matrix<Value_type> *zero_matrix(int x = 0, int y = 0)
{
    Matrix<Value_type> *result = new Matrix<Value_type>;
    Array<Value_type> *zero_array = zeros<Value_type>(x);

    for (int i = 0; i < y; i++)
        result->add_row(zero_array);

    return result;
}

template <typename Value_type>
Matrix<Value_type> *filled_matrix(int x = 0, int y = 0, double value = 0)
{
    Matrix<Value_type> *result = new Matrix<Value_type>;
    Array<Value_type> *zero_array = fill(x, value);

    for (int i = 0; i < y; i++)
        result->add_row(zero_array);

    return result;
}

template <typename Value_type>
double convolve(Matrix<Value_type> *main_matrix, Matrix<Value_type> *kernel, int center_i = 0, int center_j = 0, int target_i = 0, int target_j = 0)
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

template <typename Value_type>
Matrix<Value_type> *conv(Matrix<Value_type> *main_matrix, Matrix<Value_type> *kernel, int padding_size = 0, double padding_value = 0, int center_i = 0, int center_j = 0)
{

    int main_matrix_x = main_matrix->matrix_size[0],
        main_matrix_y = main_matrix->matrix_size[1];

    main_matrix->padding(padding_value, padding_size);

    Matrix<Value_type> *result = new Matrix<Value_type>;

    for (int i = 0; i < main_matrix_x; i++)
    {
        Array<Value_type> *temp_array = new Array<Value_type>;
        for (int j = 0; j < main_matrix_y; j++)
        {
            double convolve_value = convolve(main_matrix, kernel, center_i, center_j, i + padding_size, j + padding_size);
            temp_array->push(convolve_value);
        }
        result->add_row(temp_array);
    }

    return result;
}