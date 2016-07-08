#ifndef TERRIBLEARRAY_H
#define TERRIBLEARRAY_H

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

template <class type>
class TerribleArray
{
    public:
        TerribleArray();
        void add(char* name, type object);
        type get(char*);
        std::vector<int> names;
        std::vector<type> objects;
    protected:
    private:


};

template <class type>
TerribleArray<type>::TerribleArray()
{
    //ctor
}

template <class type>
void TerribleArray<type>::add(char* name, type object)
{
    int hashed_name = _hashString(name);

    this->names.push_back(hashed_name);
    this->objects.push_back(object);
}

template <class type>
type TerribleArray<type>::get(char* name)
{
    int hashed_name = _hashString(name);

    for (unsigned int i = 0; i < names.size(); i++)
    {
        if (names.at(i) == hashed_name)
        {
            return objects.at(i);
        }
    }

    return type();
}

#endif // TERRIBLEARRAY_H
