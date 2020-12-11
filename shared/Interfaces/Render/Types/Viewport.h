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
	Viewport(uint32 width, uint32 height)
		: X(0)
		, Y(0)
		, Width(width)
		, Height(height)
		, MinDepth(0.0f)
		, MaxDepth(1.0f)
	{
		UpdateOrthoMatrix();
	}
    Viewport(uint32 x, uint32 y, uint32 width, uint32 height, float minDepth, float maxDepth)
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

	uint32 GetX() const
	{
		return X;
	}
	uint32 GetY() const
	{
		return Y;
	}
	uint32 GetWidth() const
	{
		return Width;
	}
	void SetWidth(uint32 Value)
	{
		Width = Value;
		UpdateOrthoMatrix();
	}
	uint32 GetHeight() const
	{
		return Height;
	}
	void SetHeight(uint32 Value)
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

	glm::ivec2 GetSize() const 
	{
		return glm::ivec2(Width, Height);
	}

	glm::ivec4 GetGLMViewport() const
	{
		return glm::vec4(X, Y, Width, Height);
	}

private:
	uint32 X;
	uint32 Y;
	uint32 Width;
	uint32 Height;
	float MinDepth;
	float MaxDepth;

	glm::mat4 OrthoMatrix;
};
