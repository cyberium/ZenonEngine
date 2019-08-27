#pragma once

// FORWARD BEGIN
class ConstantBuffer;
class IRenderDevice;
// FORWARD END

#include "Material.h"

class MaterialImpl : public IMaterial
{
public:
	MaterialImpl(IRenderDevice* renderDevice, size_t Size);
	virtual ~MaterialImpl();

	virtual void SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader);
	virtual std::shared_ptr<Shader> GetShader(Shader::ShaderType type) const;
	virtual const ShaderMap& GetShaders() const;

	virtual std::shared_ptr<Texture> GetTexture(uint8 ID) const;
	virtual void SetTexture(uint8 ID, std::shared_ptr<Texture> texture);

    virtual std::shared_ptr<SamplerState> GetSampler(uint8 ID) const;
    virtual void SetSampler(uint8 ID, std::shared_ptr<SamplerState> samplerState);

	virtual void Bind() const;
	virtual void Unbind() const;

	virtual void SetWrapper(std::weak_ptr<IMaterial> _wrapper) override;
	virtual void UpdateConstantBuffer() const override;
	virtual void UpdateConstantBuffer(const void* _data, size_t size) const override;
	virtual void MarkConstantBufferDirty() override;

protected:
	ShaderMap                        m_Shaders;

	TextureMap                       m_Textures;
    SamplersMap                      m_Samplers;
	std::shared_ptr<ConstantBuffer>  m_pConstantBuffer;

	std::weak_ptr<IMaterial>          m_Wrapper;
	IRenderDevice*                   m_RenderDevice;
	mutable bool                     m_Dirty;
};