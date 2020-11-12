#pragma once

#include "M2_SkinTypes.h"

#include "M2_Skin_Batch.h"
#include "M2_SkinSection.h"

// FORWARD BEGIN
class CM2;
class CM2_Base_Instance;
// FORWARD END

class CM2_Skin 
	: public ModelProxie
{
public:
	CM2_Skin(IBaseManager& BaseManager, IRenderDevice& RenderDevice, const CM2& M2Model, const SM2_SkinProfile& M2SkinProfile);
	virtual ~CM2_Skin();

	void Load(const SM2_Header& M2Header, const std::shared_ptr<IFile>& File, const std::vector<SM2_Vertex>& Vertices);

	const std::vector<std::shared_ptr<CM2_SkinSection>>& GetSections() const
	{
		return m_Sections;
	}

	const std::unordered_map<size_t, std::vector<std::shared_ptr<CM2_Skin_Batch>>>& Get_Geom_Bathes_Map() const
	{
		return m_Geom_Bathes_Map;
	}

	// IModel
	void Accept(IVisitor* visitor) override final;

private:
	std::vector<std::shared_ptr<CM2_SkinSection>> m_Sections; // 'Geometries'
	std::vector<std::shared_ptr<CM2_Skin_Batch>> m_Batches;   // 'Materials'

	std::unordered_map<size_t, std::vector<std::shared_ptr<CM2_Skin_Batch>>> m_Geom_Bathes_Map;
	
private:
	IBaseManager& m_BaseManager;
	IRenderDevice& m_RenderDevice;
	const CM2& m_M2Model;
	const SM2_SkinProfile m_M2SkinProfile;
};