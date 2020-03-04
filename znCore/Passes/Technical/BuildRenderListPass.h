#pragma once

#include "../ScenePass.h"



//
// CSceneNodeListPass
//
class ZN_API CSceneNodeListPass
	: public ScenePass
{
public:
	CSceneNodeListPass(IRenderDevice& RenderDevice, const std::shared_ptr<IScene>& Scene);
	virtual ~CSceneNodeListPass();

	const std::vector<const ISceneNode3D*>& GetNodesList(SceneNodeType SceneNodeType) const;

	// IRenderPass
	virtual void PreRender(RenderEventArgs& e) override;
	virtual void Render(RenderEventArgs& e) override;

	// IVisitor
	virtual bool Visit(const ISceneNode3D* SceneNode) override;

private:
	std::unordered_map<SceneNodeType, std::vector<const ISceneNode3D*>> m_NodesByType;
	std::vector<const ISceneNode3D*> m_EmptyList;
};



//
// BuildRenderListPass
//
class ZN_API BuildRenderListPass
	: public ScenePass
{
public:
	struct ZN_API SModelElement
	{
		SModelElement(const ISceneNode3D* Node, const IModel* Model)
			: Node(Node)
			, Model(Model)
		{}

		const ISceneNode3D* Node;
		const IModel* Model;
	};

	struct ZN_API SGeometryElement
	{
		SGeometryElement(const ISceneNode3D* Node, const IModel* Model, const IGeometry* Geometry, const IMaterial* Material, const SGeometryDrawArgs GeometryDrawArgs)
			: Node(Node)
			, Model(Model)
			, Geometry(Geometry)
			, Material(Material)
			, GeometryDrawArgs(GeometryDrawArgs)
		{}

		const ISceneNode3D* Node;
		const IModel* Model;
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

	const std::vector<SModelElement>& GetModelsList() const;
	const std::vector<SGeometryElement>& GetGeometryList() const;
	const std::vector<SLightElement>& GetLightList() const;

	// IRenderPass
	virtual void PreRender(RenderEventArgs& e) override;
	virtual void Render(RenderEventArgs& e) override;

	// IVisitor
	virtual bool Visit(const ISceneNode3D* node) override;
	virtual bool Visit(const IModel* Model) override;
	virtual bool Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	virtual bool Visit(const ILightComponent3D* light) override;

private:
	std::vector<const ISceneNode3D*> m_NodesList;
	std::vector<SModelElement>     m_ModelsList;
	std::vector<SGeometryElement>  m_GeometryList;
	std::vector<SLightElement>     m_LightList;
};



//
// Templated
//
template <typename TNode = ISceneNode3D, typename TModel = IModel, typename TGeometry = IGeometry>
class BuildRenderListPassTemplated
	: public BuildRenderListPass
{
public:
	BuildRenderListPassTemplated(IRenderDevice& RenderDevice, std::shared_ptr<IScene> scene)
		: BuildRenderListPass(RenderDevice, scene)
	{}
	virtual ~BuildRenderListPassTemplated()
	{}

	// IVisitor
	bool Visit(const ISceneNode3D* node) override
	{
		if (const TNode* nodeT = dynamic_cast<const TNode*>(node))
			return BuildRenderListPass::Visit(nodeT);
		return false;
	}
	bool Visit(const IModel* Model) override
	{
		if (const TModel* ModelT = dynamic_cast<const TModel*>(Model))
			return BuildRenderListPass::Visit(ModelT);
		return false;
	}
	bool Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override
	{
		if (const TGeometry* GeometryT = dynamic_cast<const TGeometry*>(Geometry))
			return BuildRenderListPass::Visit(GeometryT, Material, GeometryDrawArgs);
		return false;
	}
};