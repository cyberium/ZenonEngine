#pragma once

class RenderDeviceOGL : public RenderDeviceBase, public IRenderDeviceOGL, public std::enable_shared_from_this<IRenderDevice>
{
public:
	RenderDeviceOGL(IBaseManager* BaseManager);
	virtual ~RenderDeviceOGL();

    void InitDevice(HDC Hdc);

	bool Initialize() override;
	void Finalize() override;

	const std::string& GetDeviceName() const;
    const RenderDeviceType GetDeviceType() const;
	IBaseManager* GetBaseManager() const;
	std::shared_ptr<IRenderWindow> CreateRenderWindow(IWindowObject * WindowObject, bool vSync);

	// Inherited from IRenderDevice
	virtual std::shared_ptr<IBuffer> CreateVoidVertexBuffer(const void* data, uint32 count, uint32 offset, uint32 stride) override;
	virtual std::shared_ptr<IBuffer> CreateVoidIndexBuffer(const void* data, uint32 count, uint32 offset, uint32 stride) override;

	virtual std::shared_ptr<IConstantBuffer> CreateConstantBuffer(const void* data, size_t size);
	virtual std::shared_ptr<IStructuredBuffer> CreateStructuredBuffer(void* data, uint32 count, uint32 stride, CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);

	virtual void DestroyBuffer(std::shared_ptr<IBuffer> buffer);
	virtual void DestroyVertexBuffer(std::shared_ptr<IBuffer> buffer);
	virtual void DestroyIndexBuffer(std::shared_ptr<IBuffer> buffer);
	virtual void DestroyConstantBuffer(std::shared_ptr<IConstantBuffer> buffer);
	virtual void DestroyStructuredBuffer(std::shared_ptr<IStructuredBuffer> buffer);

	virtual void Lock();
	virtual void Unlock();

	virtual std::shared_ptr<IShader> CreateShader(IShader::ShaderType type, const std::string& fileName, const IShader::ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, std::shared_ptr<IShaderInputLayout> _customLayout);
	virtual void DestroyShader(std::shared_ptr<IShader> shader);
	
	virtual std::shared_ptr<IMesh> CreateMesh();
	virtual void DestroyMesh(std::shared_ptr<IMesh> mesh);

	virtual std::shared_ptr<ITexture> CreateTexture2D(const std::string& fileName);
	virtual std::shared_ptr<ITexture> CreateTextureCube(const std::string& fileName);

	virtual std::shared_ptr<ITexture> CreateTexture2D(uint16_t width, uint16_t height, uint16_t slices = 1, const ITexture::TextureFormat& format = ITexture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);
	virtual std::shared_ptr<ITexture> CreateTextureCube(uint16_t size, uint16_t numCubes = 1, const ITexture::TextureFormat& format = ITexture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);
	virtual std::shared_ptr<ITexture> CreateTexture();
	virtual std::shared_ptr<ITexture> GetDefaultTexture() const;

	virtual void DestroyTexture(std::shared_ptr<ITexture> texture);

	virtual std::shared_ptr<IMaterial> CreateMaterial(size_t Size);
	virtual void DestroyMaterial(std::shared_ptr<IMaterial> material);

	virtual std::shared_ptr<IQuery> CreateQuery(IQuery::QueryType queryType = IQuery::QueryType::Timer, uint8_t numBuffers = 3);
	virtual void DestoryQuery(std::shared_ptr<IQuery> query);

	virtual std::shared_ptr<IRenderTarget> CreateRenderTarget();
	virtual void DestroyRenderTarget(std::shared_ptr<IRenderTarget> renderTarget);

	virtual std::shared_ptr<ISamplerState> CreateSamplerState();
	virtual void DestroySampler(std::shared_ptr<ISamplerState> sampler);

	virtual std::shared_ptr<IPipelineState> CreatePipelineState();
	virtual void DestoryPipelineState(std::shared_ptr<IPipelineState> pipeline);

public:
	bool                        IsDoubleBuffered();
	uint32                      GetDefaultRB();
	void                        SetDefaultRB(uint32 _obj);

protected:
	void                        CreateDevice() override final;

private:
	void                        LoadDefaultResources();

private:
	IBaseManager*                   m_BaseManager;

	uint32						m_DepthFormat;
	bool						_doubleBuffered;
	uint16						_maxComputeBufferAttachments;
	bool                        m_IsDefaultFBOMultisampled;
	int							m_RBDefault;
};