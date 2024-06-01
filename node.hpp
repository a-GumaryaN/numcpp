#include <iostream>
#include <cmath>

template <typename Value_type>
class Node
{
public:
    Value_type value;
    Node *next = 0;
    Node *prev = 0;
};