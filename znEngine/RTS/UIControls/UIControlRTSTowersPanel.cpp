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
	}*/
}

void CUIControlRTSTowersPanel::AddTowerButton(std::string Name, std::string TowerXMLName, std::string TowerTextureName, uint32 TowerCost, IScene& Scene)
{
	STowerDescription towerDescription;
	towerDescription.Name = Name;
	towerDescription.Cost = TowerCost;
	towerDescription.XMLPath = TowerXMLName;

	{
		CXMLManager xmlManager(GetBaseManager());
		auto reader = xmlManager.CreateReaderFromFile(TowerXMLName);
		_ASSERT(false == reader->GetChilds().empty());
		auto firstXMLChild = reader->GetChilds()[0];

		towerDescription.SceneNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<CSceneNodeFactory>()->LoadSceneNode3DXML(firstXMLChild, Scene);
		towerDescription.SceneNode->MakeMeOrphan();
	}

	std::shared_ptr<CUIControlRTSTowerBtn> towerBtnNode = GetScene().CreateUIControlTCast<CUIControlRTSTowerBtn>(shared_from_this());
	towerBtnNode->SetTowerTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D(TowerTextureName));
	towerBtnNode->SetOnClickCallback(std::bind(&CUIControlRTSTowersPanel::OnTowerButtonClick, this, std::placeholders::_1, std::placeholders::_2));
	towerDescription.UIButton = towerBtnNode;

	m_TowerButtons.push_back(towerDescription);

	const float cDiff = 15.0f;

	for (size_t i = 0; i < m_TowerButtons.size(); i++)
	{
		const auto& towerBtnNode = m_TowerButtons.at(i);
		towerBtnNode.UIButton->SetLocalPosition(glm::vec2((towerBtnNode.UIButton->GetSize().x + cDiff) * i, 0.0f));
	}

	glm::vec2 panelSize = glm::vec2(towerBtnNode->GetSize().x * m_TowerButtons.size(), 85.0f);
	if (m_TowerButtons.size() > 1)
		panelSize.x += cDiff * (m_TowerButtons.size() - 1);

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

	std::shared_ptr<CMaterialUIControl> uiMaterial = MakeShared(CMaterialUIControl, GetBaseManager().GetApplication().GetRenderDevice());
	uiMaterial->SetTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("Interface Pack/Spritesheet/interfacePack_sheet@2.png"));


	ClearSubgeometries();


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

void CUIControlRTSTowersPanel::OnTowerButtonClick(const IUIControl * Node, glm::vec2 PosInsideButton)
{
	const auto& towerButton = std::find_if(m_TowerButtons.begin(), m_TowerButtons.end(), [Node](const STowerDescription& TowerDescription) {
		return TowerDescription.UIButton.get() == Node;
	});
	if (towerButton == m_TowerButtons.end())
		throw CException("Tower button '%s' not found.", Node->GetName().c_str());

	if (m_OnTowerButtonClicked)
		m_OnTowerButtonClicked(*towerButton);
}

