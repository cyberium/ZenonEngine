#pragma once

struct Viewport
{
	explicit Viewport(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f, float minDepth = 0.0f, float maxDepth = 1.0f)
		: X(x)
		, Y(y)
		, Width(width)
		, Height(height)
		, MinDepth(minDepth)
		, MaxDepth(maxDepth)
	{
		OrthoMatrix = glm::ortho<float>(0.0f, Width, Height, 0.0f, -1.0f, MaxDepth);
	}

	void UpdateOrthoMatrix()
	{
		OrthoMatrix = glm::ortho<float>(0.0f, Width, Height, 0.0f, -1.0f, MaxDepth);
	}

	float X;
	float Y;
	float Width;
	float Height;
	float MinDepth;
	float MaxDepth;

	mat4 OrthoMatrix;
};
