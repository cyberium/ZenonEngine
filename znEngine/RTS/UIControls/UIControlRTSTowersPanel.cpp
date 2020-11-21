#include "stdafx.h"

// General
#include "UIControlRTSTowersPanel.h"


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

	const SUISpreadsheetCell cUIWindowCell(glm::vec2(656, 0), glm::vec2(128.0f, 128.0f), glm::vec2(1193.0f, 1193.0f));
}





CUIControlRTSTowersPanel::CUIControlRTSTowersPanel(IScene& Scene)
	: CUIControlCommon(Scene)
{
}

CUIControlRTSTowersPanel::~CUIControlRTSTowersPanel()
{
}



//
// CUIControl
//
void CUIControlRTSTowersPanel::Initialize()
{
	__super::Initialize();

	//<SubTexture name="button_rectangleWood.png" x="320" y="1152" width="80" height="40"/>


	// Gold counter
	{
		std::shared_ptr<IUIControlCommon> goldPanel = GetScene().CreateUIControlTCast<IUIControlCommon>(shared_from_this());
		goldPanel->SetTranslate(glm::vec2(-15.0f, -55.0f));

		{
			std::shared_ptr<IUIControlText> goldText = GetScene().CreateUIControlTCast<IUIControlText>(goldPanel);
			goldText->SetTranslate(glm::vec2(10.0f, 8.0f));
			goldText->SetFont(GetBaseManager().GetManager<IznFontsManager>()->Add(GetRenderDevice(), "IDB_FONT_CONSOLAS", 20));
			goldText->SetText("888 $");
			goldText->SetColor(glm::vec4(0.2f, 0.3f, 0.1f, 1.0f));
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
}

void CUIControlRTSTowersPanel::AddTowerButton(std::shared_ptr<ITexture> RTSTowerBtnTexture)
{
	std::shared_ptr<CUIControlRTSTowerBtn> towerBtnNode = GetScene().CreateUIControlTCast<CUIControlRTSTowerBtn>(shared_from_this());
	towerBtnNode->SetTowerTexture(RTSTowerBtnTexture);
	m_TowerButtons.push_back(towerBtnNode);

	const float cDiff = 15.0f;

	for (size_t i = 0; i < m_TowerButtons.size(); i++)
	{
		const auto& towerBtnNode = m_TowerButtons.at(i);
		towerBtnNode->SetTranslate(glm::vec2((towerBtnNode->GetSize().x + cDiff) * i, 0.0f));
	}

	ClearSubgeometries();
	CreateWindowGeometry(glm::vec2(towerBtnNode->GetSize().x * m_TowerButtons.size() + (cDiff * (m_TowerButtons.size() - 1)), 85.0f));
}

const std::vector<std::shared_ptr<CUIControlRTSTowerBtn>>& CUIControlRTSTowersPanel::GetTowerButtons()
{
	return m_TowerButtons;
}



//
// Protected
//
void CUIControlRTSTowersPanel::CreateWindowGeometry(glm::vec2 Size)
{
	glm::vec2 sizeDiv3 = cUIWindowCell.CellSize / 3.0f;
	glm::vec2 sizeDiv3UV = cUIWindowCell.UVCellSize / 3.0f;

	SetSize(sizeDiv3 + Size + sizeDiv3);

	std::shared_ptr<CMaterialUIControl> uiMaterial = MakeShared(CMaterialUIControl, GetBaseManager().GetApplication().GetRenderDevice());
	uiMaterial->SetTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("Interface Pack/Spritesheet/interfacePack_sheet@2.png"));

	// Left-Top
	{
		SSubgeometry subGeom;
		subGeom.Translate = glm::vec2(-sizeDiv3.x, -sizeDiv3.y);
		subGeom.Size = sizeDiv3;
		subGeom.Material = uiMaterial;
		subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			cUIWindowCell.UVCellStart + 0.0f * sizeDiv3UV,
			cUIWindowCell.UVCellStart + 1.0f * sizeDiv3UV
		);
		AddSubgeometry(subGeom);
	}



	// Left
	{
		SSubgeometry subGeom;
		subGeom.Translate = glm::vec2(-sizeDiv3.x, 0.0f);
		subGeom.Size = glm::vec2(sizeDiv3.x, Size.y);
		subGeom.Material = uiMaterial;
		subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 0.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 1.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 1.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 2.0f * sizeDiv3UV.y)
		);
		AddSubgeometry(subGeom);
	}



	// Left-Bottom
	{
		SSubgeometry subGeom;
		subGeom.Translate = glm::vec2(-sizeDiv3.x, Size.y);
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
		subGeom.Translate = glm::vec2(Size.x, -sizeDiv3.y);
		subGeom.Size = sizeDiv3;
		subGeom.Material = uiMaterial;
		subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 2.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 0.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 3.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 1.0f * sizeDiv3UV.y)
		);
		AddSubgeometry(subGeom);
	}



	// Right
	{
		SSubgeometry subGeom;
		subGeom.Translate = glm::vec2(Size.x, 0.0f);
		subGeom.Size = glm::vec2(sizeDiv3.x, Size.y);
		subGeom.Material = uiMaterial;
		subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 2.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 1.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 3.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 2.0f * sizeDiv3UV.y)
		);
		AddSubgeometry(subGeom);
	}



	// Right-Bottom
	{
		SSubgeometry subGeom;
		subGeom.Translate = glm::vec2(Size.x, Size.y);
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
		subGeom.Translate = glm::vec2(0.0f, -sizeDiv3.y);
		subGeom.Size = glm::vec2(Size.x, sizeDiv3.y);
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
		subGeom.Translate = glm::vec2(0.0f, Size.y);
		subGeom.Size = glm::vec2(Size.x, sizeDiv3.y);
		subGeom.Material = uiMaterial;
		subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 1.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 2.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 2.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 3.0f * sizeDiv3UV.y)
		);
		AddSubgeometry(subGeom);
	}



	// Center
	{
		std::shared_ptr<CMaterialUIControl> uiMaterialCenter = MakeShared(CMaterialUIControl, GetBaseManager().GetApplication().GetRenderDevice());
		uiMaterialCenter->SetTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("Interface Pack/Spritesheet/interfacePack_sheet@2.png"));
		uiMaterialCenter->SetColor(glm::vec4(0.8f, 1.0f, 1.0f, 1.0f));

		SSubgeometry subGeom;
		subGeom.Translate = glm::vec2(0.0f, 0.0f);
		subGeom.Size = glm::vec2(Size.x, Size.y);
		subGeom.Material = uiMaterial;
		subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 1.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 1.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 2.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 2.0f * sizeDiv3UV.y)
		);
		AddSubgeometry(subGeom);
	}
}

