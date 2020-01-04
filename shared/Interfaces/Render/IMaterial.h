#pragma once

// FORWARD BEGIN
ZN_INTERFACE IShader;
ZN_INTERFACE ITexture;
ZN_INTERFACE ISamplerState;
// FORWARD END

ZN_INTERFACE ZN_API IMaterial : public std::enable_shared_from_this<IMaterial>
{
	typedef std::unordered_map<uint8, std::shared_ptr<ISamplerState>> SamplersMap;

	virtual ~IMaterial() {}

	virtual void SetName(const std::string& Name) = 0;
	virtual std::string GetName() const = 0;

	virtual void SetShader(SShaderType type, std::shared_ptr<IShader> pShader) = 0;
	virtual std::shared_ptr<IShader> GetShader(SShaderType type) const = 0;
	virtual const ShaderMap& GetShaders() const = 0;

	virtual std::shared_ptr<ITexture> GetTexture(uint8 ID) const = 0;
	virtual void SetTexture(uint8 ID, std::shared_ptr<ITexture> texture) = 0;

	virtual std::shared_ptr<ISamplerState> GetSampler(uint8 ID) const = 0;
	virtual void SetSampler(uint8 ID, std::shared_ptr<ISamplerState> samplerState) = 0;

	virtual void Bind(const ShaderMap& shaders) const = 0;
	virtual void Unbind(const ShaderMap& shaders) const = 0;

	virtual void SetWrapper(std::weak_ptr<IMaterial> _wrapper) = 0;
	virtual void UpdateConstantBuffer() const = 0;
	virtual void UpdateConstantBuffer(const void* _data, size_t size) const = 0;
	virtual void MarkConstantBufferDirty() = 0;
};

typedef std::map<std::string, std::shared_ptr<IMaterial> > MaterialNameMap;
typedef std::vector<std::shared_ptr<IMaterial>> MaterialList;
typedef std::vector<std::shared_ptr<const IMaterial>> MaterialConstList;
