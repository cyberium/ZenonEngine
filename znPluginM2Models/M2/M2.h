#pragma once

#include "M2_Headers.h"

// Components
#include "M2_Comp_Materials.h"
#include "M2_Comp_Miscellaneous.h"
#include "M2_Comp_Skeleton.h"

// Skins
#include "M2_Skin.h"

class ZN_API CM2 
	: public CLoadableObject
{
public:
	CM2(IBaseManager& BaseManager, IRenderDevice& RenderDevice, const std::string& FileName);
	virtual ~CM2();

	// CM2
	std::shared_ptr<CM2_Skin> GetSkin() const;

	// CLoadableObject
	bool Load() override;
	bool Delete() override;

	IBaseManager& GetBaseManager() const { return m_BaseManager; }
	IRenderDevice& GetRenderDevice() const { return m_RenderDevice; }

public:
	std::string getFilename() const { return m_F->Path_Name(); }
	std::string getFilenameWithoutExt() const { return m_F->Path() + "" + m_F->Name_NoExtension(); }
	std::string getFilePath() const { return m_F->Path(); }
	std::string getUniqueName() const { return m_UniqueName; }
	const BoundingBox& GetBounds() const { return m_Bounds; }
	
public:
	const bool isAnimated() const { return m_IsAnimated; }
private:
	bool m_IsAnimated;


public:
	const CM2_Comp_Materials& getMaterials() const { return *m_Materials; }
	const CM2_Comp_Miscellaneous& getMiscellaneous() const { return *m_Miscellaneous; }
	const CM2_Comp_Skeleton& getSkeleton() const { return *m_Skeleton; }
private:
	std::unique_ptr<CM2_Comp_Materials> m_Materials;
	std::unique_ptr<CM2_Comp_Miscellaneous> m_Miscellaneous;
	std::unique_ptr<CM2_Comp_Skeleton> m_Skeleton;


private:
	std::shared_ptr<IFile>              m_F;

private:
	SM2_Header				            m_Header;
	std::string							m_UniqueName;
	BoundingBox							m_Bounds;

	// Skins
	std::shared_ptr<CM2_Skin> m_Skin;

private:
	// Buffers and geom
	std::shared_ptr<IModel>				m_CollisionGeom;
	uint32								m_CollisionIndCnt;
	uint32								m_CollisionVetCnt;

private:
	IBaseManager& m_BaseManager;
	IRenderDevice& m_RenderDevice;
};
