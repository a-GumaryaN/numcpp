#include <iostream>

using namespace std;

class Node
{
public:
    double value = 0;
    Node *next = 0;
    Node *prev = 0;
};

class Array
{
public:
    int size = 0;
    Node *first_node = nullptr;
    Node *last_node = nullptr;

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

    double avg()
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
};

class Proxy_node
{
public:
    Array *value = nullptr;
    Proxy_node *next = nullptr;
    Proxy_node *prev = nullptr;
};

class Matrix
{
public:
    int size = 0;
    Proxy_node *first_array = nullptr;
    Proxy_node *last_array = nullptr;

    void add_row(Array *new_row)
    {
        // create new array node
        Proxy_node *new_node = new Proxy_node;
        new_node->value = new_row;
        if (this->first_array == nullptr)
        {
            this->first_array = new_node;
            this->last_array = new_node;
        }
        else
        {
            new_node->prev = this->last_array;
            this->last_array->next = new_node;
            this->last_array = new_node;
        }
        this->size++;
    }

    void remove_row(int array_index = 0)
    {

        if (array_index > this->size)
            throw std::invalid_argument("index is overlap");

        Proxy_node *temp = this->first_array;
        for (int i = 0; i < array_index; i++)
        {
            temp = temp->next;
        }

        if (this->first_array == temp)
            this->first_array = temp->next;

        if (this->last_array == temp)
            this->last_array = temp->prev;

        if (temp->prev != nullptr)
            temp->prev->next = temp->next;

        if (temp->next != nullptr)
            temp->next->prev = temp->prev;

        delete temp;
    }
};

int main()
{

    Array array1;
    array1.push(12.0);
    array1.push(13.0);
    array1.push(14.0);
    array1.push(15.0);

    array1.add(3.5,-2);

    array1.show();

    // Array array2 ;
    // array2.push(10.0);
    // array2.push(11.0);
    // array2.push(12.0);
    // array2.push(13.0);
    // array2.push(14.0);
    // array2.push(15.0);

    // array2.remove(0);
    // array2.remove(0);
    // array2.remove(0);

    // Array *array2 = new Array;
    // array2->push(10.0);
    // array2->push(11.0);
    // array2->push(12.0);
    // array2->push(13.0);
    // array2->push(14.0);
    // array2->push(15.0);

    // Matrix *mat1 = new Matrix();
    // mat1->add_row(array1);
    // mat1->add_row(array2);

    // mat1->remove_row(0);

    // cout << mat1->first_array->value->get(0);

    return 1;
}