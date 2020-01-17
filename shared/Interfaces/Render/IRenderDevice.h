#pragma once

// FORWARD BEGIN
ZN_INTERFACE IRenderWindow;
ZN_INTERFACE IMesh;
ZN_INTERFACE IShader;
ZN_INTERFACE ITexture;
ZN_INTERFACE IMaterial;
ZN_INTERFACE IRenderTarget;
ZN_INTERFACE IQuery;
ZN_INTERFACE ISamplerState;
ZN_INTERFACE IPipelineState;
ZN_INTERFACE IBuffer;
// FORWARD END

ZN_INTERFACE ZN_API IRenderDevicePrimitiveCollection
{
	virtual ~IRenderDevicePrimitiveCollection() {}

	virtual std::shared_ptr<IMesh> CreateLine(cvec3 _dest) = 0; // Create a line
	virtual std::shared_ptr<IMesh> CreatePlane(cvec3 N = vec3(0, 1, 0)) = 0; // Create a plane in 3D.
	virtual std::shared_ptr<IMesh> CreateScreenQuad(float left = 0.0f, float right = 1.0f, float bottom = 0.0f, float top = 1.0f, float z = 0.0f) = 0; // Create a screen-space quad that can be used to render full-screen post-process effects to the screen.
	virtual std::shared_ptr<IMesh> CreateSphere() = 0; // Create a sphere in 3D
	virtual std::shared_ptr<IMesh> CreateCube() = 0; // Create a cube in 3D.
	virtual std::shared_ptr<IMesh> CreateCone() = 0; // Create a cone.
	virtual std::shared_ptr<IMesh> CreateQuad() = 0;

	// 3D meshes
	virtual std::shared_ptr<IMesh> Create3DQuad(float width = 10.0f, float height = 10.0f) = 0;
	virtual std::shared_ptr<IMesh> Create3DBeizerLine(vec3 start = vec3(0.0f, 0.0f, 0.0f), vec3 end = vec3(500.0f, 0.0f, 500.0f)) = 0;

	// UI meshes
	virtual std::shared_ptr<IMesh> CreateUIQuad(float width = 10.0f, float height = 10.0f) = 0;
	virtual std::shared_ptr<IMesh> CreateUIBeizerLine(vec2 start = vec2(0.0f, 0.0f), vec2 end = vec2(500.0f, 500.0f)) = 0;
};

