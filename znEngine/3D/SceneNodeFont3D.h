#pragma once

#include "SceneNode3D.h"

#include "../UI/Fonts/Font.h"
#include "../UI/Materials/UI_Font_Material.h"

class CSceneNodeFont3D : public SceneNode3D
{
public:

	CSceneNodeFont3D();
	virtual ~CSceneNodeFont3D();

	// CUITextNode
	void                        SetFont(std::shared_ptr<CFontMesh> _font);
	void                        SetText(const std::string& _text);
	void                        SetColor(cvec4 _color);


	virtual bool Accept(IVisitor& visitor) override;

private:

	std::shared_ptr<CFontMesh>         m_Font;
	std::shared_ptr<UI_Font_Material>  m_Material;
	std::string                        m_Text;
};