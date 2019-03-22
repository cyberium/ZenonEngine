#pragma once

class MaterialWrapper : public Material
{
public:
	MaterialWrapper(std::shared_ptr<Material> _materal);
	virtual ~MaterialWrapper();

	virtual void SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader);
	virtual std::shared_ptr<Shader> GetShader(Shader::ShaderType type) const;
	virtual const ShaderMap& GetShaders() const;

	virtual std::shared_ptr<Texture> GetTexture(uint8 ID) const;
	virtual void SetTexture(uint8 type, std::shared_ptr<Texture> texture);

	virtual void Bind() const;
	virtual void Unbind() const;

	virtual void SetWrapper(std::weak_ptr<Material> _wrapper);
	virtual void CreateConstantBuffer(const void* data, size_t size);
	virtual void UpdateConstantBuffer() const;
	virtual void UpdateConstantBuffer(const void* _data, size_t size) const;
	virtual void MarkConstantBufferDirty();

private:
	std::shared_ptr<Material> m_Material;
};