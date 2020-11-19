#include "stdafx.h"

// General
#include "UIControlWindow.h"


namespace
{
	const glm::vec2 cUITextureSize = glm::vec2(1193.0f, 1193.0f);
	const glm::vec2 cWindowTextureSize = glm::vec2(128.0f, 128.0f);

	const glm::vec2 cWindowTextureStart = glm::vec2(528.0f, 502.0f) / cUITextureSize;
	const glm::vec2 cWindowTextureEnd = cWindowTextureStart + (cWindowTextureSize / cUITextureSize);
}


CUIControlWindow::CUIControlWindow(IScene& Scene)
	: CUIControlCommon(Scene)
{
}

CUIControlWindow::~CUIControlWindow()
{
}



//
// CUIControl
//
void CUIControlWindow::Initialize()
{
	glm::vec2 sizeDiv3 = cWindowTextureSize / 3.0f;


	std::shared_ptr<CMaterialUIControl> uiMaterial = MakeShared(CMaterialUIControl, GetBaseManager().GetApplication().GetRenderDevice());
	uiMaterial->SetTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("Interface Pack/Spritesheet/interfacePack_sheet@2.png"));

	// Left-Top
	{
		glm::vec2 cWindowTextureEndNew = cWindowTextureStart + (cWindowTextureSize / cUITextureSize) / 3.0f;

		SSubgeometry subGeom;
		subGeom.Translate = glm::vec2(0.0f);
		subGeom.Size = sizeDiv3;
		subGeom.Material = uiMaterial;
		subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(sizeDiv3, cWindowTextureStart, cWindowTextureEndNew);
		AddSubgeometry(subGeom);
	}
}
