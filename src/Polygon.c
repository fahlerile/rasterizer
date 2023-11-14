#include <float.h>
#include "Polygon.h"
#include "Utils/NDC.h"
#include "Utils/Array.h"
#include "Utils/Utils.h"

Polygon* newPolygon(Vector3d* vertices, Color* colors, Renderer* renderer, size_t n)
{
    assert(n >= 3);

    Polygon* this = allocate(sizeof(Polygon));

    this->vertices = vertices;
    for (size_t i = 0; i < n; i++)
        this->vertices[i] = NDCtoScreenSpace(renderer, vertices[i]);

    this->edgeVectors = allocate(n * sizeof(Vector3d));
    this->colors = colors;
    this->n = n;

    // Calculate `edgeVectors`
    for (size_t i = 0; i < n; i++)
        this->edgeVectors[i] = Vector3dSubtract(vertices[(i == n-1) ? 0 : i+1], vertices[i]);

    // Calculate `areaX2`
    if (n == 3)
    {
        this->areaX2 = Vector3dMagnitude(
            Vector3dCross(this->edgeVectors[0], Vector3dNegate(this->edgeVectors[2]))
        );
    }
    else  // NOT TESTED!
    {
        this->areaX2 = 0;
        for (size_t i = 2; i < n; i++)
        {
            double areaOfThisSubtriangleX2 = Vector3dMagnitude(Vector3dCross(
                Vector3dSubtract(this->vertices[0], this->vertices[i]),
                Vector3dNegate(this->edgeVectors[i-1])
            ));
            this->areaX2 += areaOfThisSubtriangleX2;
        }
    }

    return this;
}

void freePolygon(Polygon* this)
{
    free(this->edgeVectors);
    free(this);
}

void getBoundingPointsPolygon(Polygon* this, Vector3d* min, Vector3d* max)
{
    Vector3d res_min = {DBL_MAX, DBL_MAX, DBL_MAX};
    Vector3d res_max = {0};

    for (size_t i = 0; i < this->n; i++)
    {
        if (this->vertices[i].x < res_min.x)
            res_min.x = this->vertices[i].x;
        if (this->vertices[i].y < res_min.y)
            res_min.y = this->vertices[i].y;
        if (this->vertices[i].z < res_min.z)
            res_min.z = this->vertices[i].z;

        if (this->vertices[i].x > res_max.x)
            res_max.x = this->vertices[i].x;
        if (this->vertices[i].y > res_max.y)
            res_max.y = this->vertices[i].y;
        if (this->vertices[i].z > res_max.z)
            res_max.z = this->vertices[i].z;
    }

    *min = res_min;
    *max = res_max;
}

void calculateBarycentricCoordinatesX2Polygon(Polygon* this, Vector3d point, double* barycentricCoordinatesX2)
{
    for (size_t i = 0; i < this->n; i++)
    {
        Vector3d vertexToPoint = Vector3dSubtract(point, this->vertices[i]);
        barycentricCoordinatesX2[i] = Vector3dMagnitude(Vector3dCross(vertexToPoint, this->edgeVectors[i]));
    }
}