#pragma once

#include "UIText.h"

class CRichTextEditorForTC
	: public CUIBaseNode
{
public:
	CRichTextEditorForTC();
	virtual ~CRichTextEditorForTC();


	void Initialize() override;

	glm::vec2                   GetTextSize() const;

	// CUIBaseNode
	virtual glm::vec2           GetSize() override;

	virtual bool                AcceptMesh(IVisitor* visitor) override;

	virtual bool                OnKeyPressed(KeyEventArgs& e);
	virtual bool                OnMouseButtonPressed(MouseButtonEventArgs& e);

private:
	void FixSelectedChar();

private:
	std::shared_ptr<CFontMesh>         m_FontMesh;
	std::shared_ptr<UI_Font_Material>  m_FontMaterial;

	std::vector<std::string> m_Lines;

	glm::ivec2 SelectedChar;
};