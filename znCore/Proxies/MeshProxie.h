#pragma once

class ZN_API MeshProxie : public IMesh
{
public:
	MeshProxie(std::shared_ptr<IMesh> _mesh);
	virtual ~MeshProxie();

	virtual void SetName(const std::string& Name);
	virtual std::string GetName() const;

	virtual void SetBounds(const BoundingBox& Bounds);
	virtual const BoundingBox& GetBounds() const;

	virtual void AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer);
	virtual void SetVertexBuffer(std::shared_ptr<IBuffer> buffer);
	virtual void SetIndexBuffer(std::shared_ptr<IBuffer> buffer);

	virtual void SetPrimitiveTopology(PrimitiveTopology _topology);

	virtual void SetMaterial(std::shared_ptr<const IMaterial> Material);
	virtual void AddMaterial(std::shared_ptr<const IMaterial> Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;

	virtual bool Render(const RenderEventArgs* renderEventArgs, const IConstantBuffer* perObject, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;

	virtual bool Accept(IVisitor* visitor, SGeometryPartParams GeometryPartParams = SGeometryPartParams());

private:
	std::shared_ptr<IMesh> m_Mesh;
};