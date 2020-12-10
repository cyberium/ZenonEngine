#pragma once

class ZN_API CRenderPrimitivesFactory
	: public IRenderPrimitivesFactory
{
public:
	CRenderPrimitivesFactory(IRenderDevice& RenderDevice);
	virtual ~CRenderPrimitivesFactory();

	// IRenderPrimitivesFactory
	std::shared_ptr<IGeometry> CreateLine(const glm::vec3& _dest) override; 
	std::shared_ptr<IGeometry> CreateLines(size_t count = 100) override;
	std::shared_ptr<IGeometry> CreateLines(const std::vector<glm::vec3>& Points) override;
	std::shared_ptr<IGeometry> CreateLines2(const std::vector<glm::vec3>& Points) override;
	std::shared_ptr<IGeometry> CreatePlane(const glm::vec3& N = glm::vec3(0, 1, 0)) override;
	std::shared_ptr<IGeometry> CreateSphere(float Radius = 1.0f) override;
	std::shared_ptr<IGeometry> CreateCube() override;
	std::shared_ptr<IGeometry> CreateBBox() override;
	std::shared_ptr<IGeometry> CreateFrustum(const Frustum& Frustum) override;
	std::shared_ptr<IGeometry> CreateCone(float diameter, float height) override;
	std::shared_ptr<IGeometry> CreateTorus(float Radius, float Thickness) override;
	std::shared_ptr<IGeometry> CreateQuad() override;

	// UI meshes
	std::shared_ptr<IGeometry> CreateUIQuad(glm::vec2 Size, glm::vec2 TexCoordStart = glm::vec2(0.0f), glm::vec2 TexCoordEnd = glm::vec2(1.0f)) override;

private:
	IRenderDevice& m_RenderDevice;
};
