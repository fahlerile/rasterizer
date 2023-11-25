#include <stdio.h>
#include <string.h>
#include "Model.h"
#include "utils/utils.h"

Model* newModel(const char* filename)
{
    Model* this = xmalloc(sizeof(Model));
    this->vertexPositions = newDynamicArray(100, sizeof(Vector4d));
    this->UVs = newDynamicArray(100, sizeof(Vector2d));
    this->normals = newDynamicArray(100, sizeof(Vector3d));
    modelParseObj(this, filename);
    this->matrices = newDynamicArray(10, sizeof(Matrix4));
    return this;
}

static void modelParseObj(Model* this, const char* filename)
{
    FILE* fp = fopen(filename, "r");
    const int n = 50;
    char* line = xmalloc(n * sizeof(char));
    char lineType[3];

    while ((line = fgets(line, n, fp)) != NULL)
    {
        lineType[0] = '\0';  // to avoid doubling on empty lines
        sscanf(line, "%s", lineType);  // BUG: may be buffer overflow here (only if wrong OBJ-file)
        if (lineType[0] == '\0' || strcmp(lineType, "#") == 0)
            continue;
        else if (strcmp(lineType, "v") == 0)
        {
            double x = NAN, y = NAN, z = NAN, w = NAN;
            sscanf(line, "%s %lf %lf %lf %lf", lineType, &x, &y, &z, &w);
            if (isnan(x) || isnan(y) || isnan(z))
                continue;
            else if (isnan(w))
                w = 1;
            Vector4d position = {x, y, z, w};
            addToDynamicArray(this->vertexPositions, &position);
        }
        else if (strcmp(lineType, "vt") == 0)
        {
            double u = NAN, v = NAN, w = NAN;
            sscanf(line, "%s %lf %lf %lf", lineType, &u, &v, &w);
            if (isnan(u) || isnan(v))
                continue;
            Vector2d UV = {u, v};
            addToDynamicArray(this->UVs, &UV);
        }
        else if (strcmp(lineType, "vn") == 0)
        {
            double x = NAN, y = NAN, z = NAN;
            sscanf(line, "%s %lf %lf %lf", lineType, &x, &y, &z);
            if (isnan(x) || isnan(y) || isnan(z))
                continue;
            Vector3d normal = Vector3dNormalize((Vector3d) {x, y, z});
            addToDynamicArray(this->normals, &normal);
        }
        // else if (strcmp(lineType, "vp"))
        else if (strcmp(lineType, "f") == 0)
            continue;
        // {
        //     char* a = "a";
        //     char* token = strtok_r(line, " ", NULL);
        //     while ((token = strtok_r(NULL, " ", &a)) != NULL)
        //     {
        //         printf("%s ", token);
        //     }
        // }
        // else if (strcmp(lineType, "g"))
        // else if (strcmp(lineType, "o"))
        else
            LOGE("Unknown line type \"%s\" occured during parsing of %s\n", lineType, filename);
    }
    xfree(line);
}

void modelAddInstance(Model* this, Vector3d position, Vector3d rotation, Vector3d scale)
{
    Matrix4 mat = Matrix4ConstructTRS(position, rotation, scale);
    addToDynamicArray(this->matrices, &mat);
}

void freeModel(Model* this)
{
    freeDynamicArray(this->matrices);
    xfree(this);
}
