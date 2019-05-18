#include "stdafx.h"

// General
#include "Viewport.h"


Viewport::Viewport(float x, float y, float width, float height, float minDepth, float maxDepth)
    : X(x)
    , Y(y)
    , Width(width)
    , Height(height)
    , MinDepth(minDepth)
    , MaxDepth(maxDepth)
{
    OrthoMatrix = glm::ortho<float>(0.0f, Width, Height, 0.0f, minDepth - 1.0f, MaxDepth);
}

Viewport::~Viewport()
{
}

void Viewport::UpdateOrthoMatrix()
{
    OrthoMatrix = glm::ortho<float>(0.0f, Width, Height, 0.0f, MinDepth - 1.0f, MaxDepth);
}