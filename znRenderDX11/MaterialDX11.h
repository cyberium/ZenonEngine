#pragma once

class ZN_API MaterialDX11 
	: public MaterialBase
{
public:
	MaterialDX11(IRenderDeviceDX11& RenderDeviceDX11);
	virtual ~MaterialDX11();

    // IMaterial
    void Bind(const ShaderMap& shaders) const override final;
    void Unbind(const ShaderMap& shaders) const override final;

private:
	void BindForShader(const IShader* shader) const;
	void UnbindForShader(const IShader* shader) const;
};
