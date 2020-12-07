#pragma once

class ZN_API GeometryBase 
	: public IGeometry
	, public Object
{
public:
	GeometryBase(IRenderDevice& RenderDevice);
	virtual ~GeometryBase();

	// IGeometry
	virtual void         SetBounds(const BoundingBox& Bounds) override;
	virtual BoundingBox  GetBounds() const override;

	virtual void         AddVertexBuffer(const BufferBinding& binding, const std::shared_ptr<IBuffer>& VertexBuffer) override;
	virtual void         SetVertexBuffer(const std::shared_ptr<IBuffer>& GlobalVertexBuffer) override;
	virtual void         SetIndexBuffer(const std::shared_ptr<IBuffer>& IndexBuffer) override;

	void                 SetPrimitiveTopology(PrimitiveTopology Topology) override;
	PrimitiveTopology    GetPrimitiveTopology() const override;

	virtual void         Accept(IVisitor* visitor, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

	// IObjectLoadSave
	void                 Load(const std::shared_ptr<IByteBuffer>& ByteBuffer) override;
	void                 Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) const override;
	void                 Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void                 Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

protected:
	SGeometryDrawArgs    FixGeometryDrawArgs(const SGeometryDrawArgs& GeometryDrawArgs) const;

protected:
	BoundingBox               m_Bounds;
	BuffersMap                m_VertexBuffers;
	std::shared_ptr<IBuffer>  m_VertexBuffer;
	std::shared_ptr<IBuffer>  m_IndexBuffer;
	PrimitiveTopology         m_PrimitiveTopology;

private: // Link to parent d3d11 device
	IRenderDevice& m_RenderDevice;
};
