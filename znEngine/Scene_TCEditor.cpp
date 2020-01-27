#include "stdafx.h"

// Gerenal
#include "Scene_TCEditor.h"

// Additional
#include "Passes/UIFontPass.h"

CScene_TCEditor::CScene_TCEditor(IBaseManager * BaseManager)
	: SceneBase(BaseManager)
{}

CScene_TCEditor::~CScene_TCEditor()
{}


//
// IGameState
//
void CScene_TCEditor::Initialize()
{
	SceneBase::Initialize();


	Load3D();
	LoadUI();
}

void CScene_TCEditor::Finalize()
{
	// Insert code here

	SceneBase::Finalize();
}


//
//
//



//
// Keyboard events
//
bool CScene_TCEditor::OnWindowKeyPressed(KeyEventArgs & e)
{
	//if (e.Key == KeyCode::F4)
	//	m_Model_Pass_Opaque->SetEnabled(!m_Model_Pass_Opaque->IsEnabled());

	//if (e.Key == KeyCode::F5)
	//	m_Model_Pass_Transperent->SetEnabled(!m_Model_Pass_Transperent->IsEnabled());

	return SceneBase::OnWindowKeyPressed(e);
}

void CScene_TCEditor::OnWindowKeyReleased(KeyEventArgs & e)
{
	SceneBase::OnWindowKeyReleased(e);
}



//
//
//
void CScene_TCEditor::Load3D()
{

}

void CScene_TCEditor::LoadUI()
{
	m_TextEditor = GetRootNodeUI()->CreateSceneNode<CRichTextEditorForTC>();

	/*std::shared_ptr<CUITextureNode> TextureUI0 = m_SceneUI->GetRootNode()->CreateWrappedSceneNode<CUITextureNode>("Test", GetRenderDevice());
	TextureUI0->SetTranslate(vec2(000.0f, 000.0f));
	TextureUI0->SetScale(vec2(600, 600));
	TextureUI0->SetTexture(m_DefferedRenderPass->GetTexture0());

	std::shared_ptr<CUITextureNode> TextureUI1 = m_SceneUI->GetRootNode()->CreateWrappedSceneNode<CUITextureNode>("Test", GetRenderDevice());
	TextureUI1->SetTranslate(vec2(600.0f, 000.0f));
	TextureUI1->SetScale(vec2(600, 600));
	TextureUI1->SetTexture(m_DefferedRenderPass->GetTexture1());

	std::shared_ptr<CUITextureNode> TextureUI2 = m_SceneUI->GetRootNode()->CreateWrappedSceneNode<CUITextureNode>("Test", GetRenderDevice());
	TextureUI2->SetTranslate(vec2(000.0f, 600.0f));
	TextureUI2->SetScale(vec2(600, 600));
	TextureUI2->SetTexture(m_DefferedRenderPass->GetTexture2());

	std::shared_ptr<CUITextureNode> TextureUI3 = m_SceneUI->GetRootNode()->CreateWrappedSceneNode<CUITextureNode>("Test", GetRenderDevice());
	TextureUI3->SetTranslate(vec2(600.0f, 600.0f));
	TextureUI3->SetScale(vec2(600, 600));
	TextureUI3->SetTexture(m_DefferedRenderPass->GetTexture3());*/

	m_TechniqueUI.AddPass(GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("ClearPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), shared_from_this()));
	m_TechniqueUI.AddPass(std::make_shared<CUIFontPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport()));
	//m_TechniqueUI.AddPass(GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("BaseUIPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), shared_from_this()));
}
