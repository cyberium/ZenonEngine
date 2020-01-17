#pragma once

class CDefferedRender
	: public CBaseScenePass
{
public:
	CDefferedRender(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~CDefferedRender();

	void PreRender(RenderEventArgs& e) override;
	void Render(RenderEventArgs& e) override;
	void PostRender(RenderEventArgs& e) override;

	virtual void UpdateViewport(const Viewport * _viewport) override;

	virtual bool Visit3D(ISceneNode* node) override;
	virtual bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	virtual bool Visit(ILightComponent3D* light) override;

	std::shared_ptr<ITexture> GetTexture0() const;
	std::shared_ptr<ITexture> GetTexture1() const;
	std::shared_ptr<ITexture> GetTexture2() const;
	std::shared_ptr<ITexture> GetTexture3() const;

protected:
	void CreatePipeline();

private:
	__declspec(align(16)) struct PerObject3D
	{
		glm::mat4 Model;
		glm::mat4 View;
		glm::mat4 Projection;
	};
	PerObject3D* m_PerObjectData;
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;

private:
	std::shared_ptr<IPipelineState> m_DefferedPipeline;
	Viewport v;

	std::shared_ptr<ITexture> m_Texture0;
	std::shared_ptr<ITexture> m_Texture1;
	std::shared_ptr<ITexture> m_Texture2;
	std::shared_ptr<ITexture> m_Texture3;
	std::shared_ptr<ITexture> m_DepthStencilTexture;
};