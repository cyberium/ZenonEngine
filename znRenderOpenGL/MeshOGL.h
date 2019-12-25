#pragma once

class ZN_API MeshOGL : public MeshBase
{
public:
    MeshOGL();
	virtual ~MeshOGL();

    void											AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer) override;
    void											SetVertexBuffer(std::shared_ptr<IBuffer> buffer) override;
    void											SetIndexBuffer(std::shared_ptr<IBuffer> buffer) override;

	void											SetPrimitiveTopology(PrimitiveTopology _topology) override final;

	bool											Render(const RenderEventArgs* renderArgs, const IConstantBuffer* perObject, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0) override final;

private:
	void                                            Commit(std::weak_ptr<IShader> _shader);

private:
    GLuint                                          m_GLObj;
	GLuint                                          m_PrimitiveTopology;

	bool                                            m_bIsDirty;
};