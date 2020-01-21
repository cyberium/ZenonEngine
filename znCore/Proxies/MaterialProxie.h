#pragma once

class ZN_API MaterialProxie : public IMaterial
{
public:
	MaterialProxie(IMaterial* _materal);
	virtual ~MaterialProxie() override;

	virtual void SetName(const std::string& Name) override;
	virtual std::string GetName() const override;

	virtual void SetShader(EShaderType type, IShader* pShader) override;
	virtual IShader* GetShader(EShaderType type) const override;
	virtual const ShaderMap& GetShaders() const override;

	virtual ITexture* GetTexture(uint8 ID) const override;
	virtual void SetTexture(uint8 type, ITexture* texture) override;

    virtual ISamplerState* GetSampler(uint8 ID) const override;
    virtual void SetSampler(uint8 ID, ISamplerState* samplerState) override;

	virtual void Bind(const ShaderMap& shaders) const override;
	virtual void Unbind(const ShaderMap& shaders) const override;

	virtual void SetWrapper(IMaterial* _wrapper) override;
	virtual void UpdateConstantBuffer() const override;
	virtual void UpdateConstantBuffer(const void* _data, size_t size) const override;
	virtual void MarkConstantBufferDirty() override;

private:
	IMaterial* m_Material;
};