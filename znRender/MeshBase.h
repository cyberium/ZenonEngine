#pragma once

#include "GeometryBase.h"

class ZN_API MeshBase 
	: public IMesh
	, public Object
{
public:
	MeshBase();
	virtual ~MeshBase();

	virtual void                                    SetName(const std::string& Name);
	virtual std::string                             GetName() const;

	virtual void                                    SetBounds(const BoundingBox& Bounds);
	virtual const BoundingBox&                      GetBounds() const;

	virtual void                                    AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer) override;
	virtual void                                    SetVertexBuffer(std::shared_ptr<IBuffer> buffer) override;
	virtual void                                    SetIndexBuffer(std::shared_ptr<IBuffer> buffer) override;

	void                                            SetMaterial(std::shared_ptr<const IMaterial> Material);
	void											AddMaterial(std::shared_ptr<const IMaterial> Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override final;
	std::vector<SRenderGeometryArgs>				GetMaterials() const override final;

	virtual bool                                    Accept(IVisitor* visitor, SGeometryPartParams GeometryPartParams);

protected:
	std::shared_ptr<GeometryBase>					m_Geometry;

	std::string                                     m_Name;
	std::vector<SRenderGeometryArgs>				m_MaterialForGeometryParts;
};
