#pragma once

class ZN_API CSceneCreateTypelessListPass
	: public ScenePass
{
public:
	struct ZN_API SNodeElement
	{
		SNodeElement(const ISceneNode* SceneNode)
			: SceneNode(SceneNode)
		{}

		const ISceneNode* SceneNode;
	};

	struct ZN_API SModelElement
	{
		SModelElement(const ISceneNode* SceneNode, const IModel* Model)
			: SceneNode(SceneNode)
			, Model(Model)
		{}

		const ISceneNode* SceneNode;
		const IModel* Model;
	};

	struct ZN_API SGeometryElement
	{
		SGeometryElement(const ISceneNode* Node, const IModel* Model, const IGeometry* Geometry, const IMaterial* Material, const SGeometryDrawArgs GeometryDrawArgs)
			: Node(Node)
			, Model(Model)
			, Geometry(Geometry)
			, Material(Material)
			, GeometryDrawArgs(GeometryDrawArgs)
		{}

		const ISceneNode* Node;
		const IModel* Model;
		const IGeometry* Geometry;
		const IMaterial* Material;
		const SGeometryDrawArgs GeometryDrawArgs;
	};

	struct ZN_API SLightElement
	{
		SLightElement(const ISceneNode* SceneNode, const ILight3D* Light)
			: SceneNode(SceneNode)
			, Light(Light)
		{}

		const ISceneNode* SceneNode;
		const ILight3D* Light;
	};

	struct ZN_API SParticleSystemElement
	{
		SParticleSystemElement(const ISceneNode* SceneNode, const IParticleSystem* ParticleSystem)
			: SceneNode(SceneNode)
			, ParticleSystem(ParticleSystem)
		{}

		const ISceneNode* SceneNode;
		const IParticleSystem* ParticleSystem;
	};



public:
	CSceneCreateTypelessListPass(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CSceneCreateTypelessListPass();

	const std::vector<SNodeElement>& GetNodesList() const;
	const std::vector<SModelElement>& GetModelsList() const;
	const std::vector<SGeometryElement>& GetGeometryList() const;
	const std::vector<SLightElement>& GetLightList() const;

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



//
// Templated
//
template <typename TNode = ISceneNode, typename TModel = IModel, typename TGeometry = IGeometry>
class CSceneCreateTypelessListPassTemplated
	: public CSceneCreateTypelessListPass
{
public:
	CSceneCreateTypelessListPassTemplated(IRenderDevice& RenderDevice, const std::shared_ptr<IScene>& scene)
		: CSceneCreateTypelessListPass(RenderDevice, scene)
	{}
	virtual ~CSceneCreateTypelessListPassTemplated()
	{}

	// IVisitor
	EVisitResult Visit(const ISceneNode* SceneNode) override
	{
		if (const TNode* SceneNodeT = dynamic_cast<const TNode*>(SceneNode))
			return CSceneCreateTypelessListPass::Visit(SceneNodeT);
		return EVisitResult::Block;
	}
	EVisitResult Visit(const IModel* Model) override
	{
		if (const TModel* ModelT = dynamic_cast<const TModel*>(Model))
			return CSceneCreateTypelessListPass::Visit(ModelT);
		return EVisitResult::Block;
	}
	EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override
	{
		if (const TGeometry* GeometryT = dynamic_cast<const TGeometry*>(Geometry))
			return CSceneCreateTypelessListPass::Visit(GeometryT, Material, GeometryDrawArgs);
		return EVisitResult::Block;
	}
};