#pragma once

#include "../ScenePass.h"

class ZN_API BuildRenderListPass
	: public ScenePass
{
public:
	struct ZN_API SGeometryElement
	{
		SGeometryElement(const ISceneNode3D* Node, const IGeometry* Geometry, const IMaterial* Material, const SGeometryDrawArgs GeometryDrawArgs)
			: Node(Node)
			, Geometry(Geometry)
			, Material(Material)
			, GeometryDrawArgs(GeometryDrawArgs)
		{}

		const ISceneNode3D* Node;
		const IGeometry* Geometry;
		const IMaterial* Material;
		const SGeometryDrawArgs GeometryDrawArgs;
	};

	struct ZN_API SLightElement
	{
		SLightElement(const ISceneNode3D* Node, const ILightComponent3D* Light)
			: Node(Node)
			, Light(Light)
		{}

		const ISceneNode3D* Node;
		const ILightComponent3D* Light;
	};

public:
	BuildRenderListPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~BuildRenderListPass();

	const std::vector<SGeometryElement>& GetGeometryList() const;
	const std::vector<SLightElement>& GetLightList() const;

	// IRenderPass
	virtual void PreRender(RenderEventArgs& e) override;
	virtual void Render(RenderEventArgs& e) override;

	// IVisitor
	virtual bool Visit(ISceneNode3D* node) override;
	virtual bool Visit(IMesh* Mesh, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	virtual bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	virtual bool Visit(ILightComponent3D* light) override;

private:
	const ISceneNode3D* m_CurrentSceneNode;

	std::vector<SGeometryElement> m_GeometryList;
	std::vector<SLightElement> m_LightList;
};
