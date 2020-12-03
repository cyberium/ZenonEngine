#pragma once

#include "Types/RenderTypes.h"

// FORWARD BEGIN
class RenderEventArgs;
ZN_INTERFACE IRenderTarget;
ZN_INTERFACE IPipelineState;
ZN_INTERFACE ISceneNode;
ZN_INTERFACE IModel;
ZN_INTERFACE IGeometry;
ZN_INTERFACE IMaterial;
ZN_INTERFACE ILight3D;
ZN_INTERFACE IParticleSystem;
// FORWARD END

ZN_INTERFACE ZN_API IRenderPass 
{
	virtual ~IRenderPass() {}

    virtual void SetEnabled(bool enabled) = 0;
    virtual bool IsEnabled() const = 0;

    virtual void PreRender(RenderEventArgs& e) = 0;
    virtual void Render(RenderEventArgs& e) = 0;
    virtual void PostRender(RenderEventArgs& e) = 0;
};


ZN_INTERFACE ZN_API IRenderPassPipelined 
	: public virtual IRenderPass
{
	virtual ~IRenderPassPipelined() {}

	virtual std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) = 0;
	virtual const IPipelineState& GetPipeline() const = 0;
};



ZN_INTERFACE ZN_API IRenderPassMultipipelinded
	: public virtual IRenderPass
{
	virtual ~IRenderPassMultipipelinded() {}

	virtual void Configure(std::shared_ptr<IRenderTarget> OutputRenderTarget) = 0;
	virtual const std::vector<std::shared_ptr<IPipelineState>>& GetPipelines() const = 0;
};


ZN_INTERFACE ZN_API IRenderPassCreateTypelessList
	: public virtual IRenderPass
{
		struct ZN_API SNodeElement
	{
		SNodeElement(const std::shared_ptr<const ISceneNode>& SceneNode)
			: SceneNode(SceneNode)
		{}

		const std::shared_ptr<const ISceneNode> SceneNode;
	};

	struct ZN_API SModelElement
	{
		SModelElement(const std::shared_ptr<const ISceneNode>& SceneNode, const std::shared_ptr<const IModel>& Model)
			: SceneNode(SceneNode)
			, Model(Model)
		{}

		const std::shared_ptr<const ISceneNode> SceneNode;
		const std::shared_ptr<const IModel> Model;
	};

	struct ZN_API SGeometryElement
	{
		SGeometryElement(const std::shared_ptr<const ISceneNode>& Node, const std::shared_ptr<const IModel>& Model, const std::shared_ptr<const IGeometry>& Geometry, const std::shared_ptr<const IMaterial>& Material, const SGeometryDrawArgs GeometryDrawArgs)
			: Node(Node)
			, Model(Model)
			, Geometry(Geometry)
			, Material(Material)
			, GeometryDrawArgs(GeometryDrawArgs)
		{}

		const std::shared_ptr<const ISceneNode> Node;
		const std::shared_ptr<const IModel> Model;
		const std::shared_ptr<const IGeometry> Geometry;
		const std::shared_ptr<const IMaterial> Material;
		const SGeometryDrawArgs GeometryDrawArgs;
	};

	struct ZN_API SLightElement
	{
		SLightElement(const std::shared_ptr<const ISceneNode>& SceneNode, const std::shared_ptr<const ILight3D>& Light)
			: SceneNode(SceneNode)
			, Light(Light)
		{}

		const std::shared_ptr<const ISceneNode> SceneNode;
		const std::shared_ptr<const ILight3D> Light;
	};

	struct ZN_API SParticleSystemElement
	{
		SParticleSystemElement(const std::shared_ptr<const ISceneNode>& SceneNode, const std::shared_ptr<IParticleSystem>& ParticleSystem)
			: SceneNode(SceneNode)
			, ParticleSystem(ParticleSystem)
		{}

		const std::shared_ptr<const ISceneNode> SceneNode;
		const std::shared_ptr<const IParticleSystem> ParticleSystem;
	};

	virtual ~IRenderPassCreateTypelessList() {}

	virtual const std::vector<SNodeElement>& GetNodesList() const = 0;
	virtual const std::vector<SModelElement>& GetModelsList() const = 0;
	virtual const std::vector<SGeometryElement>& GetGeometryList() const = 0;
	virtual const std::vector<SLightElement>& GetLightList() const = 0;
};


/*

//
// For plugins
//
ZN_INTERFACE ZN_API IRenderPassCreator
{
	virtual ~IRenderPassCreator() {}

	virtual size_t                       GetRenderPassCount() const = 0;
	virtual std::string                  GetRenderPassName(size_t Index) const = 0;
	virtual std::shared_ptr<IRenderPass> CreateRenderPass(size_t Index, IRenderDevice& RenderDevice, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport, std::shared_ptr<IScene> Scene) const = 0;
};



ZN_INTERFACE ZN_API 
	__declspec(uuid("A0ED56A7-77B5-40E4-B479-1CA95CAAD96C")) IRenderPassFactory
	: public IManager
{
	virtual ~IRenderPassFactory() {}

	virtual void AddRenderPassCreator(std::shared_ptr<IRenderPassCreator> Creator) = 0;
	virtual void RemoveRenderPassCreator(std::shared_ptr<IRenderPassCreator> Creator) = 0;

	virtual std::shared_ptr<IRenderPass> CreateRenderPass(std::string RenderPassName, IRenderDevice& RenderDevice, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport, std::shared_ptr<IScene> Scene) const = 0;
};

*/
