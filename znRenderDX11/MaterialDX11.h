#pragma once

class OW_ENGINE_API MaterialDX11 : public MaterialImpl
{
public:
	MaterialDX11(IRenderDevice* renderDevice, size_t Size);
	virtual ~MaterialDX11();

    // MaterialImpl
    void Bind(const ShaderMap& shaders) const override final;
    void Unbind(const ShaderMap& shaders) const override final;


private:
	void BindForShader(const Shader* shader) const;
	void UnbindForShader(const Shader* shader) const;
};
