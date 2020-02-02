#pragma once

class ZN_API MaterialProxie : public IMaterial
{
public:
	MaterialProxie(std::shared_ptr<IMaterial> _materal);
	virtual ~MaterialProxie() override;

	// IMaterial
	virtual void SetName(const std::string& Name) override;
	virtual std::string GetName() const override;

	virtual void SetTexture(uint8 type, const std::shared_ptr<ITexture> texture) override;
	virtual const std::shared_ptr<ITexture>& GetTexture(uint8 ID) const override;
	
	virtual void SetSampler(uint8 ID, const std::shared_ptr<ISamplerState> samplerState) override;
    virtual const std::shared_ptr<ISamplerState>& GetSampler(uint8 ID) const override;
    
	virtual void Bind(const ShaderMap& shaders) const override;
	virtual void Unbind(const ShaderMap& shaders) const override;

	virtual void SetWrapper(IMaterial* _wrapper) override;
	virtual void UpdateConstantBuffer() const override;
	virtual void UpdateConstantBuffer(const void* _data, size_t size) const override;
	virtual void MarkConstantBufferDirty() override;

private:
	std::shared_ptr<IMaterial> m_Material;
};