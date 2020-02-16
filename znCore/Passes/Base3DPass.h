#pragma once

#include "ScenePassPipelined.h"

__declspec(align(16)) struct ZN_API PerObject3D
{
	glm::mat4 Model;
};


class ZN_API Base3DPass 
	: public ScenePassPipelined
{
public:
	Base3DPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> scene);
	virtual ~Base3DPass();

	// IVisitor
	virtual bool Visit(const ISceneNode3D* node) override;
	virtual bool Visit(const IModel* Model) override;
	virtual bool Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

protected:
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
	IShaderParameter* m_PerObjectParameter;
};



//
// Templated
//
template <typename TNode = ISceneNode3D, typename TModel = IModel, typename TGeometry = IGeometry>
class ZN_API Base3DPassTemplated
	: public Base3DPass
{
public:
	Base3DPassTemplated(IRenderDevice& RenderDevice, std::shared_ptr<IScene> scene)
		: Base3DPass(RenderDevice, scene)
	{}
	virtual ~Base3DPassTemplated()
	{}

	// IVisitor
	inline bool Visit(const ISceneNode3D* node) override
	{
		if (const TNode* nodeT = dynamic_cast<const TNode*>(node))
			return Base3DPass::Visit(nodeT);
		return false;
	}
	inline bool Visit(const IModel* Model) override
	{
		if (const TModel* ModelT = dynamic_cast<const TModel*>(Model))
			return Base3DPass::Visit(ModelT);
		return false;
	}
	inline bool Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override
	{
		if (const TGeometry* GeometryT = dynamic_cast<const TGeometry*>(Geometry))
			return Base3DPass::Visit(GeometryT, Material, GeometryDrawArgs);
		return false;
	}
};