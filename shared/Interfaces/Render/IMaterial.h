#pragma once

// FORWARD BEGIN
ZN_INTERFACE IShader;
ZN_INTERFACE ITexture;
ZN_INTERFACE ISamplerState;
// FORWARD END

typedef ZN_API int64 MaterialType;

ZN_INTERFACE ZN_API IMaterial 
{
	virtual ~IMaterial() {}

	virtual void SetType(MaterialType Type) = 0;
	virtual MaterialType GetType() const = 0;
	virtual bool Is(MaterialType MaterialType) const = 0;
	virtual void SetName(const std::string& Name) = 0;
	virtual std::string GetName() const = 0;

	virtual void SetTexture(uint8 ID, const std::shared_ptr<ITexture> texture) = 0;
	virtual const std::shared_ptr<ITexture>& GetTexture(uint8 ID) const = 0;
	
	virtual void SetSampler(uint8 ID, const std::shared_ptr<ISamplerState> samplerState) = 0;
	virtual const std::shared_ptr<ISamplerState>& GetSampler(uint8 ID) const = 0;
	
	virtual void Bind(const ShaderMap& shaders) const = 0;
	virtual void Unbind(const ShaderMap& shaders) const = 0;
};

typedef std::map<std::string, std::shared_ptr<IMaterial>> MaterialNameMap;
typedef std::vector<std::shared_ptr<IMaterial>> MaterialList;
typedef std::vector< std::shared_ptr<const IMaterial>> MaterialConstList;


ZN_INTERFACE ZN_API IMaterialDataOwner
{
	virtual ~IMaterialDataOwner() {}

	virtual void InitializeMaterialData(size_t BufferSize) = 0;
	virtual const void* GetMaterialData() const = 0;
	virtual void* GetMaterialDataEx() = 0;
	virtual void MarkMaterialDataDirty() = 0;
};


ZN_INTERFACE ZN_API IMaterialsCreator
{
	virtual ~IMaterialsCreator() {}

	virtual size_t GetSupportedMaterialsTypesCount() const = 0;
	virtual MaterialType GetSupportedMaterialsTypeName(size_t Index) const = 0;
	virtual std::shared_ptr<IMaterial> CreateMaterial(size_t Index) const = 0;
};


ZN_INTERFACE ZN_API __declspec(uuid("BA11EAC5-26D2-4F3C-A171-7A216257424C")) IMaterialsFactory
	: public IManager
{
	virtual ~IMaterialsFactory() {}

	virtual void AddMaterialsCreator(std::shared_ptr<IMaterialsCreator> Creator) = 0;
	virtual void RemoveMaterialsCreator(std::shared_ptr<IMaterialsCreator> Creator) = 0;

	virtual std::shared_ptr<IMaterial> CreateMaterial(MaterialType MaterialType) const = 0;
};