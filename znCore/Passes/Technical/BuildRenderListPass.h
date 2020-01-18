#pragma once

#include "../ScenePass.h"

class ZN_API BuildRenderListPass
	: public ScenePass
{
public:
	struct ZN_API SGeometryElement
	{
		SGeometryElement(const ISceneNode* Node, const IGeometry* Geometry, const IMaterial* Material, const SGeometryPartParams GeometryPartParams)
			: Node(Node)
			, Geometry(Geometry)
			, Material(Material)
			, GeometryPartParams(GeometryPartParams)
		{}

		const ISceneNode* Node;
		const IGeometry* Geometry;
		const IMaterial* Material;
		const SGeometryPartParams GeometryPartParams;
	};

	struct ZN_API SLightElement
	{
		SLightElement(const ISceneNode* Node, const ILightComponent3D* Light)
			: Node(Node)
			, Light(Light)
		{}

		const ISceneNode* Node;
		const ILightComponent3D* Light;
	};

public:
	BuildRenderListPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~BuildRenderListPass();

	const std::vector<SGeometryElement>& GetGeometryList() const;
	const std::vector<SLightElement>& GetLightList() const;

	// IRenderPass
	virtual void PreRender(RenderEventArgs& e) override;
	virtual void Render(RenderEventArgs& e) override;

	// IVisitor
	virtual bool Visit3D(ISceneNode* node) override;
	virtual bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	virtual bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	virtual bool Visit(ILightComponent3D* light) override;

private:
	const ISceneNode* m_CurrentSceneNode;

	std::vector<SGeometryElement> m_GeometryList;
	std::vector<SLightElement> m_LightList;
};
