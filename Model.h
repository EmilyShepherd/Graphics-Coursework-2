#ifndef MODEL_H
#define MODEL_H

#include "Material.h"
#include <vector>

/**
 * Vertex for use by shader
 */
typedef struct Vertex
{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat UV[2];
} Vertex;


typedef struct DrawFrame
{
    int points;
    MaterialType mt;
} DrawFrame;

class Model
{
    public:
        Model(char *path);
        void draw(GLuint, glm::mat4 M);
        static void initFrame(GLuint);
    protected:
    private:
        std::vector<Vertex> points;
        unsigned int buffer;
        std::vector<DrawFrame> frames;
};

#endif // MODEL_H
