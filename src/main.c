#define SDL_MAIN_HANDLED
#include <time.h>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "Shaders.h"
#include "Context.h"
#include "Type.h"
#include "log.h"

#include <string.h>
#include <assert.h>

Context context;
Vector2i tileDimensionsArgv;

typedef struct
{
    double position[3];
} Vertex;

void vertexShader(void* pVertex, void* pOutput)
{
    memcpy(pOutput, pVertex, sizeof(double) * 3);
}

int main(int argc, char** argv)
{
    assert(argc == 4);

    constructContext(&context);

    char* tok, *subtok;
    int i, j;

    size_t neededFrames = atoi(argv[1]);

    // argv[2] - triangle data (x1,y1,z1;x2,y2,z2;x3,y3,z3)
    Vertex data[3];
    char* saveptr_semic, *saveptr_comma;
    i = 0;
    tok = strtok_r(argv[2], ";", &saveptr_semic);
    while (tok != NULL)
    {
        j = 0;
        subtok = strtok_r(tok, ",", &saveptr_comma);
        while (subtok != NULL)
        {
            data[i].position[j] = atof(subtok);
            subtok = strtok_r(NULL, ",", &saveptr_comma);
            j++;
        }
        tok = strtok_r(NULL, ";", &saveptr_semic);
        i++;
    }

    // argv[3] - tile size (width,height)
    int tiledims[2];
    i = 0;
    tok = strtok(argv[3], ",");
    while (tok != NULL)
    {
        tiledims[i] = (int) atoi(tok);
        i++;
        tok = strtok(NULL, ",");
    }
    tileDimensionsArgv.x = tiledims[0];
    tileDimensionsArgv.y = tiledims[1];

    // Vertex data[3] = {
    //     {.position = {-0.60, -0.45,  0.0}},
    //     {.position = {-0.30,  0.60,  0.0}},
    //     {.position = { 0.60, -0.45,  0.0}},
    // };
    VertexAttribute attributes[1] = {{
        .nItems = 3,
        .type = TYPE_DOUBLE,
        .offsetBytes = 0
    }};
    VertexBuffer* vb = newVertexBuffer(
        sizeof(Vertex), sizeof(data), data, 1, attributes
    );

    ShaderProgram shaderProgram = {
        .vertexShader = {
            .shader = vertexShader,
            .nBytesPerVertex = sizeof(double) * 3,
            .nAttributes = 1,
            .attributes = attributes,
            .indexOfPositionAttribute = 0
        },
        .geometryShader = {
            .shader = NULL,
        },
        .fragmentShader = {
            .shader = NULL
        }
    };

    size_t frameCount = 0;
    clock_t begin, end;
    double frametimeSec;
    while (context.running)
    {
        begin = clock();

        rendererClearBuffer(context.renderer, (Color) {0, 0, 0, 255});
        drawVertexBuffer(vb, PRIMITIVE_TRIANGLES, 0, 3, &shaderProgram);
        // drawIndexBuffer();

        pollEvents();
        rendererSwapBuffer(context.renderer);

        frameCount++;
        end = clock();
        frametimeSec = (double) (end - begin) / CLOCKS_PER_SEC;
        LOGI(
            "Frametime: %lf s; FPS: %lf; Framecount: %zu\n", 
            frametimeSec, 1 / frametimeSec, frameCount
        );

        if (frameCount == neededFrames)
            break;
    }

    freeVertexBuffer(vb);
    return 0;
}

