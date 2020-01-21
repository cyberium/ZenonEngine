#pragma once

class MaterialOGL : public MaterialBase
{
public:
	MaterialOGL(IRenderDevice* RenderDeviceOGL, size_t Size);
	virtual ~MaterialOGL();

    void                                            SetShader(EShaderType type, IShader* pShader) override;

    // MaterialBase
	void                                            Bind(const ShaderMap& shaders) const override final;
	void                                            Unbind(const ShaderMap& shaders) const override final;

private:
	void											BindForShader(const IShader* shader) const;
	void											UnbindForShader(const IShader* shader) const;

private:
	uint32                                          m_GLProgramPipeline;
};