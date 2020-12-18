#pragma once

#include "../ObjectsFactories/ObjectProxy.h"

class ZN_API MaterialProxie 
	: virtual public IMaterial
	, protected IMaterialDataOwner
	, public CObjectProxy
{
public:
	MaterialProxie(std::shared_ptr<IMaterial> _materal);
	virtual ~MaterialProxie() override;

	// IMaterial
	virtual void SetTexture(uint8 type, const std::shared_ptr<ITexture> texture) override;
	virtual std::shared_ptr<ITexture> GetTexture(uint8 ID) const override;

	virtual void SetSampler(uint8 ID, const std::shared_ptr<ISamplerState> samplerState) override;
    virtual std::shared_ptr<ISamplerState> GetSampler(uint8 ID) const override;

	virtual void Bind(const IShader* PixelShader) const override;
	virtual void Unbind(const IShader* PixelShader) const override;

	// IObjectLoadSave
	void Load(const std::shared_ptr<IByteBuffer>& Buffer) override;
	void Save(const std::shared_ptr<IByteBuffer>& Buffer) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

protected:
	// IMaterialDataOwner
	void InitializeMaterialData(size_t BufferSize) override;
	const void* GetMaterialData() const override;
	void* GetMaterialDataEx() override;
	void MarkMaterialDataDirty() override;

private:
	std::shared_ptr<IMaterial> m_Material;
};