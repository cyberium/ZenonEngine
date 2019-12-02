#pragma once

class OW_ENGINE_API MeshBase : public IMesh, public Object
{
    typedef std::map<BufferBinding, std::shared_ptr<IBuffer>> BufferMap;
public:
	MeshBase();
	virtual ~MeshBase();

	virtual void                                    AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer) override;
	virtual void                                    SetVertexBuffer(std::shared_ptr<IBuffer> buffer) override;
	virtual void                                    SetIndexBuffer(std::shared_ptr<IBuffer> buffer) override;

	virtual void                                    SetPrimitiveTopology(PrimitiveTopology _topology) = 0;

	virtual void                                    SetMaterial(std::shared_ptr<const IMaterial> material) override final;
	virtual std::shared_ptr<const IMaterial>        GetMaterial() const override final;

	virtual bool                                    Accept(IVisitor* visitor, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0);

protected:
    BufferMap                                       m_VertexBuffers;
    std::shared_ptr<IBuffer>                        m_VertexBuffer;
    std::shared_ptr<IBuffer>                        m_pIndexBuffer;
    std::shared_ptr<const IMaterial>                m_pMaterial;
};
