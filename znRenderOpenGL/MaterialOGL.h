#pragma once

class MaterialOGL : public MaterialBase
{
public:
	MaterialOGL(IRenderDevice* renderDevice, size_t Size);
	virtual ~MaterialOGL();

    void                                            SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader) override;

    // MaterialBase
	void                                            Bind() const override final;
	void                                            Unbind() const override final;

private:
	uint32                                          m_GLProgramPipeline;
};