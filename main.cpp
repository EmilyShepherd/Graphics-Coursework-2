#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include "utils.h"
#include <vector>
#include "time.h"
#include "Object.h"

GLuint shaderprogram;

Camera camera;

int fps;

int intour = 0;
int active = 1;

Object sunObj;
double worldTime = 00.0;

/**
 * Loads the shaders
 */
void SetupShaders(void)
{
    shaderprogram = glCreateProgram();
    loadShader((char *)"main.vert", GL_VERTEX_SHADER, shaderprogram);
    loadShader((char *)"main.frag", GL_FRAGMENT_SHADER, shaderprogram);

    glBindAttribLocation(shaderprogram, 0, "in_Position");
    glBindAttribLocation(shaderprogram, 1, "in_Normal");
    glBindAttribLocation(shaderprogram, 2, "in_UV");
    glLinkProgram(shaderprogram);
    glUseProgram(shaderprogram);
}

/**
 * Handles key presses
 */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // In the tour we must ignore other keys
    if (intour && key != GLFW_KEY_Q && key != GLFW_KEY_ESCAPE && key != GLFW_KEY_E)
        return;

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
            case GLFW_KEY_LEFT:
                camera.direction = glm::vec3
                (
                      glm::vec4(camera.direction, 0.f)
                    * glm::rotate
                      (
                          glm::mat4(1.f),
                          -1.f,
                          glm::vec3(0.f, 1.f, 0.f)
                      )
                );
                break;

            case GLFW_KEY_RIGHT:
                camera.direction = glm::vec3
                (
                      glm::vec4(camera.direction, 0.f)
                    * glm::rotate
                      (
                          glm::mat4(1.f),
                          1.f,
                          glm::vec3(0.f, 1.f, 0.f)
                      )
                );
                break;

            case GLFW_KEY_UP:
                camera.speed++;
                break;

            case GLFW_KEY_DOWN:
                camera.speed = --camera.speed < 0 ? 0 : camera.speed;
                break;

            case GLFW_KEY_PAGE_UP:
                camera.position += glm::vec3(0.f, 1.f, 0.f);
                break;

            case GLFW_KEY_PAGE_DOWN:
                if (camera.position.y - 1 > 0)
                    camera.position -= glm::vec3(0.f, 1.f, 0.f);
                break;

            case GLFW_KEY_SPACE:
                camera.speed = 0;
                break;

            case GLFW_KEY_ESCAPE:
            case GLFW_KEY_Q:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_P:
                camera.position  = glm::vec3(141.489120, 76.000000, -84.977142);
                camera.direction = glm::vec3(-0.000978, 0.000000, -0.000207);
                break;

            case GLFW_KEY_Y:
                camera.init();
                break;

            case GLFW_KEY_C:
                printf("==========================\n");
                printf
                (
                    "  Camera Position  : %f, %f, %f\n",
                    camera.position[0], camera.position[1], camera.position[2]
                );
                printf
                (
                    "  Camera Direction : %f, %f, %f\n",
                    camera.direction[0], camera.direction[1], camera.direction[2]
                );
                printf("==========================\n");
                break;

            case GLFW_KEY_T:
                intour = 1;
                camera.elapsedTime = 0.0;
                worldTime = 35.0;
                sunObj.reset();
                sunObj.elapsedTime = 35.0;
                break;

            case GLFW_KEY_E:
                intour = 0;
                break;

            case GLFW_KEY_H:
                printf("+--------------------------------------------------+\n");
                printf("| ECS MINING CORP                                  |\n");
                printf("|                                                  |\n");
                printf("| Camera Controls:                                 |\n");
                printf("| ----------------                                 |\n");
                printf("|   UP    - Increase Camera Speed                  |\n");
                printf("|   DOWN  - Decrease Camera Speed                  |\n");
                printf("|   LEFT  - Pan Left                               |\n");
                printf("|   RIGHT - Pan Right                              |\n");
                printf("|   SPACE - Stop All Movement                      |\n");
                printf("|   PAGE UP   - Move Camera Upwards                |\n");
                printf("|   PAGE DOWN - Move Camera Downwards              |\n");
                printf("|                                                  |\n");
                printf("| General Controls:                                |\n");
                printf("| -----------------                                |\n");
                printf("|   T - Start Tour                                 |\n");
                printf("|   E - Exit Tour                                  |\n");
                printf("|                                                  |\n");
                printf("|                                                  |\n");
                printf("+--------------------------------------------------+\n");
                break;
        }
    }
}

/**
 * main
 */
