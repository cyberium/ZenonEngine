#pragma once

// FORWARD BEGIN
ZN_INTERFACE ITexture;
ZN_INTERFACE ISamplerState;
ZN_INTERFACE IPropertiesGroup;
// FORWARD END

ZN_INTERFACE ZN_API IMaterial 
	: public virtual IObject
	, public std::enable_shared_from_this<IMaterial>
{
	virtual ~IMaterial() {}

	virtual void SetTexture(uint8 ID, const std::shared_ptr<ITexture> texture) = 0;
	virtual std::shared_ptr<ITexture> GetTexture(uint8 ID) const = 0;

	virtual void SetSampler(uint8 ID, const std::shared_ptr<ISamplerState> samplerState) = 0;
	virtual std::shared_ptr<ISamplerState> GetSampler(uint8 ID) const = 0;

	virtual void Bind(const IShader* PixelShader) const = 0;
	virtual void Unbind(const IShader* PixelShader) const = 0;
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


ZN_INTERFACE ZN_API IMaterialUIControl
	: public virtual IMaterial
{
	virtual ~IMaterialUIControl() {}

	virtual void SetColor(glm::vec4 color) = 0;
	virtual glm::vec4 GetColor() const = 0;
	virtual void SetTexture(std::shared_ptr<ITexture> Texture) = 0;
	virtual std::shared_ptr<ITexture> GetTexture() const = 0;
};


ZN_INTERFACE ZN_API IMaterialsCreator
{
	virtual ~IMaterialsCreator() {}

	virtual size_t GetSupportedMaterialsTypesCount() const = 0;
	virtual std::string GetSupportedMaterialsClassName(size_t Index) const = 0;
	virtual std::shared_ptr<IMaterial> CreateMaterial(size_t Index) const = 0;
};


ZN_INTERFACE ZN_API IMaterialsFactory 
	: public IManager
{
	ZN_OBJECTCLASS(cMaterialsFactory)

	virtual ~IMaterialsFactory() {}

	virtual void AddMaterialsCreator(std::shared_ptr<IMaterialsCreator> Creator) = 0;
	virtual void RemoveMaterialsCreator(std::shared_ptr<IMaterialsCreator> Creator) = 0;

	virtual std::shared_ptr<IMaterial> CreateMaterial(std::string ClassName) const = 0;
	virtual std::shared_ptr<IMaterial> AddMaterial(std::shared_ptr<IMaterial> Material) = 0;

	virtual void PrintMaterials() const = 0;
};