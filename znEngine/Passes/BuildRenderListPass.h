#pragma once

#if 0

class BuildRenderListPass
	: public ScenePassPipelined
{
public:
	struct SRenderListElement
	{
		SRenderListElement(const ISceneNode* Node, const IGeometry* Geometry, const IMaterial* Material, const SGeometryPartParams GeometryPartParams)
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

public:
	BuildRenderListPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline);
	virtual ~BuildRenderListPass();


	// IRenderPass
	virtual void Render(RenderEventArgs& e) override;

	// IVisitor
	virtual bool Visit3D(ISceneNode* node) override;
	virtual bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	virtual bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;

protected:
	__declspec(align(16)) struct PerObject3D
	{
		glm::mat4 Model;
		glm::mat4 View;
		glm::mat4 Projection;
	};
	std::vector<PerObject3D*>                       m_PerObjectData;
	std::vector <std::shared_ptr<IConstantBuffer>>  m_PerObjectConstantBuffer;

private:
	const ISceneNode* m_CurrentSceneNode;
	std::vector<SRenderListElement> m_RenderList;
};

#endif
