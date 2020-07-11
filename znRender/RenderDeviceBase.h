#pragma once

class ZN_API CRenderDeviceBase 
	: public IRenderDevice
{
public:
	CRenderDeviceBase();
	virtual ~CRenderDeviceBase();

	// IRenderDevice
	IRenderPrimitivesFactory&                       GetPrimitivesFactory() const override;
	IRenderObjectsFactory&                          GetObjectsFactory() const override;
	std::shared_ptr<ITexture>                       GetDefaultTexture() const override;


protected:
	void                                            LoadDefaultResources();
	
protected:
	std::unique_ptr<IRenderPrimitivesFactory>       m_RenderPrimitivesFactory;
	std::unique_ptr<IRenderObjectsFactory>          m_RenderObjectsFactory;
	std::shared_ptr<ITexture>                       m_DefaultTexture;
};
