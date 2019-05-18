#pragma once

class Viewport
{
public:
    Viewport(float x = 0.0f, float y = 0.0f, float width = 800.0f, float height = 600.0f, float minDepth = 0.0f, float maxDepth = 1.0f);
    virtual ~Viewport();

    void UpdateOrthoMatrix();

	float X;
	float Y;
	float Width;
	float Height;
	float MinDepth;
	float MaxDepth;

	glm::mat4 OrthoMatrix;
};
