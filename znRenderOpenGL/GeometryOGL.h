#pragma once

class ZN_API GeometryOGL 
	: public GeometryBase
{
public:
	GeometryOGL();
	virtual ~GeometryOGL();

	void											AddVertexBuffer(const BufferBinding& binding, IBuffer* buffer) override;
	void											SetVertexBuffer(IBuffer* buffer) override;
	void											SetIndexBuffer(IBuffer* buffer) override;

	void											SetPrimitiveTopology(PrimitiveTopology _topology) override final;

	virtual bool                                    Render(const RenderEventArgs* renderArgs, const IConstantBuffer* PerObject, const std::unordered_map<EShaderType, IShader*>& ShadersMap, const IMaterial* Material, const SGeometryDrawArgs& GeometryDrawArgs) const override;
	virtual bool                                    RenderInstanced(const RenderEventArgs* renderArgs, const IStructuredBuffer* InstancesBuffer, const std::unordered_map<EShaderType, IShader*>& ShadersMap, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs) const override;


private:
	void                                            Commit(const IShader* _shader) const;

private:
	GLuint                                          m_GLObj;
	GLuint                                          m_PrimitiveTopology;

	mutable bool                                            m_bIsDirty;
};