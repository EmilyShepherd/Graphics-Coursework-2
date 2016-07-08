/**
 * General utilities for Object / path / shader loading
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>


/**
 * Read a file (for the shader loader)
 */
void loadShader(char *file, GLuint type, GLuint program)
{
    FILE *fptr;
    long length;
    char *buf;

    fptr = fopen(file, "r");
    if (!fptr)
    {
        fprintf(stderr, "[FATAL] Shader '%s' not found\n", file);
        exit(EXIT_FAILURE);
    }

    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf = (char*)malloc(length + 1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&buf, 0);
    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status)
    {
        glAttachShader(program, shader);
    }
    else
    {
        glGetShaderInfoLog(shader, length, NULL, buf);
        printf("[ERROR] Shader Compilation Failure For: %s\n", file);
        printf("%s", buf);
    }

    free(buf);
}

unsigned char* loadBMP(const char * imagepath, int* width, int* height)
{
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned char * data;

	FILE * file = fopen(imagepath, "rb");
	if (!file)
    {
        fprintf(stderr, "[ERROR] Texture '%s' was not found\n", imagepath);
        *width = 0;
        *height = 0;
        return 0;
    }

	if
    (
        fread(header, 1, 54, file) != 54  ||
        header[0]                  != 'B' ||
        header[1]                  != 'M' ||
        *(int*)&(header[0x1E])     != 0   ||
        *(int*)&(header[0x1C])     != 24
    )
    {
		fprintf(stderr, "[ERROR] Texture '%s' is not a BMP file\n", imagepath);
		*width = 0;
        *height = 0;
        return 0;
	}

	// Read the information about the image
	dataPos   = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	*width    = *(int*)&(header[0x12]);
	*height   = *(int*)&(header[0x16]);

	if (imageSize == 0) imageSize = *width * *height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)   dataPos   = 54; // The BMP header is done that way

    // Read the data
    data = (unsigned char*)malloc(sizeof(unsigned char) * imageSize);
	//data = new unsigned char [imageSize];
	fread(data, 1, imageSize, file);
	fclose (file);

	return data;
}

char* append(const char* dir, const char* path)
{
    char* joined = (char*)malloc
    (
        sizeof(char*) * (strlen(dir) + strlen(path))
    );
    strcpy(joined, dir);
    strcat(joined, path);

    return joined;
}

int _hashString(char* str)
{
    unsigned int value = 0;

    while (1)
    {
        value += (unsigned int)*str;
        str++;

        if (*str == 0)
            break;
    }

    return value;
}
