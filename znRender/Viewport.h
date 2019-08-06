#pragma once

#include "common.h"

class OW_ENGINE_API Viewport
{
public:
    Viewport(float x = 0.0f, float y = 0.0f, float width = 1280.0f, float height = 1024.0f, float minDepth = 0.0f, float maxDepth = 1.0f);
    virtual ~Viewport();

	const glm::mat4& GetOrthoMatix() const;
    void UpdateOrthoMatrix();

	float GetX() const;
	float GetY() const;
	float GetWidth() const;
	void SetWidth(float Value);
	float GetHeight() const;
	void SetHeight(float Value);
	float GetMinDepth() const;
	float GetMaxDepth() const;

private:
	float X;
	float Y;
	float Width;
	float Height;
	float MinDepth;
	float MaxDepth;

	glm::mat4 OrthoMatrix;
};
