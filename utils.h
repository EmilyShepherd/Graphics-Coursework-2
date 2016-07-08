#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "time.h"

void loadShader(char *file, GLuint type, GLuint program);
unsigned char* loadBMP(const char * imagepath, int* width, int* height);
char* append(const char* dir, const char* path);

/**
 * Cameras and static objects are entities
 */
class Camera
{
    public:
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 up;
        float speed;
        Camera();
        glm::vec3 lookAt;
        glm::mat4 VP();
        glm::mat4 V();
        void moveForward();
        void tour(double seconds);
        double elapsedTime = 0.0;
        void rotate(float angle);
        void init();
        int tourFrame(double seconds, double rotAim, glm::vec3 movAim);
        int tourFrame(double seconds, double rotAim);
        int tourFrame(double seconds, glm::vec3 movAim);
        int tourPause(double seconds);
        double base = 0.0;
};
