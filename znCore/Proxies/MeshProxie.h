#pragma once

class ZN_API MeshProxie : public IMesh
{
public:
	MeshProxie(std::shared_ptr<IMesh> _mesh);
	virtual ~MeshProxie();

	// IMesh
	virtual void SetName(const std::string& Name);
	virtual std::string GetName() const;

	virtual void SetBounds(const BoundingBox& Bounds);
	virtual const BoundingBox& GetBounds() const;

	virtual void AddVertexBuffer(const BufferBinding& binding, const std::shared_ptr<IBuffer> buffer);
	virtual void SetVertexBuffer(const std::shared_ptr<IBuffer> buffer);
	virtual void SetIndexBuffer(const std::shared_ptr<IBuffer> buffer);

	virtual void SetPrimitiveTopology(PrimitiveTopology _topology);

	virtual void SetMaterial(const std::shared_ptr<IMaterial> Material);
	virtual void AddMaterial(const std::shared_ptr<IMaterial> Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

	virtual IGeometry& GetGeometry() const override;

	virtual bool Render(const RenderEventArgs& renderEventArgs, const SGeometryDrawArgs& GeometryDrawArgs = SGeometryDrawArgs()) const override;

	virtual bool Accept(IVisitor* visitor, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs());

private:
	std::shared_ptr<IMesh> m_Mesh;
};