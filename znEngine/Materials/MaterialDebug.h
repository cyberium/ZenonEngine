#pragma once

namespace
{
	__declspec(align(16)) struct SMaterialDebugProperties
	{
		SMaterialDebugProperties()
			: DiffuseColor(1.0f, 1.0f, 1.0f, 1.0f)
		{}
		glm::vec4 DiffuseColor;
		//-------------------------- ( 16 bytes )
	};
}

class ZN_API MaterialDebug 
	: public MaterialProxieT<SMaterialDebugProperties>
{
public:
	MaterialDebug(IRenderDevice& RenderDevice);
	virtual ~MaterialDebug();

	glm::vec4 GetDiffuseColor() const;
	void SetDiffuseColor(const glm::vec4& diffuse);
};