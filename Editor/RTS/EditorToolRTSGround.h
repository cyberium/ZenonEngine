#pragma once

#include "Tools/EditorToolBase.h"

class CRTSGround
	: public CEditorToolBase
{
public:
	struct SRTSCell
	{
		enum class ERTSCellType : uint8
		{
			ctEmpty = 0,
			ctGround = 1,
			ctWater = 2
		};

		int32 IndexX, IndexZ;
		ERTSCellType Type;
		std::shared_ptr<ISceneNode3D> Node;
	};

	struct SRTSGroundCellProto
	{
		std::string FileName;
		SRTSCell::ERTSCellType Type;
	};

public:
	CRTSGround(IEditor& Editor);
	virtual ~CRTSGround();

	// IEditorTool
	void Initialize() override;
	void Finalize() override;
	void Enable() override;
	void Disable() override;

	// 3D
	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) override;

	void AddCell(CRTSGround::SRTSCell::ERTSCellType CellType, uint32 IndexX, uint32 IndexZ);
	CRTSGround::SRTSCell& GetCell(uint32 IndexX, uint32 IndexZ);

protected:
	IBaseManager& GetBaseManager() const;
	IRenderDevice& GetRenderDevice() const;

private:
	SRTSCell m_Cells[64][64];
	uint32 m_CellSize;
	std::shared_ptr<ISceneNode3D> m_GroundRoot;
	std::shared_ptr<ISceneNode3D> m_GroundRootForSelectors;

	IScene& m_Scene;
};