#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>

Object::Object() {}

Object::Object(Model* _model, double seconds)
{
    model = _model;
    time  = seconds;
}

Object::Object(Model* _model, double seconds, glm::vec3 _mov)
{
    model = _model;
    time  = seconds;
    mov   = _mov;

    init();
}

void Object::init()
{
    targetMov = glm::translate(start, mov);

    if (rotAngle)
    {
        targetMov = glm::rotate(targetMov, rotAngle, rotAxis);
    }

    movePerSec.x = mov.x / time;
    movePerSec.y = mov.y / time;
    movePerSec.z = mov.z / time;

    rotPerSec = rotAngle / time;
}

glm::mat4 Object::draw(GLuint shaderprogram, double seconds)
{
    glm::mat4 M = calculateM(seconds);
    model->draw(shaderprogram, M);

    return M;
}
void Object::reset()
{
    elapsedTime = 0.0;
}

glm::mat4 Object::calculateM(double seconds)
{
    double totalTime = time * (doReverse ? 2 : 1) + pause;
    glm::vec3 mov;
    float angle;
    glm::mat4 base;

    elapsedTime += seconds;

    while (elapsedTime >= totalTime)
    {
        elapsedTime -= totalTime;
    }

    if (elapsedTime >= totalTime - pause)
    {
        return after * start;
    }
    else if (elapsedTime > time)
    {
        double t = -1 * (elapsedTime - time);

        mov.x =  movePerSec.x * t;
        mov.y =  movePerSec.y * t;
        mov.z =  movePerSec.z * t;

        angle = rotPerSec * t;

        base = targetMov;
    }
    else
    {
        mov.x =  movePerSec.x * elapsedTime;
        mov.y =  movePerSec.y * elapsedTime;
        mov.z =  movePerSec.z * elapsedTime;

        angle = rotPerSec * elapsedTime;

        base = start;
    }

    glm::mat4 M = glm::translate(after, mov);

    if (angle)
    {
        M = glm::rotate(M, glm::radians(angle), rotAxis);
    }

    return M * base;
}
