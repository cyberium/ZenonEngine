#pragma once

class ZN_API CRenderPrimitivesFactory
	: public IRenderPrimitivesFactory
{
public:
	CRenderPrimitivesFactory(IRenderDevice& RenderDevice);
	virtual ~CRenderPrimitivesFactory();

	// IRenderPrimitivesFactory
	std::shared_ptr<IMesh> CreateLine(cvec3 _dest) override; 
	std::shared_ptr<IMesh> CreatePlane(cvec3 N = vec3(0, 1, 0)) override;
	std::shared_ptr<IMesh> CreateScreenQuad(float left = 0.0f, float right = 1.0f, float bottom = 0.0f, float top = 1.0f, float z = 0.0f) override;
	std::shared_ptr<IMesh> CreateSphere() override;
	std::shared_ptr<IMesh> CreateCube() override;
	std::shared_ptr<IMesh> CreateCone() override;
	std::shared_ptr<IMesh> CreateQuad() override;

	// 3D meshes
	std::shared_ptr<IMesh> Create3DQuad(float width = 10.0f, float height = 10.0f) override;
	std::shared_ptr<IMesh> Create3DBeizerLine(vec3 start = vec3(0.0f, 0.0f, 0.0f), vec3 end = vec3(500.0f, 0.0f, 500.0f)) override;

	// UI meshes
	std::shared_ptr<IMesh> CreateUIQuad(float width = 10.0f, float height = 10.0f) override;
	std::shared_ptr<IMesh> CreateUIBeizerLine(vec2 start = vec2(0.0f, 0.0f), vec2 end = vec2(500.0f, 500.0f)) override;

private:
	IRenderDevice& m_RenderDevice;
};
