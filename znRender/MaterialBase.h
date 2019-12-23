#pragma once

class OW_ENGINE_API MaterialBase : public IMaterial
{
public:
	MaterialBase(std::weak_ptr<IRenderDevice> renderDevice, size_t Size);
	virtual ~MaterialBase();

	virtual void SetName(const std::string& Name);
	virtual std::string GetName() const;

	virtual void SetShader(IShader::ShaderType type, std::shared_ptr<IShader> pShader);
	virtual std::shared_ptr<IShader> GetShader(IShader::ShaderType type) const;
	virtual const ShaderMap& GetShaders() const;

	virtual std::shared_ptr<ITexture> GetTexture(uint8 ID) const;
	virtual void SetTexture(uint8 ID, std::shared_ptr<ITexture> texture);

    virtual std::shared_ptr<ISamplerState> GetSampler(uint8 ID) const;
    virtual void SetSampler(uint8 ID, std::shared_ptr<ISamplerState> samplerState);

	virtual void Bind(const ShaderMap& shaders) const;
	virtual void Unbind(const ShaderMap& shaders) const;

	virtual void SetWrapper(std::weak_ptr<IMaterial> _wrapper) override;
	virtual void UpdateConstantBuffer() const override;
	virtual void UpdateConstantBuffer(const void* _data, size_t size) const override;
	virtual void MarkConstantBufferDirty() override;

protected:


protected:
	std::string                      m_Name;

	ShaderMap                        m_Shaders;

	TextureMap                       m_Textures;
    SamplersMap                      m_Samplers;
	std::shared_ptr<IConstantBuffer> m_pConstantBuffer;

	std::weak_ptr<IMaterial>         m_Wrapper;
	std::weak_ptr<IRenderDevice>     m_RenderDevice;
	mutable bool                     m_Dirty;
};