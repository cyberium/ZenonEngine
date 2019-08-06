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
	UpdateOrthoMatrix();
}

Viewport::~Viewport()
{
}

const glm::mat4& Viewport::GetOrthoMatix() const
{
	return OrthoMatrix;
}

void Viewport::UpdateOrthoMatrix()
{
    OrthoMatrix = glm::ortho<float>(0.0f, Width, Height, 0.0f, MinDepth - 1.0f, MaxDepth);
}

float Viewport::GetX() const
{
	return X;
}

float Viewport::GetY() const
{
	return Y;
}

float Viewport::GetWidth() const
{
	return Width;
}

void Viewport::SetWidth(float Value)
{
	Width = Value;
	UpdateOrthoMatrix();
}

float Viewport::GetHeight() const
{
	return Height;
}

void Viewport::SetHeight(float Value)
{
	Height = Value;
	UpdateOrthoMatrix();
}

float Viewport::GetMinDepth() const
{
	return MinDepth;
}

float Viewport::GetMaxDepth() const
{
	return MaxDepth;
}
