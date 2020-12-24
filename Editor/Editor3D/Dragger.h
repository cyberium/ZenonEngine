#pragma once

#include "EditorInterfaces.h"

class CDragger
{
public:
	CDragger(IEditor3DFrame& Editor3DFrame);
	virtual ~CDragger();

	void Initialize(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget);

	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld);
	void OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld);
	void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld);
	
	bool OnDragEnterEvent(const SDragData& Data);
	void OnDragMoveEvent(const glm::vec2& Position);
	void OnDragLeaveEvent();
	void OnDropEvent(const glm::vec2& Position);

protected: // Editor
	std::shared_ptr<ISceneNode> CreateDraggedNode(const CByteBuffer& ByteBuffer);
	void MoveDraggedNode(const glm::vec2& MousePos);
	void CreateCopyDraggedNode();
	void Clear();
	glm::vec3 CalculateDraggedNodePosition(const glm::vec2& MousePos);

private:
	IEditor& GetEditor() const;
	const IBaseManager& GetBaseManager() const;
	const IRenderDevice& GetRenderDevice() const;
	IScene& GetScene() const;

private:
	bool m_IsDraggingPermanentCreation;
	std::shared_ptr<ISceneNode> m_DraggerNode;
	std::shared_ptr<ISceneNode> m_LastCreatedNode;
	std::shared_ptr<IUIControlText> m_DraggerTextUI;

	IEditor3DFrame& m_Editor3DFrame;
};
