#pragma once

class ZN_API GeometryBase 
	: public IGeometry
	, public Object
{
public:
	GeometryBase();
	virtual ~GeometryBase();

	virtual size_t                                  GetHash() const override;

	virtual void                                    SetBounds(const BoundingBox& Bounds) override;
	virtual const BoundingBox&                      GetBounds() const override;

	virtual void                                    AddVertexBuffer(const BufferBinding& binding, IBuffer* buffer) override;
	virtual void                                    SetVertexBuffer(IBuffer* buffer) override;
	virtual void                                    SetIndexBuffer(IBuffer* buffer) override;

	virtual bool                                    Accept(IVisitor* visitor, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;

protected:
	void UpdateHash();

protected:
	BoundingBox                                     m_Bounds;
	std::unordered_map<BufferBinding, IBuffer*> m_VertexBuffers;
    IBuffer*                        m_VertexBuffer;
    IBuffer*                        m_pIndexBuffer;

private:
	size_t                                          m_Hash;
};
