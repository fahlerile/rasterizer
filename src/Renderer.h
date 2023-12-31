#pragma once
#include "SDL2/SDL.h"
#include "Vector/Vector.h"

// @brief A wrapper around default SDL Window and Renderer structures
typedef struct
{
    SDL_Window* internal_window;
    SDL_Renderer* internal_renderer;
    Vector2i dimensions;
} Renderer;

Renderer* newRenderer(int width, int height, int flags);
void freeRenderer(Renderer* this);

void rendererSetDrawColor(Renderer* this, Color color);
void rendererDrawPixel(Renderer* this, Vector2i point, Color color);

void rendererSaveBuffer(Renderer* this, const char* filename);
void rendererSwapBuffer(Renderer* this);
void rendererClearBuffer(Renderer* this, Color color);

