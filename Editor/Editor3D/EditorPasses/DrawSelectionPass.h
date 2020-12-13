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
	CDrawSelectionPass(IRenderDevice& RenderDevice, IEditorToolSelector& Selector);
	virtual ~CDrawSelectionPass();

	void SetNeedRefresh();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override final;

private:
	void RefreshInstanceBuffer();

protected:
	IEditorToolSelector&             m_Selector;
	bool                               m_IsDirty;

	std::shared_ptr<IGeometry>         m_QuadGeometry;

	IShaderParameter*                  m_ShaderInstancesBufferParameter;
	std::shared_ptr<IStructuredBuffer> m_InstancesBuffer;
	size_t							   m_InstancesCnt;
};
