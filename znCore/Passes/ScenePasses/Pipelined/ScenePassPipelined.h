#pragma once

#include "../../RenderPassPipelined.h"

class ZN_API ScenePassPipelined
	: public RenderPassPipelined
	, public IVisitor
{
public:
	ScenePassPipelined(IScene& Scene);
	virtual ~ScenePassPipelined();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IVisitor
	virtual EVisitResult Visit(const ISceneNode* node) override;
	virtual EVisitResult Visit(const IUIControl* node) override;
	virtual EVisitResult Visit(const IModel* Model) override;
	virtual EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs) override;
	virtual EVisitResult Visit(const ISceneNodeComponent* Component) override;
	virtual EVisitResult Visit(const ILight3D* light) override;
	virtual EVisitResult Visit(const IParticleSystem* ParticleSystem) override;

protected:
	IScene& GetScene() const;
	
private:
	IScene& m_Scene;
};