#pragma once

class ZN_API MaterialBase 
	: public IMaterial
{
public:
	MaterialBase(IRenderDevice& renderDevice, size_t Size);
	virtual ~MaterialBase();

	// IMaterial
	virtual void SetName(const std::string& Name);
	virtual std::string GetName() const;

	virtual void SetShader(EShaderType type, const std::shared_ptr<IShader> pShader);
	virtual const std::shared_ptr<IShader>& GetShader(EShaderType type) const;
	virtual const ShaderMap& GetShaders() const;

	virtual void SetTexture(uint8 ID, const std::shared_ptr<ITexture> texture);
	virtual const std::shared_ptr<ITexture>& GetTexture(uint8 ID) const;
	
	virtual void SetSampler(uint8 ID, const std::shared_ptr<ISamplerState> samplerState);
    virtual const std::shared_ptr<ISamplerState>& GetSampler(uint8 ID) const;

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

	std::shared_ptr<IConstantBuffer> m_pConstantBuffer;

	IMaterial*         m_Wrapper;
	mutable bool                     m_Dirty;

private:
	IRenderDevice&     m_RenderDevice;
};