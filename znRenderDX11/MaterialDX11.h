#pragma once

class OW_ENGINE_API MaterialDX11 : public MaterialBase
{
public:
	MaterialDX11(IRenderDevice* renderDevice, size_t Size);
	virtual ~MaterialDX11();

    // MaterialBase
    void Bind(const ShaderMap& shaders) const override final;
    void Unbind(const ShaderMap& shaders) const override final;


private:
	void BindForShader(const IShader* shader) const;
	void UnbindForShader(const IShader* shader) const;
};
