#include "stdafx.h"

// General
#include "UIControlRTSTowerBtn.h"

// Additional
#include "Materials/MaterialUIControl.h"

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

	std::shared_ptr<CMaterialUIControl> contentMaterial = MakeShared(CMaterialUIControl, GetBaseManager().GetApplication().GetRenderDevice());
	//contentMaterial->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 0.7f));

	SSubgeometry subGeom;
	subGeom.Translate = glm::vec2(14.0f, 14.0f);
	subGeom.Size = glm::vec2(58);
	subGeom.Material = contentMaterial;
	subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f));
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
			std::shared_ptr<CMaterialUIControl> goldPanelMaterial = MakeShared(CMaterialUIControl, GetBaseManager().GetApplication().GetRenderDevice());
			goldPanelMaterial->SetTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("Interface Pack/Spritesheet/interfacePack_sheet@2.png"));

			SUISpreadsheetCell goldPanelCell(glm::vec2(320.0f, 1152.0f), glm::vec2(80.0f, 40.0f), glm::vec2(1193.0f, 1193.0f));

			SSubgeometry subGeom;
			subGeom.Translate = glm::vec2(0.0f, 0.0f);
			subGeom.Size = goldPanelCell.CellSize;
			subGeom.Material = goldPanelMaterial;
			subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f), goldPanelCell.UVCellStart, goldPanelCell.UVCellEnd);
			goldPanel->AddSubgeometry(subGeom);
		}
	}



	CreateWindowGeometry(0.0f);
}

void CUIControlRTSTowerBtn::SetTowerTexture(std::shared_ptr<ITexture> Texture)
{
	m_ButtonContent->GetSubgeometries()[0].Material->SetTexture(Texture);
}

void CUIControlRTSTowerBtn::OnMouseEntered()
{
	for (auto& subGeometry : GetSubgeometries())
	{
		std::dynamic_pointer_cast<CMaterialUIControl>(subGeometry.Material)->SetColor(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
	}
}

void CUIControlRTSTowerBtn::OnMouseLeaved()
{
	for (auto& subGeometry : GetSubgeometries())
	{
		std::dynamic_pointer_cast<CMaterialUIControl>(subGeometry.Material)->SetColor(glm::vec4(1.0f));
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

	std::shared_ptr<CMaterialUIControl> uiMaterial = MakeShared(CMaterialUIControl, GetBaseManager().GetApplication().GetRenderDevice());
	uiMaterial->SetTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("Interface Pack/Spritesheet/interfacePack_sheet@2.png"));

	// Left-Top
	{
		SSubgeometry subGeom;
		subGeom.Translate = glm::vec2(0.0f, 0.0f);
		subGeom.Size = sizeDiv3;
		subGeom.Material = uiMaterial;
		subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			cUIWindowCell.UVCellStart + 0.0f * sizeDiv3UV,
			cUIWindowCell.UVCellStart + 1.0f * sizeDiv3UV
		);
		AddSubgeometry(subGeom);
	}



	// Left-Bottom
	{
		SSubgeometry subGeom;
		subGeom.Translate = glm::vec2(0.0f, sizeDiv3.y);
		subGeom.Size = sizeDiv3;
		subGeom.Material = uiMaterial;
		subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 0.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 2.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 1.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 3.0f * sizeDiv3UV.y)
		);
		AddSubgeometry(subGeom);
	}



	// Right-Top
	{
		SSubgeometry subGeom;
		subGeom.Translate = glm::vec2(sizeDiv3.x + Width, 0.0f);
		subGeom.Size = sizeDiv3;
		subGeom.Material = uiMaterial;
		subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 2.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 3.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 1.0f * sizeDiv3UV.y)
		);
		AddSubgeometry(subGeom);
	}



	// Right-Bottom
	{
		SSubgeometry subGeom;
		subGeom.Translate = glm::vec2(sizeDiv3.x + Width, sizeDiv3.y);
		subGeom.Size = sizeDiv3;
		subGeom.Material = uiMaterial;
		subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			cUIWindowCell.UVCellStart + 2.0f * sizeDiv3UV,
			cUIWindowCell.UVCellStart + 3.0f * sizeDiv3UV
		);
		AddSubgeometry(subGeom);
	}



	// Top
	{
		SSubgeometry subGeom;
		subGeom.Translate = glm::vec2(sizeDiv3.x, 0.0f);
		subGeom.Size = glm::vec2(Width, sizeDiv3.y);
		subGeom.Material = uiMaterial;
		subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 1.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 0.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 2.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 1.0f * sizeDiv3UV.y)
		);
		AddSubgeometry(subGeom);
	}



	// Bottom
	{
		SSubgeometry subGeom;
		subGeom.Translate = glm::vec2(sizeDiv3.x, sizeDiv3.y); 
		subGeom.Size = glm::vec2(Width, sizeDiv3.y);
		subGeom.Material = uiMaterial;
		subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 1.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 2.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 2.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 3.0f * sizeDiv3UV.y)
		);
		AddSubgeometry(subGeom);
	}
}
