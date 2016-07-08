#ifndef MATERIAL_H
#define MATERIAL_H
#include "TerribleArray.h"
#include <glm/glm.hpp>
#include <GL/glew.h>

typedef struct MaterialType
{
    float color[3];
    GLuint texture;
    int illum = 2;
    bool inited = false;
} MaterialType;

class Material
{
    public:
        Material();
        Material(char* name);
        MaterialType getMaterial(char* name);
        static Material get(char* path);
        TerribleArray<MaterialType> materials;
        bool inited = false;
    protected:
    private:
        unsigned char** textures;
};


//class

#endif // MATERIAL_H
