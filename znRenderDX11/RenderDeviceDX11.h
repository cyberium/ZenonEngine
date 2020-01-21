#pragma once

class ZN_API RenderDeviceDX11 
	: public RenderDeviceBase
	, public IRenderDeviceDX11
{
public:
	RenderDeviceDX11(IBaseManager* BaseManager);
	virtual ~RenderDeviceDX11();

	bool Initialize() override;
	void Finalize() override;

	std::string GetDeviceName() const;
    const RenderDeviceType GetDeviceType() const;
	IBaseManager* GetBaseManager() const;
	std::shared_ptr<IRenderWindow> CreateRenderWindow(IWindowObject * WindowObject, bool vSync);

	// IRenderDevice
	std::shared_ptr<IShader> CreateShader(EShaderType type, const std::string& fileName, const IShader::ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, std::shared_ptr<IShaderInputLayout> _customLayout = nullptr);
	void DestroyShader(std::shared_ptr<IShader> shader);

	std::shared_ptr<IMesh> CreateMesh();
	void DestroyMesh(std::shared_ptr<IMesh> mesh);

	std::shared_ptr<ITexture> CreateTexture2D(const std::string& fileName);
	std::shared_ptr<ITexture> CreateTextureCube(const std::string& fileName);

	std::shared_ptr<ITexture> CreateTexture2D(uint16_t width, uint16_t height, uint16_t slices = 1, const ITexture::TextureFormat& format = ITexture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);
	std::shared_ptr<ITexture> CreateTextureCube(uint16_t size, uint16_t numCubes = 1, const ITexture::TextureFormat& format = ITexture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);

	std::shared_ptr<ITexture> GetDefaultTexture() const;

	std::shared_ptr<ITexture> CreateTexture();
	void DestroyTexture(std::shared_ptr<ITexture> texture);

	std::shared_ptr<IMaterial> CreateMaterial(size_t Size);
	void DestroyMaterial(std::shared_ptr<IMaterial> material);

	std::shared_ptr<IRenderTarget> CreateRenderTarget();
	void DestroyRenderTarget(std::shared_ptr<IRenderTarget> renderTarget);

	std::shared_ptr<IQuery> CreateQuery(IQuery::QueryType queryType = IQuery::QueryType::Timer, uint8_t numBuffers = 3);
	void DestoryQuery(std::shared_ptr<IQuery> query);

	std::shared_ptr<ISamplerState> CreateSamplerState();
	void DestroySampler(std::shared_ptr<ISamplerState> sampler);

	std::shared_ptr<IPipelineState> CreatePipelineState();
	void DestoryPipelineState(std::shared_ptr<IPipelineState> pipeline);

	std::shared_ptr<IBuffer> CreateVoidVertexBuffer(const void* data, uint32 count, uint32 offset, uint32 stride) override;
	void DestroyVertexBuffer(std::shared_ptr<IBuffer> buffer);

	std::shared_ptr<IBuffer> CreateVoidIndexBuffer(const void* data, uint32 count, uint32 offset, uint32 stride) override;
	void DestroyIndexBuffer(std::shared_ptr<IBuffer> buffer);

	std::shared_ptr<IConstantBuffer> CreateConstantBuffer(const void* data, size_t size);
	void DestroyConstantBuffer(std::shared_ptr<IConstantBuffer> buffer);

	std::shared_ptr<IStructuredBuffer> CreateStructuredBuffer(void* data, uint32 count, uint32 stride, CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);
	void DestroyStructuredBuffer(std::shared_ptr<IStructuredBuffer> buffer);

	// RenderDeviceDX11
	std::shared_ptr<IRenderDevice> GetRenderDevice() override final;
	ID3D11Device4* GetDeviceD3D11() override final;
	ID3D11DeviceContext3* GetDeviceContextD3D11() override final;

protected:
	void                                            InitializeD3D11();
	void											DestroyBufferD3D11(std::shared_ptr<IBuffer> buffer);

private:
	void                                            LoadDefaultResources();

private:
	IBaseManager*                                   m_BaseManager;

	// DirectX
	ATL::CComPtr<ID3D11Device4>                     m_DeviceD3D11;
	ATL::CComPtr<ID3D11Debug>                       m_DebugD3D11;
	ATL::CComPtr<ID3D11DeviceContext3>              m_DeviceImmediateContext;
	ATL::CComPtr<ID3D11DeviceContext3>              m_DeviceDefferedContext;
	ATL::CComPtr<ID3D11Multithread>                 m_pMultiThread;
};