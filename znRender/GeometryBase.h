#pragma once

class ZN_API GeometryBase 
	: public IGeometry
	, public Object
{
public:
	GeometryBase();
	virtual ~GeometryBase();

	virtual void                                    SetBounds(const BoundingBox& Bounds) override;
	virtual const BoundingBox&                      GetBounds() const override;

	virtual void                                    AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer) override;
	virtual void                                    SetVertexBuffer(std::shared_ptr<IBuffer> buffer) override;
	virtual void                                    SetIndexBuffer(std::shared_ptr<IBuffer> buffer) override;

	virtual bool                                    Accept(IVisitor* visitor, const IConstantBuffer* PerObject, const std::unordered_map<SShaderType, std::shared_ptr<IShader>>& ShadersMap, const IMaterial* Material, const SGeometryPartParams& GeometryPartParams) override;

protected:
	BoundingBox                                     m_Bounds;
	std::unordered_map<BufferBinding, std::shared_ptr<IBuffer>> m_VertexBuffers;
    std::shared_ptr<IBuffer>                        m_VertexBuffer;
    std::shared_ptr<IBuffer>                        m_pIndexBuffer;
};
