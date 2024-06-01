#include <iostream>
#include <cmath>
#include "node.hpp"

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

template <typename Value_type>
class Array
{
public:
    int size = 0;
    Node<Value_type> *first_node = nullptr;
    Node<Value_type> *last_node = nullptr;

    // ************************* method's for access values *************************
    void show()
    {
        if (this->size == 0)
            std::cout << "array is empty\n";

        Node<Value_type> *temp_pointer = this->first_node;
        std::cout << "[ ";
        for (int i = 0; i < this->size; i++)
        {
            std::cout << temp_pointer->value << ", ";
            temp_pointer = temp_pointer->next;
        }
        std::cout << " ]";
    }

    Value_type get(int node_number = 0)
    {
        if (node_number >= this->size)
        {
            // raise overlap error
            throw std::invalid_argument("index is overlap");
        }
        Node<Value_type> *temp_pointer = this->first_node;
        for (int i = 0; i < node_number; i++)
        {
            temp_pointer = temp_pointer->next;
        }
        return temp_pointer->value;
    }

    // ************************* method's for changing values *************************

    void push(Value_type new_value)
    {
        // Create new node (same as before)
        Node<Value_type> *new_node = new Node<Value_type>;
        // set created node value
        new_node->value = new_value;
        if (this->first_node == nullptr)
        {
            // set first_node pointer to created node
            this->first_node = new_node;
            // set last_node pointer to created node
            this->last_node = new_node;
            // set next of created node to nullptr
            this->last_node->next = nullptr;
            // set next of first created node to nullptr
            this->first_node->prev = nullptr;
        }
        else
        {
            // Set next of current last node to the new node
            this->last_node->next = new_node;
            // Set prev of the new node to the current last node
            new_node->prev = this->last_node;
            // Update last_node to point to the newly added node
            this->last_node = new_node;
            // set next of created node to nullptr
            this->last_node->next = nullptr;
        }
        this->size++;
    }

    void pop()
    {
        if (this->size == 0)
            throw std::invalid_argument("array is empty");

        if (this->last_node == this->first_node)
        {
            delete this->last_node;
            this->last_node = nullptr;
            this->first_node = nullptr;
            this->size = 0;
        }
        if (this->last_node != nullptr)
        {
            Node<Value_type> *temp_node = this->last_node;

            this->last_node = this->last_node->prev;

            this->last_node->next = nullptr;

            delete temp_node;

            this->size--;
        }
    }

    void set(Value_type new_value = 0, int location = 0)
    {

        if (this->size == 0)
            throw std::invalid_argument("array is empty");

        if (location < 0)
            location = this->size - ((-1 * location) % this->size);

        if (location >= this->size)
            location = location % this->size;

        Node<Value_type> *temp_pointer = this->first_node;

        for (int i = 0; i < location; i++)
        {
            temp_pointer = temp_pointer->next;
        }
        temp_pointer->value = new_value;
        return;
    }

    void add(Value_type value = 0, int prev_index = -2)
    {

        if (prev_index == -1)
        {
            this->add_to_first(value);
            return;
        }

        if (prev_index <= -2 || prev_index >= this->size - 1)
        {
            this->push(value);
            return;
        }

        if (prev_index < this->size - 1)
            this->add_after_node(value, prev_index);
    }

    void add_to_first(Value_type value)
    {
        Node<Value_type> *new_node = new Node<Value_type>;
        new_node->value = value;

        new_node->next = this->first_node;

        this->first_node->prev = new_node;
        this->first_node = new_node;
        this->size += 1;
    }

    void add_after_node(Value_type value = 0, int prev_index = 0)
    {

        if (prev_index >= this->size)
        {
            // raise overlap error
            throw std::invalid_argument("index is overlap");
        }

        Node<Value_type> *new_node = new Node<Value_type>;
        new_node->value = value;

        Node<Value_type> *temp_pointer = this->first_node;
        for (int i = 0; i < prev_index; i++)
        {
            temp_pointer = temp_pointer->next;
        }

        new_node->prev = temp_pointer;
        new_node->next = temp_pointer->next;

        temp_pointer->next->prev = new_node;
        temp_pointer->next = new_node;

        // set next of created node to nullptr
        this->last_node->next = nullptr;

        this->size++;
    }

