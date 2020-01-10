#pragma once

#include "Base3DPass.h"

class BuildRenderListPass
	: public Base3DPass
{
public:
	struct SRenderListElement
	{
		SRenderListElement(const ISceneNode* Node, const IGeometry* Geometry, const IMaterial* Material, const std::unordered_map<SShaderType, std::shared_ptr<IShader>>& ShadersMap, const SGeometryPartParams GeometryPartParams)
			: Node(Node)
			, Geometry(Geometry)
			, Material(Material)
			, ShadersMap(ShadersMap)
			, GeometryPartParams(GeometryPartParams)
		{}

		const ISceneNode* Node;
		const IGeometry* Geometry;
		const IMaterial* Material;
		const std::unordered_map<SShaderType, std::shared_ptr<IShader>> ShadersMap;
		SGeometryPartParams GeometryPartParams;
	};

public:
	BuildRenderListPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline);
	virtual ~BuildRenderListPass();


	// IRenderPass
	virtual void PreRender(RenderEventArgs& e) override;
	virtual void PostRender(RenderEventArgs& e) override;

	// IVisitor
	virtual bool Visit3D(ISceneNode* node) override;
	virtual bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	virtual bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;

private:
	const ISceneNode* m_CurrentSceneNode;
	std::vector<SRenderListElement> m_RenderList;
};