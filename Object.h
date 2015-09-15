#ifndef OBJECT_H
#define OBJECT_H
#include "Model.h"

#define ANIMATE_ROTATE 1
#define ANIMATE_TRANSITION 2

class Object
{
    public:
        Object();
        Object(Model* _model, double seconds);
        Object(Model* _model, double seconds, glm::vec3 transition);
        Object(Model* _model, double seconds, glm::vec3 transition, glm::vec3 rot);
        glm::mat4 draw(GLuint shaderprogram, double seconds);
        void init();
        glm::mat4 calculateM(double seconds);
        void reset();

        Model* model;
        double time;
        double pause = 0.0;
        double elapsedPause = 0.0;
        double elapsedTime = 0.0;
        int doReverse = 0;
        int inReverse = 0;
        glm::mat4 start = glm::mat4(1.0);
        glm::mat4 after = glm::mat4(1.0);
        glm::vec3 movePerSec = glm::vec3(0.0);

        glm::vec3 mov = glm::vec3(0.0);
        glm::vec3 rotAxis = glm::vec3(0.0);

        float rotAngle = 0.0;
        float rotPerSec = 0.0;

        glm::mat4 targetMov = glm::mat4(1.0);
        glm::mat4 targetRot = glm::mat4(1.0);
    protected:
    private:
};

#endif // OBJECT_H
