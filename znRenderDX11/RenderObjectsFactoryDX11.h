#pragma once

class ZN_API CRenderObjectsFactoryDX11
	: public CRenderObjectsFactory
{
public:
	CRenderObjectsFactoryDX11(IRenderDeviceDX11& RenderDeviceDX11);
	virtual ~CRenderObjectsFactoryDX11();

	// IRenderObjectsFactory
	std::shared_ptr<IRenderWindow>      CreateRenderWindow(std::unique_ptr<IznNativeWindow> WindowObject, bool vSync) override;
	std::shared_ptr<IGeometry>          CreateGeometry() override;
	std::shared_ptr<IModel>             CreateModel() override;

	std::shared_ptr<ITexture>           CreateEmptyTexture();
	std::shared_ptr<ITexture>           CreateTexture2D(size_t width, size_t height, size_t slices = 1, const ITexture::TextureFormat& format = ITexture::TextureFormat(), EAccess cpuAccess = EAccess::None) override;
	std::shared_ptr<ITexture>           CreateTextureCube(size_t size, const ITexture::TextureFormat& format = ITexture::TextureFormat(), EAccess cpuAccess = EAccess::None) override;

	std::shared_ptr<IMaterial>          CreateMaterial(const std::string& MaterialName) override;
	std::shared_ptr<IRenderTarget>      CreateRenderTarget() override;
	std::shared_ptr<IQuery>             CreateQuery(IQuery::QueryType queryType, size_t numBuffers) override;
	std::shared_ptr<ISamplerState>      CreateSamplerState() override;
	std::shared_ptr<IBlendState>        CreateBlendState() override;
	std::shared_ptr<IDepthStencilState> CreateDepthStencilState() override;
	std::shared_ptr<IRasterizerState>   CreateRasterizerState() override;
	std::shared_ptr<IPipelineState>     CreatePipelineState() override;
	std::shared_ptr<IBuffer>            CreateVoidVertexBuffer(const void* data, size_t count, size_t offset, size_t stride) override;
	std::shared_ptr<IBuffer>            CreateVoidIndexBuffer(const void* data, size_t count, size_t offset, size_t stride) override;
	std::shared_ptr<IConstantBuffer>    CreateConstantBuffer(const void* data, size_t size) override;
	std::shared_ptr<IStructuredBuffer>  CreateStructuredBuffer(const void* data, size_t count, size_t stride, EAccess cpuAccess = EAccess::None) override;

	std::shared_ptr<IShader>            LoadShader(EShaderType type, const std::string& fileName, const std::string& entryPoint, const IShader::ShaderMacros& shaderMacros, IShaderInputLayout* _customLayout = nullptr) override;

	std::shared_ptr<IBuffer>            LoadVoidBuffer(const std::shared_ptr<IByteBuffer>& ByteBuffer) override;
	std::shared_ptr<IConstantBuffer>    LoadConstantBuffer(const std::shared_ptr<IByteBuffer>& ByteBuffer) override;
	std::shared_ptr<IStructuredBuffer>  LoadStructuredBuffer(const std::shared_ptr<IByteBuffer>& ByteBuffer) override;

	std::shared_ptr<IBuffer>            LoadVoidBuffer(const std::shared_ptr<IXMLReader>& Reader) override;
	std::shared_ptr<IConstantBuffer>    LoadConstantBuffer(const std::shared_ptr<IXMLReader>& Reader) override;
	std::shared_ptr<IStructuredBuffer>  LoadStructuredBuffer(const std::shared_ptr<IXMLReader>& Reader) override;

private:
	IRenderDeviceDX11& m_RenderDeviceDX11;
	std::recursive_mutex m_LockMutex;
};