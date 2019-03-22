#pragma once

class ShaderOGL;

class MeshOGL : public IMesh
{
public:
	MeshOGL();
	virtual ~MeshOGL();

	virtual void AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer);
	virtual void SetIndexBuffer(std::shared_ptr<IBuffer> buffer);

	virtual void SetPrimitiveTopology(PrimitiveTopology _topology);

	virtual void SetMaterial(std::shared_ptr<const Material> material);
	virtual std::shared_ptr<const Material> GetMaterial() const;

	virtual bool Render(RenderEventArgs& renderArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0);

	virtual bool Accept(IVisitor& visitor);

private:
	GLuint m_GLObj;

private:
	void Commit(std::weak_ptr<ShaderOGL> _shader);

private:
	typedef std::map<BufferBinding, std::shared_ptr<IBuffer> > BufferMap;
	BufferMap m_VertexBuffers;
	size_t m_VertexCount;
	std::shared_ptr<IBuffer> m_pIndexBuffer;
	GLuint m_PrimitiveTopology;
	std::shared_ptr<const Material> m_pMaterial;

	bool m_bIsDirty;
};