ZN_INTERFACE ZN_API 
	__declspec(novtable, uuid("19615D02-1592-4420-8D0C-3DB4F7C6186A"))
	IRenderDevice : public IManager
{
	virtual ~IRenderDevice() {}

	virtual bool Initialize() = 0;
	virtual void Finalize() = 0;

	virtual const std::string& GetDeviceName() const = 0;
	virtual const RenderDeviceType GetDeviceType() const = 0;
	virtual IBaseManager* GetBaseManager() const = 0;
	virtual std::shared_ptr<IRenderWindow> CreateRenderWindow(IWindowObject * WindowObject, bool vSync) = 0;
	virtual std::shared_ptr<IRenderDevicePrimitiveCollection> GetPrimitiveCollection() const = 0;

	virtual void Lock() = 0;
	virtual void Unlock() = 0;

	virtual std::shared_ptr<IMesh> CreateMesh() = 0;
	virtual void DestroyMesh(std::shared_ptr<IMesh> mesh) = 0;

	virtual std::shared_ptr<IShader> CreateShader(EShaderType type, const std::string& fileName, const IShader::ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, std::shared_ptr<IShaderInputLayout> _customLayout = nullptr) = 0;
	virtual void DestroyShader(std::shared_ptr<IShader> shader) = 0;

	// Create a texture from a file.
	virtual std::shared_ptr<ITexture> CreateTexture2D(const std::string& fileName) = 0;
	virtual std::shared_ptr<ITexture> CreateTextureCube(const std::string& fileName) = 0;

	// Create an empty texture of a predefined size.
	virtual std::shared_ptr<ITexture> CreateTexture2D(uint16_t width, uint16_t height, uint16_t slices, const ITexture::TextureFormat& format = ITexture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false) = 0;
	virtual std::shared_ptr<ITexture> CreateTextureCube(uint16_t size, uint16_t numCubes = 1, const ITexture::TextureFormat& format = ITexture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false) = 0;
	virtual std::shared_ptr<ITexture> GetDefaultTexture() const = 0;

	// Create an null texture (can be loaded later using ITexture::LoadTexture2D function.
	virtual std::shared_ptr<ITexture> CreateTexture() = 0;
	virtual void DestroyTexture(std::shared_ptr<ITexture> texture) = 0;

	// Create an material
	virtual std::shared_ptr<IMaterial> CreateMaterial(size_t Size) = 0;
	virtual void DestroyMaterial(std::shared_ptr<IMaterial> material) = 0;

	// Create a render target
	virtual std::shared_ptr<IRenderTarget> CreateRenderTarget() = 0;
	virtual void DestroyRenderTarget(std::shared_ptr<IRenderTarget> renderTarget) = 0;

	// Create a GPU query object. Used for performance profiling, occlusion queries, or primitive output queries.
	virtual std::shared_ptr<IQuery> CreateQuery(IQuery::QueryType queryType = IQuery::QueryType::Timer, uint8_t numBuffers = 3) = 0;
	virtual void DestoryQuery(std::shared_ptr<IQuery> query) = 0;

	virtual std::shared_ptr<ISamplerState> CreateSamplerState() = 0;
	virtual void DestroySampler(std::shared_ptr<ISamplerState> sampler) = 0;

	virtual std::shared_ptr<IPipelineState> CreatePipelineState() = 0;
	virtual void DestoryPipelineState(std::shared_ptr<IPipelineState> pipeline) = 0;

	virtual std::shared_ptr<IBuffer> CreateVoidVertexBuffer(const void* data, uint32 count, uint32 offset, uint32 stride) = 0;
	virtual void DestroyVertexBuffer(std::shared_ptr<IBuffer> buffer) = 0;

	virtual std::shared_ptr<IBuffer> CreateVoidIndexBuffer(const void* data, uint32 count, uint32 offset, uint32 stride) = 0;
	virtual void DestroyIndexBuffer(std::shared_ptr<IBuffer> buffer) = 0;

	virtual std::shared_ptr<IConstantBuffer> CreateConstantBuffer(const void* data, size_t size) = 0;
	virtual void DestroyConstantBuffer(std::shared_ptr<IConstantBuffer> buffer) = 0;

	virtual std::shared_ptr<IStructuredBuffer> CreateStructuredBuffer(void* data, uint32 count, uint32 stride, CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false) = 0;
	virtual void DestroyStructuredBuffer(std::shared_ptr<IStructuredBuffer> buffer) = 0;

	// Templates

	// Create an vertex buffer (with std::vector)
	template<typename T>
	inline std::shared_ptr<IBuffer> CreateVertexBuffer(const std::vector<T>& data)
	{
		return CreateVoidVertexBuffer(data.data(), (uint32)data.size(), 0, sizeof(T));
	}
	template<typename T>
	inline std::shared_ptr<IBuffer> CreateVertexBuffer(const T* data, uint32 count)
	{
		return CreateVoidVertexBuffer(data, count, 0, sizeof(T));
	}

	// Create an index buffer.
	template<typename T>
	inline std::shared_ptr<IBuffer> CreateIndexBuffer(const std::vector<T>& data)
	{
		return CreateVoidIndexBuffer(data.data(), (uint32)data.size(), 0, sizeof(T));
	}
	template<typename T>
	inline std::shared_ptr<IBuffer> CreateIndexBuffer(const T* data, uint32 count)
	{
		return CreateVoidIndexBuffer(data, count, 0, sizeof(T));
	}

	// Create a constant buffer (or Uniform buffer)
	template<typename T>
	std::shared_ptr<IConstantBuffer> CreateConstantBuffer(const T& data);

	// Create a StructuredBuffer
	template<typename T>
	std::shared_ptr<IStructuredBuffer> CreateStructuredBuffer(const std::vector<T>& data, CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);
};


template< typename T >
std::shared_ptr<IConstantBuffer> IRenderDevice::CreateConstantBuffer(const T& data)
{
	return CreateConstantBuffer(&data, sizeof(T));
}

template<typename T>
std::shared_ptr<IStructuredBuffer> IRenderDevice::CreateStructuredBuffer(const std::vector<T>& data, CPUAccess cpuAccess, bool gpuWrite)
{
	size_t stride = sizeof(T);
	size_t numElements = data.size();
	return CreateStructuredBuffer((void*)data.data(), (uint32)numElements, (uint32)stride, cpuAccess, gpuWrite);
}