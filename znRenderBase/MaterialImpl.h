#pragma once

class IRenderDevice;

#include "Material.h"

// A material class is used to wrap the shaders and to manage the shader parameters.
class MaterialImpl : public Material
{
public:
	MaterialImpl(IRenderDevice* renderDevice);
	virtual ~MaterialImpl();

	virtual void SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader);
	virtual std::shared_ptr<Shader> GetShader(Shader::ShaderType type) const;
	virtual const ShaderMap& GetShaders() const;

	virtual std::shared_ptr<Texture> GetTexture(uint8 ID) const;
	virtual void SetTexture(uint8 type, std::shared_ptr<Texture> texture);

	virtual void Bind() const;
	virtual void Unbind() const;

	virtual void SetWrapper(std::weak_ptr<Material> _wrapper) override;
	virtual void CreateConstantBuffer(const void* data, size_t size) override;
	virtual void UpdateConstantBuffer() const override;
	virtual void UpdateConstantBuffer(const void* _data, size_t size) const override;
	virtual void MarkConstantBufferDirty() override;

protected:
	ShaderMap                        m_Shaders;

	TextureMap                       m_Textures;
	std::shared_ptr<ConstantBuffer>  m_pConstantBuffer;

	std::weak_ptr<Material>          m_Wrapper;
	IRenderDevice*                   m_RenderDevice;
	mutable bool                     m_Dirty;
};