#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils.h"

extern GLuint shaderprogram;
extern Camera camera;
extern int intour;

glm::mat4 P = glm::perspective(100.0f, 1.0f, 0.01f, 1000.0f);

/**
 * Sets up default values on the entity
 */
Camera::Camera()
{
    position  = glm::vec3(0.f);
    direction = glm::vec3(0.f, 0.f, 0.01f);
    up        = glm::vec3(0.f, 1.f, 0.f);
    speed     = 0;
    init();
}

void Camera::init()
{
    position  = glm::vec3(7.5, 17, -46.4);
    direction = glm::vec3(0.000883f, 0.f, 0.000469);
}

/**
 * Moves the camera forward with its current speed
 */
void Camera::moveForward( void )
{
    position += glm::vec3(speed * 2) * direction;
    lookAt    = position + direction;
};

/**
 * Returns the View with the Perspective
 */
glm::mat4 Camera::VP()
{
    return P * V();
}

glm::mat4 Camera::V()
{
    lookAt    = position + direction;
    return glm::lookAt(position, lookAt, up);
}

void Camera::rotate(float angle)
{
    direction = glm::vec3
    (
          glm::vec4(direction, 0.f)
        * glm::rotate
          (
              glm::mat4(1.f),
              angle,
              glm::vec3(0.f, 1.f, 0.f)
          )
    );
}

int Camera::tourFrame(double duration, double rotAim, glm::vec3 movAim)
{
    base += duration;

    if (elapsedTime < base)
    {
        double frameTime = (elapsedTime - base + duration) / duration;

        position += glm::vec3(frameTime) * movAim;
        rotate(frameTime * rotAim);
        return 0;
    }
    else
    {
        position += movAim;
        rotate(rotAim);
        return 1;
    }
}

int Camera::tourPause(double duration)
{
    base += duration;

    return (elapsedTime >= base);
}

int Camera::tourFrame(double duration, double rotAim)
{
    return tourFrame(duration, rotAim, glm::vec3(0.));
}

int Camera::tourFrame(double duration, glm::vec3 movAim)
{
    return tourFrame(duration, 0.0, movAim);
}

void Camera::tour(double seconds)
{
    elapsedTime += (double)seconds;

    position  = glm::vec3(40.961548, 11.000000, -8.952918);
    direction = glm::vec3(-0.000857, 0.000000, 0.000515);
    base      = 0.0;

    tourPause(3.) &&
    tourFrame(5., 90.) &&
    tourFrame(5., -90., glm::vec3(-9., 6., -40.)) &&
    tourPause(2.) &&
    tourFrame(10., 120., glm::vec3(0., 0., 20.)) &&
    tourPause(.5) &&
    tourFrame(7, 10., glm::vec3(7., -3., -45.)) &&
    tourFrame(3., -90., glm::vec3(-2, 0, -10)) &&
    tourPause(3.) &&
    tourFrame(2., 140., glm::vec3(6, 1., -5)) &&
    tourFrame(2., glm::vec3(10., 0., -10.)) &&
    tourFrame(2., -60., glm::vec3(2., 1., -10.)) &&
    tourPause(2.) &&
    tourFrame(2., 60., glm::vec3(20., 10., -20.)) &&
    tourFrame(3., glm::vec3(40., 0., -20.)) &&
    tourFrame(3., 10., glm::vec3(60., -60., 0.)) &&
    tourFrame(2., -70.) &&
    tourFrame(2., 10., glm::vec3(30., 35., -80.)) &&
    tourFrame(4.5, 210., glm::vec3(10., 30., 0.)) &&
    tourFrame(4., 80., glm::vec3(5., 20., 80.)) &&
    tourFrame(2., -20., glm::vec3(-10., 10., 10.)) &&
    tourFrame(4., glm::vec3(-130., 10., 10.)) &&
    tourFrame(9., 100., glm::vec3(-130., 0., 0.)) &&
    tourFrame(8., 50., glm::vec3(-50., -40., -20.)) &&
    tourFrame(4., 35., glm::vec3(50., -10., 30.)) &&
    tourFrame(3., -90, glm::vec3(-5., 45., -60.)) &&
    tourFrame(1., -30, glm::vec3(0., 0., -10.)) &&
    tourFrame(2.5, glm::vec3(0., -40., -50.)) &&
    tourPause(1.) &&
    tourFrame(4., -50., glm::vec3(-55., 50., -25.)) &&
    tourFrame(4., 80., glm::vec3(-30., 50., -20.)) &&
    tourFrame(4., 120., glm::vec3(0., 10., -50.)) &&
    tourFrame(4., 80., glm::vec3(35., -5., -10.)) &&
    tourPause(2.) &&
    tourFrame(10., -80) &&
    (intour = 0);
}

