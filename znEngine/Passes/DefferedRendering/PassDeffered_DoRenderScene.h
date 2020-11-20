#pragma once

#include "../Scene/SceneCreateTypelessListPass.h"

class ZN_API CPassDeffered_DoRenderScene
	: public RenderPassPipelined
{
public:
	CPassDeffered_DoRenderScene(IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneCreateTypelessListPass);
	virtual ~CPassDeffered_DoRenderScene();

	// CPassDeffered_DoRenderScene
	std::shared_ptr<ITexture> GetTexture0() const;
	std::shared_ptr<ITexture> GetTexture1() const;
	std::shared_ptr<ITexture> GetTexture2() const;
	std::shared_ptr<ITexture> GetTexture3() const;
	std::shared_ptr<ITexture> GetTextureDepthStencil() const;

	// IRenderPass
	void PreRender(RenderEventArgs& e) override;
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

	// IVisitor
	void DoRenderSceneNode(const ISceneNode* SceneNode);
	void DoRenderGeometry(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs);

private:
	std::shared_ptr<IRenderTarget> CreateGBuffer(std::shared_ptr<IRenderTarget> RenderTarget);

private:
	std::shared_ptr<IRenderPassCreateTypelessList> m_SceneCreateTypelessListPass;

private:
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;

	IShaderParameter* m_PerObjectShaderParameter;
	IShaderParameter* m_ShaderBonesBufferParameter;

private:
	std::shared_ptr<ITexture> m_Texture0;
	std::shared_ptr<ITexture> m_Texture1;
	std::shared_ptr<ITexture> m_Texture2;
	std::shared_ptr<ITexture> m_Texture3;
	std::shared_ptr<ITexture> m_DepthStencilTexture;
};