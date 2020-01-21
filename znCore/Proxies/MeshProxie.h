#pragma once

class ZN_API MeshProxie : public IMesh
{
public:
	MeshProxie(IMesh* _mesh);
	virtual ~MeshProxie();

	virtual void SetName(const std::string& Name);
	virtual std::string GetName() const;

	virtual void SetBounds(const BoundingBox& Bounds);
	virtual const BoundingBox& GetBounds() const;

	virtual void AddVertexBuffer(const BufferBinding& binding, IBuffer* buffer);
	virtual void SetVertexBuffer(IBuffer* buffer);
	virtual void SetIndexBuffer(IBuffer* buffer);

	virtual void SetPrimitiveTopology(PrimitiveTopology _topology);

	virtual void SetMaterial(const IMaterial* Material);
	virtual void AddMaterial(const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;

	virtual IGeometry* GetGeometry() const override;

	virtual bool Render(const RenderEventArgs* renderEventArgs, const IConstantBuffer* perObject, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;

	virtual bool Accept(IVisitor* visitor, SGeometryPartParams GeometryPartParams = SGeometryPartParams());

private:
	IMesh* m_Mesh;
};