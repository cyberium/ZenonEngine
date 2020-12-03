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
	virtual EVisitResult Visit(const std::shared_ptr<ISceneNode>& node) override;
	virtual EVisitResult Visit(const std::shared_ptr<IUIControl>& node) override;
	virtual EVisitResult Visit(const std::shared_ptr<IModel>& Model) override;
	virtual EVisitResult Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs) override;
	virtual EVisitResult Visit(const std::shared_ptr<ISceneNodeComponent>& Component) override;
	virtual EVisitResult Visit(const std::shared_ptr<ILight3D>& light) override;
	virtual EVisitResult Visit(const std::shared_ptr<IParticleSystem>& ParticleSystem) override;

protected:
	void BindPerObjectData(const PerObject& PerObject);

protected:
	IScene& GetScene() const;
	
private:
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
	//IShaderParameter* m_PerObjectParameter;
	IScene& m_Scene;
};