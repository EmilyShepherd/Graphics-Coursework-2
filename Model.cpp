#include "Model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils.h"

// Float.h defines these as 0.0!? Stupid
#define MAX_DIST 2000.0
#define MIN_DIST -2000.0
#define LIGHTS 21

extern Camera camera;

float lightPositions[LIGHTS][3] = {};
float spotDirections[LIGHTS][3] = {};

int lights = 0;

void copyArray(glm::vec3 from, float* to)
{
    to[0] = from.x;
    to[1] = from.y;
    to[2] = from.z;
}

void copyString(char* from, char* to)
{
    for (int i = 0; i < 128; i++)
    {
        to[i] = from[i];

        if (to[i] == 0)
        {
            break;
        }
    }
}

Model::Model(char *path)
{
    FILE * file = fopen(path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "[ERROR] Impossible to open object file: '%s'!\n", path);
        return;
    }

    // For some reason everything crashes if this isn't a vec3.
    std::vector< glm::vec3 > temp_normals;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_vertices;

    char currentMaterial[128];
    Material m;
    MaterialType mt;
    int position = 0;

    float lowX = MAX_DIST;
    float lowY = MAX_DIST;
    float lowZ = MAX_DIST;
    float highX = MIN_DIST;
    float highY = MIN_DIST;
    float highZ = MIN_DIST;
    glm::vec3 lastNormal;

    while (1)
    {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;

        if (strcmp(lineHeader, "mtllib") == 0)
        {
            char material_path[128];
            fscanf(file, "%s", material_path);

            m = Material(material_path);
        }
        else if (strcmp(lineHeader, "o") == 0)
        {
            if (mt.illum == 4)
            {
                if (lights == LIGHTS)
                {
                    fprintf(stderr, "[WARN] Found more lights than have space for!\n");
                }
                else
                {
                    lightPositions[lights][0] = lowX + (highX - lowX) / 2;
                    lightPositions[lights][1] = lowY + (highY - lowY) / 2;
                    lightPositions[lights][2] = lowZ + (highZ - lowZ) / 2;

                    spotDirections[lights][0] = lastNormal.x;
                    spotDirections[lights][1] = lastNormal.y;
                    spotDirections[lights][2] = lastNormal.z;

                    lights++;
                }
            }

            lowX = MAX_DIST;
            lowY = MAX_DIST;
            lowZ = MAX_DIST;
            highX = MIN_DIST;
            highY = MIN_DIST;
            highZ = MIN_DIST;
        }
        else if (strcmp(lineHeader, "usemtl") == 0)
        {
            char material_name[128];
            fscanf(file, "%s", material_name);

            if (strcmp(currentMaterial, "") == 0)
            {
                copyString(material_name, currentMaterial);
                mt = m.getMaterial(currentMaterial);
            }
            else if (strcmp(material_name, currentMaterial) != 0)
            {
                copyString(material_name, currentMaterial);

                DrawFrame df;
                df.points = position;
                df.mt = mt;
                frames.push_back(df);

                mt = m.getMaterial(material_name);;
                position = 0;
            }

            if (!mt.inited)
            {
                fprintf(stderr, "[ERROR] Material '%s' not found\n", material_name);
            }
        }
        else if (strcmp(lineHeader, "v") == 0)
        {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0)
        {
            glm::vec2 uv;
            fscanf(file, "%f %f", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0)
        {
            glm::vec3 normal;
            fscanf(file, "%f %f %f", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0)
        {
            position += 3;
            for (int i = 0; i < 3; i++)
            {
                int vertexIndex, uvIndex, normalIndex;

                fscanf(file, "%d/", &vertexIndex);

                if (!fscanf(file, "%d/", &uvIndex))
                {
                    fscanf(file, "/%d", &normalIndex);
                    uvIndex = -1;
                }
                else
                {
                    fscanf(file, "%d", &normalIndex);
                }

                glm::vec3 position = temp_vertices[vertexIndex - 1];
                lastNormal         = temp_normals[normalIndex - 1];
                if (position.x > highX) highX = position.x;
                if (position.x < lowX)  lowX  = position.x;
                if (position.y > highY) highY = position.y;
                if (position.y < lowY)  lowY  = position.y;
                if (position.z > highZ) highZ = position.z;
                if (position.z < lowZ)  lowZ  = position.z;

                Vertex v;
                copyArray(position,   &v.position[0]);
                copyArray(lastNormal, &v.normal[0]);

                if (uvIndex >= 0)
                {
                    v.UV[0] = temp_uvs[uvIndex - 1].x;
                    v.UV[1] = temp_uvs[uvIndex - 1].y;
                }

                points.push_back(v);
            }
        }

        // Skip to end of line
        fscanf(file, "\n");
    }

    DrawFrame df;
    df.mt = mt;
    df.points = position;
    frames.push_back(df);

    if (mt.illum == 4)
    {
        if (lights == LIGHTS)
        {
            printf("[WARN] Found more lights than have space for!\n");
        }
        else
        {
            lightPositions[lights][0] = lowX + (highX - lowX) / 2;
            lightPositions[lights][1] = lowY + (highY - lowY) / 2;
            lightPositions[lights][2] = lowZ + (highZ - lowZ) / 2;

            spotDirections[lights][0] = lastNormal.x;
            spotDirections[lights][1] = lastNormal.y;
            spotDirections[lights][2] = lastNormal.z;

            lights++;
        }
    }

    fclose(file);

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(struct Vertex), &points[0], GL_STATIC_DRAW);
}

