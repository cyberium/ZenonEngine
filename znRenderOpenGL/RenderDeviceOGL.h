#pragma once

class RenderDeviceOGL : public IRenderDevice
{
public:
	typedef IRenderDevice base;

	RenderDeviceOGL();
	virtual ~RenderDeviceOGL();

	const std::string& GetDeviceName() const;
    const DeviceType GetDeviceType() const;

	// Inherited from IRenderDevice
	virtual std::shared_ptr<IBuffer> CreateVoidVertexBuffer(const void* data, uint32 count, uint32 offset, uint32 stride);

	virtual std::shared_ptr<IBuffer> CreateUInt16IndexBuffer(const uint16* data, uint32 count);
	virtual std::shared_ptr<IBuffer> CreateUInt32IndexBuffer(const uint32* data, uint32 count);

	virtual std::shared_ptr<ConstantBuffer> CreateConstantBuffer(const void* data, size_t size);
	virtual std::shared_ptr<StructuredBuffer> CreateStructuredBuffer(void* data, uint32 count, uint32 stride, CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);

	virtual void DestroyBuffer(std::shared_ptr<IBuffer> buffer);
	virtual void DestroyVertexBuffer(std::shared_ptr<IBuffer> buffer);
	virtual void DestroyIndexBuffer(std::shared_ptr<IBuffer> buffer);
	virtual void DestroyConstantBuffer(std::shared_ptr<ConstantBuffer> buffer);
	virtual void DestroyStructuredBuffer(std::shared_ptr<StructuredBuffer> buffer);

	virtual void Lock();
	virtual void Unlock();

	virtual std::shared_ptr<Shader> CreateShader(Shader::ShaderType type, const std::string& fileName, const Shader::ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, std::shared_ptr<IShaderInputLayout> _customLayout);
	virtual void DestroyShader(std::shared_ptr<Shader> shader);
	
	virtual std::shared_ptr<IMesh> CreateMesh();
	virtual void DestroyMesh(std::shared_ptr<IMesh> mesh);

	virtual std::shared_ptr<Texture> CreateTexture2D(const std::string& fileName);
	virtual std::shared_ptr<Texture> CreateTextureCube(const std::string& fileName);

	virtual std::shared_ptr<Texture> CreateTexture2D(uint16_t width, uint16_t height, uint16_t slices = 1, const Texture::TextureFormat& format = Texture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);
	virtual std::shared_ptr<Texture> CreateTextureCube(uint16_t size, uint16_t numCubes = 1, const Texture::TextureFormat& format = Texture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);
	virtual std::shared_ptr<Texture> CreateTexture();
	virtual std::shared_ptr<Texture> GetDefaultTexture() const;

	virtual void DestroyTexture(std::shared_ptr<Texture> texture);

	virtual std::shared_ptr<Material> CreateMaterial();
	virtual void DestroyMaterial(std::shared_ptr<Material> material);

	virtual std::shared_ptr<Query> CreateQuery(Query::QueryType queryType = Query::QueryType::Timer, uint8_t numBuffers = 3);
	virtual void DestoryQuery(std::shared_ptr<Query> query);

	virtual std::shared_ptr<IRenderTarget> CreateRenderTarget();
	virtual void DestroyRenderTarget(std::shared_ptr<IRenderTarget> renderTarget);

	virtual std::shared_ptr<SamplerState> CreateSamplerState();
	virtual void DestroySampler(std::shared_ptr<SamplerState> sampler);

	virtual std::shared_ptr<PipelineState> CreatePipelineState();
	virtual void DestoryPipelineState(std::shared_ptr<PipelineState> pipeline);

public:
	bool IsDoubleBuffered();
	uint32 GetDefaultRB();
	void SetDefaultRB(uint32 _obj);

public:
	void CreateDevice(HDC _hdc);
	void LoadDefaultResources();

private:
	uint32						m_DepthFormat;
	bool						_doubleBuffered;
	uint16						_maxComputeBufferAttachments;
	bool                        m_IsDefaultFBOMultisampled;
	int							m_RBDefault;
};