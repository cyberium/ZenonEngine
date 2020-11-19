#include "stdafx.h"

// General
#include "UIControlWindow.h"


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
	__super::Initialize();

	CreateWindowGeometry(glm::vec2(128.0f * 3.0f, 85.0f));
}



//
// Protected
//
void CUIControlWindow::CreateWindowGeometry(glm::vec2 Size)
{
	glm::vec2 sizeDiv3 = cUIWindowCell.CellSize / 3.0f;
	glm::vec2 sizeDiv3UV = cUIWindowCell.UVCellSize / 3.0f;

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
		subGeom.Material = uiMaterialCenter;
		subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f),
			glm::vec2(cUIWindowCell.UVCellStart.x + 1.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 1.0f * sizeDiv3UV.y),
			glm::vec2(cUIWindowCell.UVCellStart.x + 2.0f * sizeDiv3UV.x, cUIWindowCell.UVCellStart.y + 2.0f * sizeDiv3UV.y)
		);
		AddSubgeometry(subGeom);
	}
}