void Model::initFrame(GLuint shaderprogram)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "V"), 1, GL_FALSE, glm::value_ptr(camera.V()));

    float distances[6] = {1000, 1000, 1000, 1000, 1000, 1000};
    int nearest[6] = {0, 0, 0, 0, 0, 0};
    int selected = 0;
    int inFOV[LIGHTS];
    float distCache[LIGHTS];

    for (int i = 0; i < lights; i++)
    {
        glm::vec3 position_worldSpace = glm::vec3(lightPositions[i][0], lightPositions[i][1], lightPositions[i][2]);
        glm::vec3 position_cameraSpace = glm::vec3(camera.V() * glm::vec4(position_worldSpace, 1.));

        float distance = glm::length(position_worldSpace - camera.position);
        float cosX = glm::dot(glm::vec3(0., 0., 1.),  glm::normalize(glm::vec3(0.,position_cameraSpace.y, 0.) - position_cameraSpace));
        float cosY = glm::dot(glm::vec3(0., 0., 1.),  glm::normalize(glm::vec3(position_cameraSpace.x, 0., 0.) - position_cameraSpace));

        if (glm::min(cosX, cosY) > 0.64278760968 || distance < 70.)
        {
            distCache[selected] = distance;
            inFOV[selected++] = i;
        }
    }

    for (int j = 0; j < selected; j++)
    {
        int i = inFOV[j];
        float distance = distCache[j];

        if (distance < distances[5])
        {
            distances[5] = distance;
            nearest[5]   = i;

            for (int d = 4; d >= 0; d--)
            {
                if (distances[d + 1] < distances[d])
                {
                    float temp       = distances[d];
                    distances[d]     = distances[d + 1];
                    distances[d + 1] = temp;

                    int temp2      = nearest[d];
                    nearest[d]     = nearest[d + 1];
                    nearest[d + 1] = temp2;
                }
                else
                {
                    break;
                }
            }
        }
    }

    glUniform1i(glGetUniformLocation(shaderprogram, "lightCount"), glm::min(6, selected));

    for (int i = 0; i < glm::min(6, selected); i++)
    {
        int d = nearest[i];
        char pointer[100];

        sprintf(pointer, "LightPosition_worldspace[%d]", i);
        glUniform3fv(glGetUniformLocation(shaderprogram, pointer), 1, (float*)lightPositions[d]);

        sprintf(pointer, "SpotDirection_worldspace[%d]", i);
        glUniform3fv(glGetUniformLocation(shaderprogram, pointer), 1, (float*)spotDirections[d]);
    }
}

void Model::draw(GLuint shaderprogram, glm::mat4 M)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(struct Vertex), &points[0], GL_STATIC_DRAW);

    glVertexAttribPointer ( ( GLuint ) 0, 3, GL_FLOAT, GL_FALSE,  sizeof ( struct Vertex ), ( const GLvoid* ) offsetof (struct Vertex, position) );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer ( ( GLuint ) 1, 3, GL_FLOAT, GL_TRUE, sizeof ( struct Vertex ), ( const GLvoid* ) offsetof(struct Vertex, normal) );
    glEnableVertexAttribArray ( 1 );

    glVertexAttribPointer ( ( GLuint ) 2, 2, GL_FLOAT, GL_FALSE, sizeof ( struct Vertex ), ( const GLvoid* ) offsetof(struct Vertex, UV) );
    glEnableVertexAttribArray ( 2 );

    glm::mat4 MVP = camera.VP() * M;

    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "M"), 1, GL_FALSE, glm::value_ptr(M));
    glUniform1i(glGetUniformLocation(shaderprogram, "texture"), 0);

    int start = 0;

    for (unsigned int i = 0; i < frames.size(); i++)
    {
        DrawFrame frame = frames.at(i);
        glUniform3fv(glGetUniformLocation(shaderprogram, "color"), 1, frame.mt.color);
        glUniform1i(glGetUniformLocation(shaderprogram, "illum"), frame.mt.illum);

        if (frame.mt.texture)
        {
            glBindTexture(GL_TEXTURE_2D, frame.mt.texture);
            glUniform1i(glGetUniformLocation(shaderprogram, "doTexture"), 1);
        }
        else
        {
            glUniform1i(glGetUniformLocation(shaderprogram, "doTexture"), 0);
        }

        glDrawArrays(GL_TRIANGLES, start, frame.points);
        start += frame.points;
    }
}
