#pragma once
#include "SDL2/SDL.h"
#include "Utils/Utils.h"

// A wrapper around default SDL Window and Renderer structures
typedef struct
{
    SDL_Window* internal_window;
    SDL_Renderer* internal_renderer;
} Renderer;

// May return NULL on error
// Returned pointer should be `free`d
Renderer* NewRenderer(int width, int height, int flags);

void SetColor(Renderer* this, Color color);

void PutPixel(Renderer* this, int x, int y, Color color);

void ClearBuffer(Renderer* this, Color color);

void SwapBuffer(Renderer* this);

void Update(Renderer* this);
