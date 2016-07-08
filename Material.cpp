#include "Material.h"
#include <string.h>
#include "utils.h"

TerribleArray<Material> items;
TerribleArray<int> loadedTextures;

Material::Material()
{

}

Material::Material(char* path)
{
    textures = (unsigned char**)malloc(sizeof(unsigned char*) * 128);
    int t = 0;

    FILE * file = fopen(path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "[ERROR] Impossible to open material file: %s !\n", path);
        return;
    }

    MaterialType mt;
    mt.inited      = true;
    char name[128] = "";
    inited         = true;

    while (1)
    {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;

        if (strcmp(lineHeader, "newmtl") == 0)
        {
            if (strcmp(name, "") != 0)
            {
                materials.add(name, mt);
            }

            fscanf(file, "%s", name);
            mt.texture = 0;
        }
        else if (strcmp(lineHeader, "Kd") == 0)
        {
            fscanf(file, "%f %f %f", &mt.color[0], &mt.color[1], &mt.color[2]);
        }
        else if (strcmp(lineHeader, "illum") == 0)
        {
            fscanf(file, "%d", &mt.illum);
        }
        else if (strcmp(lineHeader, "map_Kd") == 0)
        {
            char path[128];
            fscanf(file, "%s", (char *)path);

            GLuint textureID = loadedTextures.get(path);

            if (textureID)
            {
                mt.texture = textureID;
            }
            else
            {
                int width, height;

                unsigned char* img = loadBMP(path, &width, &height);

                if (img != 0)
                {
                    textures[t] = img;

                    glGenTextures(1, &textureID);

                    glBindTexture(GL_TEXTURE_2D, textureID);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, textures[t]);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                    glGenerateMipmap(GL_TEXTURE_2D);

                    mt.texture = textureID;
                    loadedTextures.add(path, textureID);
                    t++;
                }
            }
        }

        fscanf(file, "\n");
    }

    if (strcmp(name, "") != 0)
    {
        materials.add(name, mt);
    }

    fclose(file);
}

MaterialType Material::getMaterial(char* name)
{
    return materials.get(name);
}

Material Material::get(char* path)
{
    Material m = items.get(path);

    if (!m.inited)
    {
        m = Material(path);
        items.add(path, m);
    }

    return m;
}
