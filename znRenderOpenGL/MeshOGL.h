#pragma once

class ShaderOGL;

class MeshOGL : public IMesh
{
    typedef std::map<BufferBinding, std::shared_ptr<IBuffer> > BufferMap;
public:
	MeshOGL();
	virtual ~MeshOGL();

	virtual void AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer) override final;
    virtual void SetVertexBuffer(std::shared_ptr<IBuffer> buffer) override final;
	virtual void SetIndexBuffer(std::shared_ptr<IBuffer> buffer) override final;

	virtual void SetPrimitiveTopology(PrimitiveTopology _topology) override final;

	virtual void SetMaterial(std::shared_ptr<const Material> material) override final;
	virtual std::shared_ptr<const Material> GetMaterial() const override final;

	virtual bool Render(const RenderEventArgs* renderArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0) override final;

	virtual bool Accept(std::shared_ptr<IVisitor> visitor, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0) override final;

private:
	GLuint m_GLObj;

private:
	void Commit(std::weak_ptr<ShaderOGL> _shader);

private:
	BufferMap m_VertexBuffers;
    std::shared_ptr<IBuffer> m_VertexBuffer;
	size_t m_VertexCount;
	std::shared_ptr<IBuffer> m_pIndexBuffer;
	GLuint m_PrimitiveTopology;
	std::shared_ptr<const Material> m_pMaterial;

	bool m_bIsDirty;
};