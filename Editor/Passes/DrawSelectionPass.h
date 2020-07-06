#pragma once

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
	std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override final;

	// IVisitor
	EVisitResult Visit(const ISceneNode3D* node) override final;
	EVisitResult Visit(const IModel* Model) override final;

protected:
	IEditor_NodesSelector&             m_Selector;

	std::shared_ptr<IGeometry>         m_QuadGeometry;

	IShaderParameter*                  m_ShaderInstancesBufferParameter;
	std::shared_ptr<IStructuredBuffer> m_InstancesBuffer;
	size_t							   m_InstancesCnt;
};
