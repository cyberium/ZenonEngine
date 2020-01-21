#include "stdafx.h"

// General
#include "UITexture.h"

namespace
{
	const vec4 cDefaultColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

CUITextureNode::CUITextureNode(IRenderDevice* RenderDevice, vec2 Size)
	: m_Size(Size)
{
	m_Material = std::make_shared<UI_Texture_Material>(RenderDevice);
	m_Material->SetWrapper(m_Material.get());
	m_Material->SetColor(cDefaultColor);

	m_Mesh = RenderDevice->GetPrimitiveCollection()->CreateUIQuad(Size.x, Size.y);
	m_Mesh->SetMaterial(m_Material);
}

CUITextureNode::~CUITextureNode()
{}



//
// CUITextureNode
//
void CUITextureNode::SetTexture(ITexture* _texture)
{
	m_Material->SetTexture(_texture);
}

void CUITextureNode::SetColor(vec4 _color)
{
	m_Material->SetColor(_color);
}




//
// CUIBaseNode
//
glm::vec2 CUITextureNode::GetSize()
{
    return m_Size;
}

bool CUITextureNode::AcceptMesh(IVisitor* visitor)
{


	return m_Mesh->Accept(visitor);
}
