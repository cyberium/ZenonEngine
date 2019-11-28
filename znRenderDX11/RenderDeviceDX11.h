#pragma once

class IMaterial;

class OW_ENGINE_API RenderDeviceDX11 : public IRenderDevice, public IRenderDeviceDX11, public std::enable_shared_from_this<RenderDeviceDX11>
{
	typedef IRenderDevice base;
public:
	RenderDeviceDX11(std::shared_ptr<IBaseManager> BaseManager);
	virtual ~RenderDeviceDX11();

	bool Initialize() override;
	void Finalize() override;

	const std::string& GetDeviceName() const;
    const DeviceType GetDeviceType() const;
	const std::shared_ptr<IBaseManager>& GetBaseManager() const;

	// Inherited from IRenderDevice
	std::shared_ptr<IBuffer> CreateVoidVertexBuffer(const void* data, uint32 count, uint32 offset, uint32 stride);

	std::shared_ptr<IBuffer> CreateUInt16IndexBuffer(const uint16* data, uint32 count);
	std::shared_ptr<IBuffer> CreateUInt32IndexBuffer(const uint32* data, uint32 count);

	std::shared_ptr<ConstantBuffer> CreateConstantBuffer(const void* data, size_t size);
	std::shared_ptr<StructuredBuffer> CreateStructuredBuffer(void* data, uint32 count, uint32 stride, CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);

	void DestroyBuffer(std::shared_ptr<IBuffer> buffer);
	void DestroyVertexBuffer(std::shared_ptr<IBuffer> buffer);
	void DestroyIndexBuffer(std::shared_ptr<IBuffer> buffer);
	void DestroyConstantBuffer(std::shared_ptr<ConstantBuffer> buffer);
	void DestroyStructuredBuffer(std::shared_ptr<StructuredBuffer> buffer);

	void Lock();
	void Unlock();

	std::shared_ptr<Shader> CreateShader(Shader::ShaderType type, const std::string& fileName, const Shader::ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, std::shared_ptr<IShaderInputLayout> _customLayout = nullptr);
	void DestroyShader(std::shared_ptr<Shader> shader);
	
	std::shared_ptr<IMesh> CreateMesh();
	void DestroyMesh(std::shared_ptr<IMesh> mesh);

	std::shared_ptr<Texture> CreateTexture2D(const std::string& fileName);
	std::shared_ptr<Texture> CreateTextureCube(const std::string& fileName);

	std::shared_ptr<Texture> CreateTexture2D(uint16_t width, uint16_t height, uint16_t slices = 1, const Texture::TextureFormat& format = Texture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);
	std::shared_ptr<Texture> CreateTextureCube(uint16_t size, uint16_t numCubes = 1, const Texture::TextureFormat& format = Texture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);
	std::shared_ptr<Texture> CreateTexture();
	std::shared_ptr<Texture> GetDefaultTexture() const;

	void DestroyTexture(std::shared_ptr<Texture> texture);

	std::shared_ptr<IMaterial> CreateMaterial(size_t Size);
	void DestroyMaterial(std::shared_ptr<IMaterial> material);

	std::shared_ptr<Query> CreateQuery(Query::QueryType queryType = Query::QueryType::Timer, uint8_t numBuffers = 3);
	void DestoryQuery(std::shared_ptr<Query> query);

	std::shared_ptr<IRenderTarget> CreateRenderTarget();
	void DestroyRenderTarget(std::shared_ptr<IRenderTarget> renderTarget);

	std::shared_ptr<SamplerState> CreateSamplerState();
	void DestroySampler(std::shared_ptr<SamplerState> sampler);

	std::shared_ptr<PipelineState> CreatePipelineState();
	void DestoryPipelineState(std::shared_ptr<PipelineState> pipeline);

	// RenderDeviceDX11
	ATL::CComPtr<ID3D11Device2>                     GetDevice() const override;
	ATL::CComPtr<ID3D11DeviceContext2>              GetDeviceContext() const override;

protected:
	void                                            CreateDevice();

private:
	void                                            LoadDefaultResources();

private:
	std::shared_ptr<IBaseManager>                   m_BaseManager;

	// DirectX
	ATL::CComPtr<ID3D11Device2>                     m_pDevice;
	ATL::CComPtr<ID3D11Debug>                       m_pDebugLayer;
	ATL::CComPtr<ID3D11DeviceContext2>              m_pDeviceContext;
	ATL::CComPtr<ID3D10Multithread>                 m_pMultiThread;
};