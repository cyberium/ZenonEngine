#pragma once

class ZN_API Viewport
{
public:
	Viewport()
		: X(0)
		, Y(0)
		, Width(1280)
		, Height(1024)
		, MinDepth(0.0f)
		, MaxDepth(1.0f)
	{
		UpdateOrthoMatrix();
	}
    Viewport(size_t x, size_t y, size_t width, size_t height, float minDepth, float maxDepth)
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
		OrthoMatrix = glm::ortho<float>(static_cast<float>(X), static_cast<float>(Width), static_cast<float>(Height), static_cast<float>(Y), MinDepth - 1.0f, MaxDepth);
	}

	size_t GetX() const
	{
		return X;
	}
	size_t GetY() const
	{
		return Y;
	}
	size_t GetWidth() const
	{
		return Width;
	}
	void SetWidth(size_t Value)
	{
		Width = Value;
		UpdateOrthoMatrix();
	}
	size_t GetHeight() const
	{
		return Height;
	}
	void SetHeight(size_t Value)
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
	size_t X;
	size_t Y;
	size_t Width;
	size_t Height;
	float MinDepth;
	float MaxDepth;

	glm::mat4 OrthoMatrix;
};
