#include "stdafx.h"

// General
#include "UIControlRTSTowerBtn.h"

// Additional]
#include "UIControls/Common/UICommonModel.h"
#include "UIControls/Common/UICommonMaterial.h"

namespace
{
	struct SUISpreadsheetCell
	{
		SUISpreadsheetCell(glm::vec2 CellStart, glm::vec2 CellSize, glm::vec2 TextureSize)
			: CellSize(CellSize)
		{
			UVCellStart = CellStart / TextureSize;
			UVCellSize = CellSize / TextureSize;
			UVCellEnd = UVCellStart + UVCellSize;
		}

		glm::vec2 UVCellStart;
		glm::vec2 UVCellSize;
		glm::vec2 UVCellEnd;

		glm::vec2 CellSize;
	};

	const SUISpreadsheetCell cUIWindowCell(glm::vec2(656.0f, 128.0f), glm::vec2(128.0f, 128.0f), glm::vec2(1193.0f, 1193.0f));
}


CUIControlRTSTowerBtn::CUIControlRTSTowerBtn(IScene& Scene)
	: CUIControlCommon(Scene)
{
}

CUIControlRTSTowerBtn::~CUIControlRTSTowerBtn()
{
}



//
// CUIControlCommon
//
void CUIControlRTSTowerBtn::Initialize()
{
	__super::Initialize();

	m_ButtonContent = GetScene().CreateUIControlTCast<IUIControlCommon>(shared_from_this());

	std::shared_ptr<CUICommonMaterial> contentMaterial = MakeShared(CUICommonMaterial, GetBaseManager().GetApplication().GetRenderDevice());
	//contentMaterial->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 0.7f));

	auto subGeom = MakeShared(CUICommonModel, GetRenderDevice());
	subGeom->SetTranslate(glm::vec2(14.0f, 14.0f));
	subGeom->SetSize(glm::vec2(58));
	subGeom->SetMaterial(contentMaterial);
	subGeom->SetGeom(GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f)));
	m_ButtonContent->AddSubgeometry(subGeom);


	// Cost
	{
		std::shared_ptr<IUIControlCommon> goldPanel = GetScene().CreateUIControlTCast<IUIControlCommon>(shared_from_this());
		goldPanel->SetLocalPosition(glm::vec2(40.0f, 66.0f));
		goldPanel->SetScale(glm::vec2(0.66f));

		{
			std::shared_ptr<IUIControlText> goldText = GetScene().CreateUIControlTCast<IUIControlText>(goldPanel);
			goldText->SetLocalPosition(glm::vec2(10.0f, 5.0f));
			goldText->SetFont(GetBaseManager().GetManager<IznFontsManager>()->Add(GetRenderDevice(), "Fonts/consola.ttf", 28));
			goldText->SetText("888$");
			goldText->SetColor(ColorRGBA(0.6f, 0.6f, 0.0f, 1.0f));
		}

		{
			std::shared_ptr<CUICommonMaterial> goldPanelMaterial = MakeShared(CUICommonMaterial, GetBaseManager().GetApplication().GetRenderDevice());
			goldPanelMaterial->SetTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("Interface Pack/Spritesheet/interfacePack_sheet@2.png"));

			SUISpreadsheetCell goldPanelCell(glm::vec2(320.0f, 1152.0f), glm::vec2(80.0f, 40.0f), glm::vec2(1193.0f, 1193.0f));

			auto subGeom = MakeShared(CUICommonModel, GetRenderDevice());
			subGeom->SetTranslate(glm::vec2(0.0f, 0.0f));
			subGeom->SetSize(goldPanelCell.CellSize);
			subGeom->SetMaterial(goldPanelMaterial);
			subGeom->SetGeom(GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f), goldPanelCell.UVCellStart, goldPanelCell.UVCellEnd));
			goldPanel->AddSubgeometry(subGeom);
		}
	}



	CreateWindowGeometry(0.0f);
}

void CUIControlRTSTowerBtn::SetTowerTexture(std::shared_ptr<ITexture> Texture)
{
	m_ButtonContent->GetSubgeometries()[0]->GetMaterial()->SetTexture(Texture);
}

