#pragma once

class MeshOGL : public IMesh
{
    typedef IMesh base;
public:
                                                    MeshOGL();
	virtual                                         ~MeshOGL();

    virtual void                                    AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer) override;
    virtual void                                    SetVertexBuffer(std::shared_ptr<IBuffer> buffer) override;
    virtual void                                    SetIndexBuffer(std::shared_ptr<IBuffer> buffer) override;

	virtual void                                    SetPrimitiveTopology(PrimitiveTopology _topology) override final;

	virtual bool                                    Render(const RenderEventArgs* renderArgs, const IConstantBuffer* perObject, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0) override final;

private:
	void                                            Commit(std::weak_ptr<Shader> _shader);

private:
    GLuint                                          m_GLObj;
	GLuint                                          m_PrimitiveTopology;

	bool                                            m_bIsDirty;
};