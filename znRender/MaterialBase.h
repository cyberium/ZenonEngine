#pragma once

class ZN_API MaterialBase : public IMaterial
{
public:
	MaterialBase(IRenderDevice* renderDevice, size_t Size);
	virtual ~MaterialBase();

	virtual void SetName(const std::string& Name);
	virtual std::string GetName() const;

	virtual void SetShader(EShaderType type, IShader* pShader);
	virtual IShader* GetShader(EShaderType type) const;
	virtual const ShaderMap& GetShaders() const;

	virtual ITexture* GetTexture(uint8 ID) const;
	virtual void SetTexture(uint8 ID, ITexture* texture);

    virtual ISamplerState* GetSampler(uint8 ID) const;
    virtual void SetSampler(uint8 ID, ISamplerState* samplerState);

	virtual void Bind(const ShaderMap& shaders) const;
	virtual void Unbind(const ShaderMap& shaders) const;

	virtual void SetWrapper(IMaterial* _wrapper) override;
	virtual void UpdateConstantBuffer() const override;
	virtual void UpdateConstantBuffer(const void* _data, size_t size) const override;
	virtual void MarkConstantBufferDirty() override;

protected:


protected:
	std::string                      m_Name;

	ShaderMap                        m_Shaders;
	TextureMap                       m_Textures;
    SamplersMap                      m_Samplers;

	IConstantBuffer* m_pConstantBuffer;

	IMaterial*         m_Wrapper;
	IRenderDevice*     m_RenderDevice;
	mutable bool                     m_Dirty;
};