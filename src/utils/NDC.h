#pragma once
#include "Vectors/Vector3.h"
#include "Renderer.h"

// Converts NDC point to screen space. Z coordinate is ignored
Vector3d NDCtoScreenSpace(Renderer* renderer, Vector3d NDC);
