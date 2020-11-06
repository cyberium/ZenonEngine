#pragma once

#include "Tools/EditorToolBase.h"

class CRTSGround
	: public CEditorToolBase
{
public:
	

	struct SRTSGroundCellProto
	{
		std::string FileName;
		ERTSCellType Type;
	};

public:
	CRTSGround(IEditor& Editor);
	virtual ~CRTSGround();

	// IEditorTool
	void Enable() override;
	void Disable() override;

	// IEditorToolUI
	void DoInitializeUI(IEditorQtUIFrame& QtUIFrame) override;

	// 3D
	void DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;
	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) override;

	// 
	void SetNodeType(ERTSCellType CellType);

protected:
	IBaseManager& GetBaseManager() const;
	IRenderDevice& GetRenderDevice() const;

private:
	std::shared_ptr<ISceneNodeRTSGround> m_GroundRoot;

	std::shared_ptr<ISceneNode> m_GroundSelectorNode;
	CRTSGround::ERTSCellType m_CellType;

private:
	IBaseManager& m_BaseManager;
};