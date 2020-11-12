#pragma once

#include "M2/M2_Base_Instance.h"

struct __declspec(align(16)) ZN_API M2PerObject
{
	M2PerObject(const glm::mat4& Model, const glm::vec4& Color)
		: Model(Model)
		, Color(Color)
	{}
	glm::mat4 Model;
	glm::vec4 Color;
};

class ZN_API CRenderPass_M2 
	: public RenderPassPipelined
{
public:
	CRenderPass_M2(IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneNodeListPass, bool OpaqueDraw);
	virtual ~CRenderPass_M2();

	// CRenderPass_M2
	void DoRenderM2Model(const CM2_Base_Instance* M2SceneNode, const CM2_Skin* M2Model, bool OpaqueDraw, UINT InstancesCnt = UINT32_MAX);

	// IRenderPass
	virtual void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

    // IVisitor
	virtual void DoRenderSceneNode(const ISceneNode* SceneNode);
	virtual void DoRenderModel(const IModel* Model);

protected:
	bool m_OpaqueDraw;
	std::shared_ptr<IRenderPassCreateTypelessList> m_SceneCreateTypelessListPass;

protected:
	const CM2_Base_Instance* m_CurrentM2Model;

	std::shared_ptr<IConstantBuffer> m_M2PerObjectConstantBuffer;
	IShaderParameter* m_ShaderM2PerObjectParameter;
	IShaderParameter* m_ShaderM2GeometryParameter;
	IShaderParameter* m_ShaderM2GeometryBonesParameter;
};
