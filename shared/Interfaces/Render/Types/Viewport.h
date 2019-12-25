#pragma once

class ZN_API Viewport
{
public:
	Viewport()
		: X(0.0f)
		, Y(0.0f)
		, Width(320.0f)
		, Height(240.0f)
		, MinDepth(0.0f)
		, MaxDepth(1.0f)
	{
		UpdateOrthoMatrix();
	}
    Viewport(float x, float y, float width, float height, float minDepth, float maxDepth)
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
		OrthoMatrix = glm::ortho<float>(X, Width, Height, Y, MinDepth - 1.0f, MaxDepth);
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
