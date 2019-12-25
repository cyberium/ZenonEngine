#pragma once

class ZN_API MeshBase : public IMesh, public Object
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

	virtual void                                    SetPrimitiveTopology(PrimitiveTopology _topology) = 0;

	virtual void                                    SetMaterial(std::shared_ptr<const IMaterial> material) override final;
	virtual std::shared_ptr<const IMaterial>        GetMaterial() const override final;

	virtual bool                                    Accept(IVisitor* visitor, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0);

protected:
	std::string                                     m_Name;
	BoundingBox                                     m_Bounds;
    BufferMap                                       m_VertexBuffers;
    std::shared_ptr<IBuffer>                        m_VertexBuffer;
    std::shared_ptr<IBuffer>                        m_pIndexBuffer;
    std::shared_ptr<const IMaterial>                m_pMaterial;
};
