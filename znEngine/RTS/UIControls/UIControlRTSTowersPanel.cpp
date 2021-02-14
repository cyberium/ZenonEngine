#include "stdafx.h"

// General
#include "UIControlRTSTowersPanel.h"

// Additional
#include "UIControls/Common/UIControlCommonModel.h"
#include "UIControls/Common/UIControlCommonMaterial.h"

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
// CUIControlCommon
//
void CUIControlRTSTowersPanel::Initialize()
{
	__super::Initialize();

	//<SubTexture name="button_rectangleWood.png" x="320" y="1152" width="80" height="40"/>


	// Gold counter
	/*{
		std::shared_ptr<IUIControlCommon> goldPanel = GetScene().CreateUIControlTCast<IUIControlCommon>(shared_from_this());
		goldPanel->SetLocalPosition(glm::vec2(-15.0f, -55.0f));

		{
			std::shared_ptr<IUIControlText> goldText = GetScene().CreateUIControlTCast<IUIControlText>(goldPanel);
			goldText->SetLocalPosition(glm::vec2(10.0f, 8.0f));
			goldText->SetFont(GetBaseManager().GetManager<IznFontsManager>()->Add(GetRenderDevice(), "Fonts/consola.ttf", 20));
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
	}*/
}

void CUIControlRTSTowersPanel::AddTowerButton(std::string TowerName, std::string TowerIcon, uint32 TowerCost, IScene& Scene)
{
	STowerDescription towerDescription;
	towerDescription.TowerName = TowerName;
	towerDescription.TowerIcon = TowerIcon;
	towerDescription.TowerCost = TowerCost;

	std::shared_ptr<CUIControlRTSTowerBtn> currenTowerBtn = GetScene().CreateUIControlTCast<CUIControlRTSTowerBtn>(shared_from_this());
	currenTowerBtn->SetTowerTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D(TowerIcon));
	currenTowerBtn->SetOnClickCallback(std::bind(&CUIControlRTSTowersPanel::OnTowerButtonClick, this, std::placeholders::_1, std::placeholders::_2));
	towerDescription.UIButton = currenTowerBtn;

	m_TowerButtons.push_back(towerDescription);

	const float cDiff = 15.0f;

	for (size_t i = 0; i < m_TowerButtons.size(); i++)
	{
		const auto& towerBtn = m_TowerButtons.at(i);
		currenTowerBtn->SetLocalPosition(glm::vec2(0.0f, (currenTowerBtn->GetSize().y + cDiff) * i));
	}

	glm::vec2 panelSize = glm::vec2(85.0f, currenTowerBtn->GetSize().y * m_TowerButtons.size());
	if (m_TowerButtons.size() > 1)
		panelSize.y += cDiff * (m_TowerButtons.size() - 1);

	CreateWindowGeometry(panelSize);
}

const std::vector<STowerDescription>& CUIControlRTSTowersPanel::GetTowerButtons()
{
	return m_TowerButtons;
}

void CUIControlRTSTowersPanel::SetTowerButtonClickCallback(std::function<bool(const STowerDescription&)> Func)
{
	_ASSERT(m_OnTowerButtonClicked == nullptr);
	m_OnTowerButtonClicked = Func;
}



