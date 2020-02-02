#include "stdafx.h"

// General
#include "MaterialTextured.h"

MaterialTextured::MaterialTextured(IRenderDevice& RenderDevice)
	: MaterialProxie(RenderDevice.GetObjectsFactory().CreateMaterial(sizeof(MaterialProperties)))
{
	// Constant buffer
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();
}

MaterialTextured::~MaterialTextured()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

cvec4 MaterialTextured::GetDiffuseColor() const
{
	return m_pProperties->m_DiffuseColor;
}

//-----

void MaterialTextured::SetDiffuseColor(cvec4 diffuse)
{
	m_pProperties->m_DiffuseColor = diffuse;
	MarkConstantBufferDirty();
}

void MaterialTextured::UpdateConstantBuffer() const
{
	MaterialProxie::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}