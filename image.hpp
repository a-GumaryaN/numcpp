#include <iostream>

using namespace std;

class Pixel
{
    int r = -1;
    int g = -1;
    int b = -1;
    int saturation = 0;
};

class Pixel_node
{
public:
    Pixel *reference = nullptr;
    Pixel_node *next = nullptr;
    Pixel_node *prev = nullptr;
    void clean()
    {
        delete this->reference;
    }
};

class Image
{
public:
    int matrix_size[2] = {0, 0};
    Pixel_node *first_Pixel = nullptr;
    Pixel_node *last_Pixel = nullptr;

    void add_row(Pixel *new_row)
    {
        if (this->matrix_size[1] == 0)
        {
            Pixel_node *new_node = new Pixel_node;
            new_node->reference = new_row;
            this->first_Pixel = new_node;
            this->last_Pixel = new_node;
            this->matrix_size[1] += 1;
            return;
        }
        else
        {
            Pixel_node *new_proxy = new Pixel_node;
            new_proxy->reference = new_row;
            new_proxy->prev = this->last_Pixel;
            this->last_Pixel->next = new_proxy;
            this->last_Pixel = new_proxy;
            this->matrix_size[1] += 1;
        }
    }

    void remove(int Pixel_index = -1)
    {
        if (this->matrix_size[1] == 1)
        {
            this->reset_matrix();
            return;
        }
        if (this->matrix_size[1] == 0)
            throw std::invalid_argument("matrix is empty");

        if (Pixel_index >= this->matrix_size[1])
            Pixel_index = Pixel_index % this->matrix_size[1];

        if (Pixel_index < 0)
            Pixel_index = this->matrix_size[1] - ((-1 * Pixel_index) % this->matrix_size[1]);

        if (Pixel_index == 0)
            this->remove_first();

        if (0 < Pixel_index && Pixel_index < this->matrix_size[1])
            this->remove_an_index(Pixel_index);
    }
    void pop()
    {
        Pixel_node *temp_proxy = this->last_Pixel;
        this->last_Pixel = this->last_Pixel->prev;
        this->last_Pixel->next = nullptr;
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
        Pixel_node *temp_proxy = this->first_Pixel;
        this->first_Pixel = this->first_Pixel->next;
        this->first_Pixel->prev = nullptr;
        temp_proxy->clean();
        delete temp_proxy;
        this->matrix_size[1] -= 1;
    }
    void remove_an_index(int Pixel_index = 0)
    {
        if (this->matrix_size[0] == 0 || this->matrix_size[1] == 0)
            throw std::invalid_argument("matrix is empty");

        if (Pixel_index >= this->matrix_size[1])
            throw std::invalid_argument("index is overlap");

        Pixel_node *target_proxy = this->first_Pixel;
        for (int i = 0; i < Pixel_index; i++)
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

        Pixel_node *temp_proxy = this->first_Pixel;
        for (int i = 0; i < y; i++)
        {
            temp_proxy = temp_proxy->next;
        }
    }

    void size()
    {
        cout << "[" << this->matrix_size[0] << "," << this->matrix_size[1] << "]";
    }
};

Image *read(string address = "")
{
    Image *new_image = new Image;
    return new_image;
}