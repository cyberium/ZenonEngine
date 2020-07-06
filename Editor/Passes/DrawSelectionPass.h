#pragma once

#include "SceneNodesSelector.h"

struct __declspec(novtable, align(16)) SSelectorPerObject
{
	SSelectorPerObject(const glm::mat4& Model, const glm::vec4& Color)
		: Model(Model)
		, Color(Color)
	{}
	glm::mat4 Model;
	glm::vec4 Color;
};

class CDrawSelectionPass
	: public RenderPassPipelined
{
public:
	CDrawSelectionPass(IRenderDevice& RenderDevice, IEditor_NodesSelector& Selector);
	virtual ~CDrawSelectionPass();

	void RefreshInstanceBuffer();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	virtual EVisitResult Visit(const ISceneNode3D* node) override;
	virtual EVisitResult Visit(const IModel* Model) override;

protected:
	IEditor_NodesSelector&             m_Selector;

	std::shared_ptr<IGeometry>         m_QuadGeometry;

	IShaderParameter*                  m_ShaderInstancesBufferParameter;
	std::shared_ptr<IStructuredBuffer> m_InstancesBuffer;
	size_t							   m_InstancesCnt;
};
