#pragma once

class ZN_API CSceneCreateTypelessListPass
	: public ScenePass
	, public IRenderPassCreateTypelessList
{
public:
	CSceneCreateTypelessListPass(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CSceneCreateTypelessListPass();

	const std::vector<SNodeElement>& GetNodesList() const override;
	const std::vector<SModelElement>& GetModelsList() const override;
	const std::vector<SGeometryElement>& GetGeometryList() const override;
	const std::vector<SLightElement>& GetLightList() const override;

	// IRenderPass
	void PreRender(RenderEventArgs& e) override;
	void Render(RenderEventArgs& e) override;

	// IVisitor
	EVisitResult Visit(const ISceneNode* SceneNode) override;
	EVisitResult Visit(const IModel* Model) override;
	EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	EVisitResult Visit(const ILight3D* light) override;

private:
	std::vector<SNodeElement> m_NodesList;
	std::vector<SModelElement> m_ModelsList;
	std::vector<SGeometryElement> m_GeometryList;
	std::vector<SLightElement> m_LightList;
};
