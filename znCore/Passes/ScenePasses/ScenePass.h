#pragma once

#include "../RenderPass.h"

class ZN_API ScenePass
	: public RenderPass
	, public IVisitor
{
public:
	ScenePass(IScene& Scene);
	virtual ~ScenePass();

	// IRenderPass
	virtual void Render(RenderEventArgs& e) override;

	// IVisitor
	virtual EVisitResult Visit(const std::shared_ptr<ISceneNode>& node) override;
	virtual EVisitResult Visit(const std::shared_ptr<IUIControl>& node) override;
	virtual EVisitResult Visit(const std::shared_ptr<IModel>& Model) override;
	virtual EVisitResult Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs) override;
	virtual EVisitResult Visit(const std::shared_ptr<ISceneNodeComponent>& Component) override;
	virtual EVisitResult Visit(const std::shared_ptr<ILight>& light) override;
	virtual EVisitResult Visit(const std::shared_ptr<IParticleSystem>& ParticleSystem) override;

protected:
	IScene& GetScene() const;
	
private:
	IScene& m_Scene;
};