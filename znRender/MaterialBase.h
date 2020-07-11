#pragma once

class ZN_API MaterialBase 
	: public IMaterial
	, public IMaterialDataOwner
	, public Object
{
public:
	MaterialBase(IRenderDevice& renderDevice);
	virtual ~MaterialBase();

	// IMaterial
	virtual void  SetTexture(uint8 ID, const std::shared_ptr<ITexture> texture) override;
	virtual const std::shared_ptr<ITexture>& GetTexture(uint8 ID) const override;
	
	virtual void  SetSampler(uint8 ID, const std::shared_ptr<ISamplerState> samplerState) override;
    virtual const std::shared_ptr<ISamplerState>& GetSampler(uint8 ID) const override;

	virtual void Bind(const ShaderMap& shaders) const override;
	virtual void Unbind(const ShaderMap& shaders) const override;

	// IMaterialDataOwner
	void         InitializeMaterialData(size_t BufferSize) override;
	const void*  GetMaterialData() const override;
	void*        GetMaterialDataEx() override;
	void         MarkMaterialDataDirty() override;

	// IObject
	Guid GetGUID() const override final { return Object::GetGUID(); };
	std::string GetName() const override final { return Object::GetName(); };
	void SetName(const std::string& Name) override final { Object::SetName(Name); };
	std::string GetTypeName() const override final { return Object::GetTypeName(); };
	std::string GetClassNameW() const override final { return Object::GetClassNameW(); };

	// IObjectLoadSave
	void Load(const std::shared_ptr<IByteBuffer>& ByteBuffer) override;
	void Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

private:
	void         FinalizeMaterialData();

protected:
	size_t                           m_BufferSize;
	TextureMap                       m_Textures;
	ShaderMap                        m_Shaders;
    SamplersMap                      m_Samplers;
	void*                            m_MaterialData;
	std::shared_ptr<IConstantBuffer> m_ConstantBuffer;
	mutable bool                     m_Dirty;

private:
	IRenderDevice& m_RenderDevice;
};