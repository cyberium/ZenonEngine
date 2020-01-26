#pragma once

class RenderDeviceOGL 
	: public RenderDeviceBase
	, public IRenderDeviceOGL
{
public:
	RenderDeviceOGL(IBaseManager* BaseManager);
	virtual ~RenderDeviceOGL();

    void InitDevice(HDC Hdc);

	bool Initialize() override;
	void Finalize() override;

	std::string GetDeviceName() const;
    const RenderDeviceType GetDeviceType() const;
	IBaseManager* GetBaseManager() const;
	IRenderWindow* CreateRenderWindow(INativeWindow * WindowObject, bool vSync);

	// Inherited from IRenderDevice
	virtual IBuffer* CreateVoidVertexBuffer(const void* data, uint32 count, uint32 offset, uint32 stride) override;
	virtual IBuffer* CreateVoidIndexBuffer(const void* data, uint32 count, uint32 offset, uint32 stride) override;

	virtual IConstantBuffer* CreateConstantBuffer(const void* data, size_t size);
	virtual IStructuredBuffer* CreateStructuredBuffer(void* data, uint32 count, uint32 stride, CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);

	virtual void DestroyBuffer(IBuffer* buffer);
	virtual void DestroyVertexBuffer(IBuffer* buffer);
	virtual void DestroyIndexBuffer(IBuffer* buffer);
	virtual void DestroyConstantBuffer(IConstantBuffer* buffer);
	virtual void DestroyStructuredBuffer(IStructuredBuffer* buffer);

	virtual void Lock();
	virtual void Unlock();

	virtual IShader* CreateShader(EShaderType type, const std::string& fileName, const IShader::ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, IShaderInputLayout* _customLayout);
	virtual void DestroyShader(IShader* shader);
	
	virtual IMesh* CreateMesh();
	virtual void DestroyMesh(IMesh* mesh);

	virtual ITexture* CreateTexture2D(const std::string& fileName);
	virtual ITexture* CreateTextureCube(const std::string& fileName);

	virtual ITexture* CreateTexture2D(uint16_t width, uint16_t height, uint16_t slices = 1, const ITexture::TextureFormat& format = ITexture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);
	virtual ITexture* CreateTextureCube(uint16_t size, uint16_t numCubes = 1, const ITexture::TextureFormat& format = ITexture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);
	virtual ITexture* CreateTexture();
	virtual ITexture* GetDefaultTexture() const;

	virtual void DestroyTexture(ITexture* texture);

	virtual IMaterial* CreateMaterial(size_t Size);
	virtual void DestroyMaterial(IMaterial* material);

	virtual IQuery* CreateQuery(IQuery::QueryType queryType = IQuery::QueryType::Timer, uint8_t numBuffers = 3);
	virtual void DestoryQuery(IQuery* query);

	virtual IRenderTarget* CreateRenderTarget();
	virtual void DestroyRenderTarget(IRenderTarget* renderTarget);

	virtual ISamplerState* CreateSamplerState();
	virtual void DestroySampler(ISamplerState* sampler);

	virtual IPipelineState* CreatePipelineState();
	virtual void DestoryPipelineState(IPipelineState* pipeline);

public:
	bool                        IsDoubleBuffered();
	uint32                      GetDefaultRB();
	void                        SetDefaultRB(uint32 _obj);

protected:
	void                        CreateDevice();

private:
	void                        LoadDefaultResources();

private:
	IBaseManager*               m_BaseManager;

	uint32						m_DepthFormat;
	bool						_doubleBuffered;
	uint16						_maxComputeBufferAttachments;
	bool                        m_IsDefaultFBOMultisampled;
	int							m_RBDefault;
};