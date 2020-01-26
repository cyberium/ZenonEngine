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
	

	// IRenderDevice
	IRenderWindow* CreateRenderWindow(INativeWindow * WindowObject, bool vSync) override;
	void DestroyRenderWindow(IRenderWindow* RenderWindow) override;

	IShader* CreateShader(EShaderType type, const std::string& fileName, const IShader::ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, IShaderInputLayout* _customLayout = nullptr);
	void DestroyShader(IShader* shader);

	IMesh* CreateMesh();
	void DestroyMesh(IMesh* mesh);

	ITexture* CreateTexture2D(const std::string& fileName);
	ITexture* CreateTextureCube(const std::string& fileName);

	ITexture* CreateTexture2D(uint16_t width, uint16_t height, uint16_t slices = 1, const ITexture::TextureFormat& format = ITexture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);
	ITexture* CreateTextureCube(uint16_t size, uint16_t numCubes = 1, const ITexture::TextureFormat& format = ITexture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);

	ITexture* GetDefaultTexture() const;

	ITexture* CreateTexture();
	void DestroyTexture(ITexture* texture);

	IMaterial* CreateMaterial(size_t Size);
	void DestroyMaterial(IMaterial* material);

	IRenderTarget* CreateRenderTarget();
	void DestroyRenderTarget(IRenderTarget* renderTarget);

	IQuery* CreateQuery(IQuery::QueryType queryType = IQuery::QueryType::Timer, uint8_t numBuffers = 3);
	void DestoryQuery(IQuery* query);

	ISamplerState* CreateSamplerState();
	void DestroySampler(ISamplerState* sampler);

	IPipelineState* CreatePipelineState();
	void DestoryPipelineState(IPipelineState* pipeline);

	IBuffer* CreateVoidVertexBuffer(const void* data, uint32 count, uint32 offset, uint32 stride) override;
	void DestroyVertexBuffer(IBuffer* buffer);

	IBuffer* CreateVoidIndexBuffer(const void* data, uint32 count, uint32 offset, uint32 stride) override;
	void DestroyIndexBuffer(IBuffer* buffer);

	IConstantBuffer* CreateConstantBuffer(const void* data, size_t size);
	void DestroyConstantBuffer(IConstantBuffer* buffer);

	IStructuredBuffer* CreateStructuredBuffer(void* data, uint32 count, uint32 stride, CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);
	void DestroyStructuredBuffer(IStructuredBuffer* buffer);

	// RenderDeviceDX11
	IRenderDevice*                                  GetRenderDevice() override final;
	ID3D11Device4*                                  GetDeviceD3D11() override final;
	ID3D11DeviceContext3*                           GetDeviceContextD3D11() override final;

protected:
	void                                            InitializeD3D11();
	void											DestroyBufferD3D11(IBuffer* buffer);

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