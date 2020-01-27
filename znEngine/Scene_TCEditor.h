#pragma once

#include "SceneFunctional/UI/UITexture.h"
#include "SceneFunctional/UI/RichTextEditorForTC.h"

class ZN_API CScene_TCEditor
	: public SceneBase
{
public:
	CScene_TCEditor(IBaseManager * BaseManager);
	virtual ~CScene_TCEditor();

	// IGameState
	void Initialize() override;
	void Finalize() override;

	// Keyboard events
	virtual bool OnWindowKeyPressed(KeyEventArgs& e) override;
	virtual void OnWindowKeyReleased(KeyEventArgs& e) override;

private:
	void Load3D();
	void LoadUI();

private:
	std::shared_ptr<CUITextureNode> m_TextureUI;
	CRichTextEditorForTC* m_TextEditor;
};