//
// Protected
//
void CUIControlRTSTowersPanel::CreateWindowGeometry(glm::vec2 Size)
{
	glm::vec2 sizeDiv3 = cUIWindowCell.CellSize / 3.0f;
	glm::vec2 sizeDiv3UV = cUIWindowCell.UVCellSize / 3.0f;

	SetSize(sizeDiv3 + Size + sizeDiv3);

	std::shared_ptr<CUIControlCommonMaterial> uiMaterial = MakeShared(CUIControlCommonMaterial, GetBaseManager().GetApplication().GetRenderDevice());
	uiMaterial->SetTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("Interface Pack/Spritesheet/interfacePack_sheet@2.png"));


	ClearSubgeometries();


	// Left-Top
	{
		auto subGeom = MakeShared(CUIControlCommonModel, GetRenderDevice());
		subGeom->SetTranslate(glm::vec2(-sizeDiv3.x, -sizeDiv3.y));
		subGeom->SetSize(sizeDiv3);
		subGeom->SetMaterial(uiMaterial);
		subGeom->SetGeom(GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			cUIWindowCell.UVCellStart + 0.0f * sizeDiv3UV,
			cUIWindowCell.UVCellStart + 1.0f * sizeDiv3UV
		));
		AddSubgeometry(subGeom);
	}



	// Left
	{
		auto subGeom = MakeShared(CUIControlCommonModel, GetRenderDevice());
		subGeom->SetTranslate(glm::vec2(-sizeDiv3.x, 0.0f));
		subGeom->SetSize(glm::vec2(sizeDiv3.x, Size.y));
		subGeom->SetMaterial(uiMaterial);
		subGeom->SetGeom(GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 0.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 1.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 1.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 2.0f * sizeDiv3UV.y)
		));
		AddSubgeometry(subGeom);
	}



	// Left-Bottom
	{
		auto subGeom = MakeShared(CUIControlCommonModel, GetRenderDevice());
		subGeom->SetTranslate(glm::vec2(-sizeDiv3.x, Size.y));
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
		auto subGeom = MakeShared(CUIControlCommonModel, GetRenderDevice());
		subGeom->SetTranslate(glm::vec2(Size.x, -sizeDiv3.y));
		subGeom->SetSize(sizeDiv3);
		subGeom->SetMaterial(uiMaterial);
		subGeom->SetGeom(GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 2.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 0.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 3.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 1.0f * sizeDiv3UV.y)
		));
		AddSubgeometry(subGeom);
	}



	// Right
	{
		auto subGeom = MakeShared(CUIControlCommonModel, GetRenderDevice());
		subGeom->SetTranslate(glm::vec2(Size.x, 0.0f));
		subGeom->SetSize(glm::vec2(sizeDiv3.x, Size.y));
		subGeom->SetMaterial(uiMaterial);
		subGeom->SetGeom(GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 2.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 1.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 3.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 2.0f * sizeDiv3UV.y)
		));
		AddSubgeometry(subGeom);
	}



	// Right-Bottom
	{
		auto subGeom = MakeShared(CUIControlCommonModel, GetRenderDevice());
		subGeom->SetTranslate(glm::vec2(Size.x, Size.y));
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
		auto subGeom = MakeShared(CUIControlCommonModel, GetRenderDevice());
		subGeom->SetTranslate(glm::vec2(0.0f, -sizeDiv3.y));
		subGeom->SetSize(glm::vec2(Size.x, sizeDiv3.y));
		subGeom->SetMaterial(uiMaterial);
		subGeom->SetGeom(GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 1.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 0.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 2.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 1.0f * sizeDiv3UV.y)
		));
		AddSubgeometry(subGeom);
	}



	// Bottom
	{
		auto subGeom = MakeShared(CUIControlCommonModel, GetRenderDevice());
		subGeom->SetTranslate(glm::vec2(0.0f, Size.y));
		subGeom->SetSize(glm::vec2(Size.x, sizeDiv3.y));
		subGeom->SetMaterial(uiMaterial);
		subGeom->SetGeom(GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 1.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 2.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 2.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 3.0f * sizeDiv3UV.y)
		));
		AddSubgeometry(subGeom);
	}



	// Center
	{
		std::shared_ptr<CUIControlCommonMaterial> uiMaterialCenter = MakeShared(CUIControlCommonMaterial, GetBaseManager().GetApplication().GetRenderDevice());
		uiMaterialCenter->SetTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("Interface Pack/Spritesheet/interfacePack_sheet@2.png"));
		uiMaterialCenter->SetColor(glm::vec4(0.8f, 1.0f, 1.0f, 1.0f));

		auto subGeom = MakeShared(CUIControlCommonModel, GetRenderDevice());
		subGeom->SetTranslate(glm::vec2(0.0f, 0.0f));
		subGeom->SetSize(glm::vec2(Size.x, Size.y));
		subGeom->SetMaterial(uiMaterial);
		subGeom->SetGeom(GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 1.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 1.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 2.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 2.0f * sizeDiv3UV.y)
		));
		AddSubgeometry(subGeom);
	}
}

void CUIControlRTSTowersPanel::OnTowerButtonClick(const IUIControl* Node, glm::vec2 PosInsideButton)
{
	const auto& towerButton = std::find_if(m_TowerButtons.begin(), m_TowerButtons.end(), [Node](const STowerDescription& TowerDescription) {
		return TowerDescription.UIButton.get() == Node;
	});
	if (towerButton == m_TowerButtons.end())
		throw CException("Tower button '%s' not found.", Node->GetName().c_str());

	if (m_OnTowerButtonClicked)
		m_OnTowerButtonClicked(*towerButton);
}