int main(void)
{
    if(!glfwInit())
    {
        fprintf(stderr, "[FATAL] Failed to start GLFW\n");
        exit(EXIT_FAILURE);
    }

    GLFWwindow* window = glfwCreateWindow(600, 600, "COMP3214 - Emily Shepherd", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "[FATAL] Failed to open window\n");
        exit(EXIT_FAILURE);
    }

    /* * * GENERAL INIT * * */
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);

    // Load and compile shaders
    SetupShaders();

    // Load objects and textures from file
    Model place      = Model((char *)"untitled.obj");
    Model skybox     = Model((char *)"sky.obj");
    Model rocks      = Model((char *)"rocks.obj");
    Model door_left  = Model((char *)"door_left.obj");
    Model door_right = Model((char *)"door_right.obj");
    Model crane      = Model((char *)"crane.obj");
    Model cranehook  = Model((char *)"crane_hook.obj");
    Model container  = Model((char *)"container.obj");

    // Start the animation clock
    clock_t lastUpdate = clock();

    // Place objects about the place
    Object rocksObj = Object(&rocks, 5, glm::vec3(0.0, 0.0, -50.0));

    Object mine_door_left     = Object(&door_left, 0.5, glm::vec3(2.0, 0.0, 0.0));
    mine_door_left.doReverse  = 1;
    mine_door_left.pause      = 4;

    Object mine_door_right    = Object(&door_right, 0.5, glm::vec3(-2.0, 0.0, 0.0));
    mine_door_right.doReverse = 1;
    mine_door_right.pause     = 4;

    sunObj.model    = &skybox;
    sunObj.time     = 40.;
    sunObj.start    = glm::rotate(glm::mat4(1.), 45.f, glm::vec3(0., 1., 0.));
    sunObj.rotAxis  = glm::vec3(-1.0, 0.0, 1.0);
    sunObj.rotAngle = 360.;
    sunObj.after    = glm::translate(glm::mat4(1.), glm::vec3(60.,0.,-200.));
    sunObj.init();

    Object crane1 = Object(&crane, 5.);
    crane1.after = glm::translate(glm::mat4(1.), glm::vec3(35.669243, 81.000000, -147.689926));
    crane1.rotAxis = glm::vec3(0.0, 1.0, 0.0);
    crane1.rotAngle = -45.;
    crane1.doReverse = 1;
    crane1.pause = 1.;
    crane1.init();

    Object cranehook_1 = Object(&cranehook, 5., glm::vec3(-40.0, 0.0, 0.0));
    cranehook_1.doReverse = 1;
    cranehook_1.pause = 1;

    Object container_1 = Object(&container, 5, glm::vec3(0.0, 20.0, 0.0));
    container_1.doReverse = 1;
    container_1.pause = 1;

    glm::vec3 ambiantDirection = glm::vec3(0.0f, .0f, -1.0f);

    while(!glfwWindowShouldClose(window))
    {
        double time = (clock() - lastUpdate) / (double)CLOCKS_PER_SEC;
        lastUpdate = clock();

        if (intour)
        {
            camera.tour(time);
        }
        else
        {
            camera.moveForward();
        }

        Model::initFrame(shaderprogram);

        /* * * DRAW * * */
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        worldTime += time;

        double segmentTime;

        while (worldTime >= 10.0 * 4.0)
        {
            worldTime -= 40.0;
        }

        if (worldTime >= 30.0)
        {
            segmentTime = 0.1 * (worldTime - 30.0);
            ambiantDirection = glm::vec3(segmentTime, -1. + segmentTime, segmentTime); //-1.0 0.0
            glUniform1f(glGetUniformLocation(shaderprogram, "ambiance"), 0.1);
        }
        else if (worldTime >= 20.0)
        {
            segmentTime = 0.1 * (worldTime - 20.0);
            ambiantDirection = glm::vec3(-1. + segmentTime, 0. - segmentTime, -1. + segmentTime); //0.0 -1.0
            glUniform1f(glGetUniformLocation(shaderprogram, "ambiance"), 0.1);
        }
        else if (worldTime >= 10.0)
        {
            segmentTime = 0.1 * (worldTime - 10.0);
            ambiantDirection = glm::vec3(0. - segmentTime, 1 - segmentTime, 0. - segmentTime); //1.0 0.0
            glUniform1f(glGetUniformLocation(shaderprogram, "ambiance"), 0.7 - 0.06 * (worldTime - 10));
        }
        else
        {
            segmentTime = 0.1 * (worldTime);
            ambiantDirection = glm::vec3(1. - segmentTime, segmentTime, 1. - segmentTime); // 0.0 0.0
            glUniform1f(glGetUniformLocation(shaderprogram, "ambiance"), 0.1 + 0.06 * (worldTime));
        }

        if (worldTime >= 15.0 || worldTime < 5.0)
        {
            glUniform1i(glGetUniformLocation(shaderprogram, "enableSpots"), 1);
        }
        else
        {
            glUniform1i(glGetUniformLocation(shaderprogram, "enableSpots"), 0);
        }

        glm::vec4 VAD = camera.V() * glm::vec4(ambiantDirection, 0.0);

        glUniform3fv(glGetUniformLocation(shaderprogram, "ambiantDirection"), 1, glm::value_ptr(VAD));

        place.draw(shaderprogram, glm::mat4(1.));

        rocksObj.draw(shaderprogram, time);
        mine_door_left.draw(shaderprogram, time);
        mine_door_right.draw(shaderprogram, time);
        sunObj.draw(shaderprogram, time);
        cranehook_1.after = crane1.draw(shaderprogram, time);
        container_1.after = cranehook_1.draw(shaderprogram, time);
        container_1.draw(shaderprogram, time);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
