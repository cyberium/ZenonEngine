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

const glm::vec4& MaterialTextured::GetDiffuseColor() const
{
	return m_pProperties->DiffuseColor;
}

//-----

void MaterialTextured::SetDiffuseColor(const glm::vec4& diffuse)
{
	m_pProperties->DiffuseColor = diffuse;
	MarkConstantBufferDirty();
}

void MaterialTextured::UpdateConstantBuffer() const
{
	MaterialProxie::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}