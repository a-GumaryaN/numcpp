#include <iostream>
#include <cmath>
#include "node.hpp"

using namespace std;

class Array
{
public:
    int size = 0;
    Node *first_node = nullptr;
    Node *last_node = nullptr;

    // ************************* method's for access values *************************
    void show()
    {
        if (this->size == 0)
            cout << "array is empty\n";

        Node *temp_pointer = this->first_node;
        for (int i = 0; i < this->size; i++)
        {
            cout << temp_pointer->value << "\n";
            temp_pointer = temp_pointer->next;
        }
    }

    double get(int node_number = 0)
    {
        if (node_number >= this->size)
        {
            // raise overlap error
            throw std::invalid_argument("index is overlap");
        }
        Node *temp_pointer = this->first_node;
        for (int i = 0; i < node_number; i++)
        {
            temp_pointer = temp_pointer->next;
        }
        return temp_pointer->value;
    }

    // ************************* method's for changing values *************************

    void push(double new_value)
    {
        if (this->first_node == nullptr)
        {
            // Create new node (same as before)
            Node *new_node = new Node;
            // set created node value
            new_node->value = new_value;
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
            // Create new node
            Node *new_node = new Node;
            // set created node value
            new_node->value = new_value;
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
            Node *temp_node = this->last_node;

            this->last_node = this->last_node->prev;

            this->last_node->next = nullptr;

            delete temp_node;

            this->size--;
        }
    }

    void set(double new_value = 0, int location = 0)
    {

        if (this->size == 0)
            throw std::invalid_argument("array is empty");

        if (location < 0)
            location = this->size - ((-1 * location) % this->size);

        if (location >= this->size)
            location = location % this->size;

        Node *temp_pointer = this->first_node;

        for (int i = 0; i < location; i++)
        {
            temp_pointer = temp_pointer->next;
        }
        temp_pointer->value = new_value;
        return;
    }

    void add(double value = 0, int prev_index = -2)
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

    void add_to_first(double value)
    {
        Node *new_node = new Node;
        new_node->value = value;

        new_node->next = this->first_node;

        this->first_node->prev = new_node;
        this->first_node = new_node;
        this->size += 1;
    }

    void add_after_node(double value = 0, int prev_index = 0)
    {

        if (prev_index >= this->size)
        {
            // raise overlap error
            throw std::invalid_argument("index is overlap");
        }

        Node *new_node = new Node;
        new_node->value = value;

        Node *temp_pointer = this->first_node;
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

        if (this->size == 0)
            throw std::invalid_argument("array is empty");

        if (index_number == 0)
            this->remove_first();

        if (0 < index_number && index_number < this->size)
            this->remove_an_index(index_number);

        if (index_number >= this->size || index_number < 0)
            this->pop();
    }

    void reset_array()
    {

        Node *temp_node = this->first_node;
        delete temp_node;

        this->first_node == nullptr;
        this->last_node == nullptr;
        this->size = 0;
    }

    void remove_first()
    {
        Node *temp_node = this->first_node;

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

        Node *temp_pointer = this->first_node;
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
        Node *temp_pointer = this->first_node;
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
        Node *temp_node = this->first_node;

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