    void remove(int index_number = -1)
    {

        if (this->size == 1)
        {
            this->reset_array();
            return;
        }

        if (index_number >= this->size)
            index_number = index_number % this->size;

        if (index_number < 0)
            index_number = this->size - ((-1 * index_number) % this->size);

        if (this->size == 0)
            throw std::invalid_argument("array is empty");

        if (index_number == 0)
            this->remove_first();

        if (0 < index_number && index_number < this->size)
            this->remove_an_index(index_number);
    }

    void reset_array()
    {

        Node<Value_type> *temp_node = this->first_node;
        delete temp_node;

        this->first_node == nullptr;
        this->last_node == nullptr;
        this->size = 0;
    }

    void remove_first()
    {
        Node<Value_type> *temp_node = this->first_node;

        this->first_node->next->prev = nullptr;

        this->first_node = this->first_node->next;

        this->size -= 1;

        delete temp_node;
    }

    void remove_an_index(int index_number = 0)
    {
        if (this->size == 0)
            throw std::invalid_argument("array is empty");

        if (index_number + 1 > this->size)
            // raise overlap error
            throw std::invalid_argument("index is overlap");

        Node<Value_type> *temp_pointer = this->first_node;
        for (int i = 0; i < index_number; i++)
        {
            temp_pointer = temp_pointer->next;
        }

        // this if for removing when only one node exist
        if (this->last_node == this->first_node)
        {
            this->last_node = nullptr;
            this->first_node = nullptr;
            this->size = 0;
            delete temp_pointer;
            return;
        }

        // this two if for removing first or last node
        if (this->last_node == temp_pointer)
        {
            this->last_node = temp_pointer->prev;
            temp_pointer->prev->next = nullptr;
        }

        if (this->first_node == temp_pointer)
        {
            this->first_node = temp_pointer->next;
            temp_pointer->next->prev = nullptr;
        }

        // this two if for remove a node in middle of chain
        if (temp_pointer->prev)
            temp_pointer->prev->next = temp_pointer->next;

        if (temp_pointer->next)
            temp_pointer->next->prev = temp_pointer->prev;

        this->size -= 1;

        delete temp_pointer;
    }

    // ************************* method's for statistics calculation *************************

    double mean()
    {
        double avg = 0;
        Node<Value_type> *temp_pointer = this->first_node;
        for (int i = 0; i < this->size; i++)
        {
            avg += temp_pointer->value;
            temp_pointer = temp_pointer->next;
        }
        return avg / this->size;
    }

    double median()
    {
        if (this->size % 2 != 0)
        {
            return this->get(this->size / 2);
        }
        else
        {
            double a = this->get(this->size / 2);
            double b = this->get((this->size / 2) - 1);
            return (a + b) / 2;
        }
    }

    double variance()
    {
        double square_of_diff = 0;
        double mean = this->mean();
        Node<Value_type> *temp_node = this->first_node;

        for (int i = 0; i < this->size; i++)
        {
            square_of_diff += pow((mean - temp_node->value), 2);
            temp_node = temp_node->next;
        }

        return square_of_diff / this->size;
    }

    double std()
    {
        double variance = this->variance();
        return sqrt(variance);
    }
};

template <typename Value_type>
Array<Value_type> *read_array(std::string str)
{
    Array<Value_type> *new_array = new Array<Value_type>;
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

template <typename Value_type>
Array<Value_type> *zeros(int length = 0)
{
    Array<Value_type> *new_array = new Array<Value_type>;
    for (int i = 0; i < length; i++)
        new_array->add(0);
    return new_array;
}

template <typename Value_type>
Array<Value_type> *fill(int length = 0, Value_type value = 0)
{
    Array<Value_type> *new_array = new Array<Value_type>;
    for (int i = 0; i < length; i++)
        new_array->add(value);
    return new_array;
}

template <typename Value_type>
double dot(Array<Value_type> *array_1, Array<Value_type> *array_2)
{
    if (array_1->size != array_2->size)
        throw std::invalid_argument("array's are not the same length");

    double result = 0;
    for (int i = 0; i < array_1->size; i++)
    {
        result += array_1->get(i) * array_2->get(i);
    }
    return result;
}

template <typename Value_type>
Array<Value_type> *multiply(Array<Value_type> *array_1, Array<Value_type> *array_2)
{
    Array<Value_type> *result = new Array<Value_type>;
    if (array_1->size != array_2->size)
        throw std::invalid_argument("array's are not the same length");

    for (int i = 0; i < array_1->size; i++)
    {
        result->push(array_1->get(i) * array_2->get(i));
    }
    return result;
}