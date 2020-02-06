#pragma once

class ZN_API GeometryBase 
	: public IGeometry
	, public Object
{
public:
	GeometryBase();
	virtual ~GeometryBase();

	// IGeometry
	virtual void         SetBounds(const BoundingBox& Bounds) override;
	virtual BoundingBox  GetBounds() const override;

	virtual void         AddVertexBuffer(const BufferBinding& binding, const std::shared_ptr<IBuffer>& VertexBuffer) override;
	virtual void         SetVertexBuffer(const std::shared_ptr<IBuffer>& GlobalVertexBuffer) override;
	virtual void         SetIndexBuffer(const std::shared_ptr<IBuffer>& IndexBuffer) override;

	virtual void         Accept(IVisitor* visitor, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

protected:
	void                 BindVertexBuffersToVertexShader(const IShader* VertexShader) const;
	void                 UnbindVertexBuffersFromVertexShader(const IShader* VertexShader) const;
	SGeometryDrawArgs    FixGeometryDrawArgs(const SGeometryDrawArgs& GeometryDrawArgs) const;
	SGeometryDrawInstancedArgs FixGeometryDrawInstancedArgs(const SGeometryDrawInstancedArgs& GeometryDrawInstancedArgs) const;

protected:
	BoundingBox               m_Bounds;
	BuffersMap                m_VertexBuffers;
	std::shared_ptr<IBuffer>  m_VertexBuffer;
	std::shared_ptr<IBuffer>  m_pIndexBuffer;
};
