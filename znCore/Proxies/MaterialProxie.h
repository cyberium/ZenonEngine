#pragma once

class ZN_API MaterialProxie 
	: public IMaterial
	, public ILoadableFromFile
	, protected IMaterialDataOwner
{
public:
	MaterialProxie(std::shared_ptr<IMaterial> _materal);
	virtual ~MaterialProxie() override;

	// IMaterial
	virtual void SetType(MaterialType Type) override;
	virtual MaterialType GetType() const override;
	virtual bool Is(MaterialType MaterialType) const override;
	virtual void SetName(const std::string& Name) override;
	virtual std::string GetName() const override;

	virtual void SetTexture(uint8 type, const std::shared_ptr<ITexture> texture) override;
	virtual const std::shared_ptr<ITexture>& GetTexture(uint8 ID) const override;
	
	virtual void SetSampler(uint8 ID, const std::shared_ptr<ISamplerState> samplerState) override;
    virtual const std::shared_ptr<ISamplerState>& GetSampler(uint8 ID) const override;
    
	virtual void Bind(const ShaderMap& shaders) const override;
	virtual void Unbind(const ShaderMap& shaders) const override;

	// ILoadableFromFile
	void Load(const std::shared_ptr<IByteBuffer>& ByteBuffer) override;
	void Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) override;

protected:
	// IMaterialDataOwner
	void InitializeMaterialData(size_t BufferSize) override;
	const void* GetMaterialData() const override;
	void* GetMaterialDataEx() override;
	void MarkMaterialDataDirty() override;

private:
	std::shared_ptr<IMaterial> m_Material;
};