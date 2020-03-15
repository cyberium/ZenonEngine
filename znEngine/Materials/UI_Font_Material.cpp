#include "stdafx.h"

// General
#include "UI_Font_Material.h"

UI_Font_Material::UI_Font_Material(IRenderDevice& RenderDevice) :
	MaterialProxie(RenderDevice.GetObjectsFactory().CreateMaterial(sizeof(MaterialProperties)))
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();
}

UI_Font_Material::~UI_Font_Material()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}



//
// UI_Font_Material
//
void UI_Font_Material::SetColor(glm::vec4 color)
{
	m_pProperties->Color = color;
	MarkConstantBufferDirty();
}


void UI_Font_Material::SetOffset(glm::vec2 offset)
{
	m_pProperties->Offset = offset;
	MarkConstantBufferDirty();
}

void UI_Font_Material::UpdateConstantBuffer() const
{
	MaterialProxie::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}

void UI_Font_Material::SetSelected(bool color)
{
	m_pProperties->IsSelected = color;
	MarkConstantBufferDirty();
}