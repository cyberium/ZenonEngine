#pragma once

class ZN_API GeometryProxie
	: public IGeometry
{
public:
	GeometryProxie(const std::shared_ptr<IGeometry>& Geometry);
	virtual ~GeometryProxie();

	// IGeometry
	virtual void SetBounds(const BoundingBox& Bounds) override;
	virtual BoundingBox GetBounds() const override;

	void AddVertexBuffer(const BufferBinding& Binding, const std::shared_ptr<IBuffer>& VertexBuffer) override;

	void SetVertexBuffer(const std::shared_ptr<IBuffer>& GlobalVertexBuffer) override final;
	void SetIndexBuffer(const std::shared_ptr<IBuffer>& IndexBuffer) override final;

	void SetPrimitiveTopology(PrimitiveTopology Topology) override final;

	virtual bool Render(const RenderEventArgs& RenderEventArgs, const IShader* VertexShader, const SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) const override;
	virtual bool RenderInstanced(const RenderEventArgs& RenderEventArgs, const IShader* VertexShader, const SGeometryDrawInstancedArgs GeometryDrawInstancedArgs = SGeometryDrawInstancedArgs()) const override;

	virtual void Accept(IVisitor* visitor, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

private:
	std::shared_ptr<IGeometry> m_Geometry;
};