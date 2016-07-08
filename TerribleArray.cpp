#include "TerribleArray.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if blah
template <class type>
TerribleArray<type>::TerribleArray()
{
    //ctor
}

template <class type>
void TerribleArray<type>::add(char* name, type object)
{
    this->names.push_back(name);
    this->objects.push_back(object);
}

template <class type>
type* TerribleArray<type>::get(char* name)
{
    for (int i = 0; i < this->names.size(); i++)
    {
        if (strcmp(this->names[i], name) == 0)
        {
            return &this->objects[i];
        }
    }

    return (type*)0;
}

template <class type>
TerribleArray<type>::~TerribleArray()
{
    //dtor
}
#endif
