#pragma once

class OW_ENGINE_API Viewport
{
public:
    Viewport(float x = 0.0f, float y = 0.0f, float width = 1280.0f, float height = 1024.0f, float minDepth = 0.0f, float maxDepth = 1.0f)
		: X(x)
		, Y(y)
		, Width(width)
		, Height(height)
		, MinDepth(minDepth)
		, MaxDepth(maxDepth)
	{
		UpdateOrthoMatrix();
	}
    virtual ~Viewport()
	{}

	const glm::mat4& GetOrthoMatix() const
	{
		return OrthoMatrix;
	}
    void UpdateOrthoMatrix()
	{
		OrthoMatrix = glm::ortho<float>(0.0f, Width, Height, 0.0f, MinDepth - 1.0f, MaxDepth);
	}

	float GetX() const
	{
		return X;
	}
	float GetY() const
	{
		return Y;
	}
	float GetWidth() const
	{
		return Width;
	}
	void SetWidth(float Value)
	{
		Width = Value;
		UpdateOrthoMatrix();
	}
	float GetHeight() const
	{
		return Height;
	}
	void SetHeight(float Value)
	{
		Height = Value;
		UpdateOrthoMatrix();
	}
	float GetMinDepth() const
	{
		return MinDepth;
	}
	float GetMaxDepth() const
	{
		return MaxDepth;
	}

private:
	float X;
	float Y;
	float Width;
	float Height;
	float MinDepth;
	float MaxDepth;

	glm::mat4 OrthoMatrix;
};
