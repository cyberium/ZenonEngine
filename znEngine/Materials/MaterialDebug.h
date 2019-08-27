#pragma once

#include "../MaterialWrapper.h"

class OW_ENGINE_API MaterialDebug : public MaterialWrapper
{
public:
	MaterialDebug();
	virtual ~MaterialDebug();

	cvec4 GetDiffuseColor() const;
	void SetDiffuseColor(cvec4 diffuse);

	void UpdateConstantBuffer() const override;

private:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: m_DiffuseColor(1, 1, 1, 1)
		{}
		vec4   m_DiffuseColor;
		//-------------------------- ( 16 bytes )
	};
	MaterialProperties* m_pProperties;
};