#include "stdafx.h"

// General
#include "MaterialParticle.h"

MaterialParticle::MaterialParticle(IRenderDevice& RenderDevice)
	: MaterialProxie(RenderDevice.GetObjectsFactory().CreateMaterial(sizeof(MaterialProperties)))
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();
}

MaterialParticle::~MaterialParticle()
{
	_aligned_free(m_pProperties);
	m_pProperties = nullptr;
}

cvec4 MaterialParticle::GetDiffuseColor() const
{
	return m_pProperties->DiffuseColor;
}

void MaterialParticle::SetDiffuseColor(cvec4 diffuse)
{
	m_pProperties->DiffuseColor = diffuse;
	MarkConstantBufferDirty();
}

void MaterialParticle::UpdateConstantBuffer() const
{
	MaterialProxie::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}