void CUIControlRTSTowerBtn::OnMouseEntered()
{
	for (auto& subGeometry : GetSubgeometries())
	{
		std::dynamic_pointer_cast<CUICommonMaterial>(subGeometry->GetMaterial())->SetColor(ColorRGBA(0.7f, 0.7f, 0.7f, 1.0f));
	}
}

void CUIControlRTSTowerBtn::OnMouseLeaved()
{
	for (auto& subGeometry : GetSubgeometries())
	{
		std::dynamic_pointer_cast<CUICommonMaterial>(subGeometry->GetMaterial())->SetColor(ColorRGBA(1.0f));
	}
}



//
// Protected
//
void CUIControlRTSTowerBtn::CreateWindowGeometry(float Width)
{
	glm::vec2 sizeDiv3 = cUIWindowCell.CellSize / 3.0f;
	glm::vec2 sizeDiv3UV = cUIWindowCell.UVCellSize / 3.0f;

	SetSize(sizeDiv3 + glm::vec2(Width, 0.0f) + sizeDiv3);

	std::shared_ptr<CUICommonMaterial> uiMaterial = MakeShared(CUICommonMaterial, GetBaseManager().GetApplication().GetRenderDevice());
	uiMaterial->SetTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("Interface Pack/Spritesheet/interfacePack_sheet@2.png"));

	// Left-Top
	{
		auto subGeom = MakeShared(CUICommonModel, GetRenderDevice());
		subGeom->SetTranslate(glm::vec2(0.0f, 0.0f));
		subGeom->SetSize(sizeDiv3);
		subGeom->SetMaterial(uiMaterial);
		subGeom->SetGeom(GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			cUIWindowCell.UVCellStart + 0.0f * sizeDiv3UV,
			cUIWindowCell.UVCellStart + 1.0f * sizeDiv3UV
		));
		AddSubgeometry(subGeom);
	}



	// Left-Bottom
	{
		auto subGeom = MakeShared(CUICommonModel, GetRenderDevice());
		subGeom->SetTranslate(glm::vec2(0.0f, sizeDiv3.y));
		subGeom->SetSize(sizeDiv3);
		subGeom->SetMaterial(uiMaterial);
		subGeom->SetGeom(GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 0.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 2.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 1.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 3.0f * sizeDiv3UV.y)
		));
		AddSubgeometry(subGeom);
	}



	// Right-Top
	{
		auto subGeom = MakeShared(CUICommonModel, GetRenderDevice());
		subGeom->SetTranslate(glm::vec2(sizeDiv3.x + Width, 0.0f));
		subGeom->SetSize(sizeDiv3);
		subGeom->SetMaterial(uiMaterial);
		subGeom->SetGeom(GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 2.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 3.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 1.0f * sizeDiv3UV.y)
		));
		AddSubgeometry(subGeom);
	}



	// Right-Bottom
	{
		auto subGeom = MakeShared(CUICommonModel, GetRenderDevice());
		subGeom->SetTranslate(glm::vec2(sizeDiv3.x + Width, sizeDiv3.y));
		subGeom->SetSize(sizeDiv3);
		subGeom->SetMaterial(uiMaterial);
		subGeom->SetGeom(GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			cUIWindowCell.UVCellStart + 2.0f * sizeDiv3UV,
			cUIWindowCell.UVCellStart + 3.0f * sizeDiv3UV
		));
		AddSubgeometry(subGeom);
	}



	// Top
	{
		auto subGeom = MakeShared(CUICommonModel, GetRenderDevice());
		subGeom->SetTranslate(glm::vec2(sizeDiv3.x, 0.0f));
		subGeom->SetSize(glm::vec2(Width, sizeDiv3.y));
		subGeom->SetMaterial(uiMaterial);
		subGeom->SetGeom(GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 1.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 0.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 2.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 1.0f * sizeDiv3UV.y)
		));
		AddSubgeometry(subGeom);
	}



	// Bottom
	{
		auto subGeom = MakeShared(CUICommonModel, GetRenderDevice());
		subGeom->SetTranslate(glm::vec2(sizeDiv3.x, sizeDiv3.y));
		subGeom->SetSize(glm::vec2(Width, sizeDiv3.y));
		subGeom->SetMaterial(uiMaterial);
		subGeom->SetGeom(GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 1.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 2.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 2.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 3.0f * sizeDiv3UV.y)
		));
		AddSubgeometry(subGeom);
	}
}
