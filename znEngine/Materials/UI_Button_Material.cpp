#include "stdafx.h"

// General
#include "UI_Button_Material.h"

UI_Button_Material::UI_Button_Material(IRenderDevice& RenderDevice) :
	MaterialProxie(RenderDevice.GetObjectsFactory().CreateMaterial(sizeof(MaterialProperties)))
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();
}

UI_Button_Material::~UI_Button_Material()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

void UI_Button_Material::SetIdleTexture(std::shared_ptr<ITexture> _texture)
{
	base::SetTexture(0, _texture);
	MarkConstantBufferDirty();
}

void UI_Button_Material::SetHoverTexture(std::shared_ptr<ITexture> _texture)
{
	base::SetTexture(1, _texture);
	MarkConstantBufferDirty();
}

void UI_Button_Material::SetClickedTexture(std::shared_ptr<ITexture> _texture)
{
	base::SetTexture(2, _texture);
	MarkConstantBufferDirty();
}

void UI_Button_Material::SetDisabledTexture(std::shared_ptr<ITexture> _texture)
{
	base::SetTexture(3, _texture);
	MarkConstantBufferDirty();
}

void UI_Button_Material::SetColor(glm::vec4 color)
{
	m_pProperties->Color = color;
	MarkConstantBufferDirty();
}

void UI_Button_Material::SetState(uint32 state)
{
	m_pProperties->State = state;
	MarkConstantBufferDirty();
}


void UI_Button_Material::UpdateConstantBuffer() const
{
	base::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}
