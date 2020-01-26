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

	virtual IMesh* CreateLine(cvec3 _dest) = 0; // Create a line
	virtual IMesh* CreatePlane(cvec3 N = vec3(0, 1, 0)) = 0; // Create a plane in 3D.
	virtual IMesh* CreateScreenQuad(float left = 0.0f, float right = 1.0f, float bottom = 0.0f, float top = 1.0f, float z = 0.0f) = 0; // Create a screen-space quad that can be used to render full-screen post-process effects to the screen.
	virtual IMesh* CreateSphere() = 0; // Create a sphere in 3D
	virtual IMesh* CreateCube() = 0; // Create a cube in 3D.
	virtual IMesh* CreateCone() = 0; // Create a cone.
	virtual IMesh* CreateQuad() = 0;

	// 3D meshes
	virtual IMesh* Create3DQuad(float width = 10.0f, float height = 10.0f) = 0;
	virtual IMesh* Create3DBeizerLine(vec3 start = vec3(0.0f, 0.0f, 0.0f), vec3 end = vec3(500.0f, 0.0f, 500.0f)) = 0;

	// UI meshes
	virtual IMesh* CreateUIQuad(float width = 10.0f, float height = 10.0f) = 0;
	virtual IMesh* CreateUIBeizerLine(vec2 start = vec2(0.0f, 0.0f), vec2 end = vec2(500.0f, 500.0f)) = 0;
};

ZN_INTERFACE ZN_API 
	__declspec(novtable, uuid("19615D02-1592-4420-8D0C-3DB4F7C6186A"))
	IRenderDevice : public IManager
{
	virtual ~IRenderDevice() {}

	virtual bool Initialize() = 0;
	virtual void Finalize() = 0;

	virtual std::string GetDeviceName() const = 0;
	virtual const RenderDeviceType GetDeviceType() const = 0;
	virtual IBaseManager* GetBaseManager() const = 0;
	
	virtual IRenderDevicePrimitiveCollection* GetPrimitiveCollection() const = 0;

	virtual IRenderWindow* CreateRenderWindow(INativeWindow * WindowObject, bool vSync) = 0;
	virtual void DestroyRenderWindow(IRenderWindow* RenderWindow) = 0;

	virtual IMesh* CreateMesh() = 0;
	virtual void DestroyMesh(IMesh* mesh) = 0;

	virtual IShader* CreateShader(EShaderType type, const std::string& fileName, const IShader::ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, IShaderInputLayout* _customLayout = nullptr) = 0;
	virtual void DestroyShader(IShader* shader) = 0;

	// Create a texture from a file.
	virtual ITexture* CreateTexture2D(const std::string& fileName) = 0;
	virtual ITexture* CreateTextureCube(const std::string& fileName) = 0;

	// Create an empty texture of a predefined size.
	virtual ITexture* CreateTexture2D(uint16_t width, uint16_t height, uint16_t slices, const ITexture::TextureFormat& format = ITexture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false) = 0;
	virtual ITexture* CreateTextureCube(uint16_t size, uint16_t numCubes = 1, const ITexture::TextureFormat& format = ITexture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false) = 0;
	virtual ITexture* GetDefaultTexture() const = 0;

	// Create an null texture (can be loaded later using ITexture::LoadTexture2D function.
	virtual ITexture* CreateTexture() = 0;
	virtual void DestroyTexture(ITexture* texture) = 0;

	// Create an material
	virtual IMaterial* CreateMaterial(size_t Size) = 0;
	virtual void DestroyMaterial(IMaterial* material) = 0;

	// Create a render target
	virtual IRenderTarget* CreateRenderTarget() = 0;
	virtual void DestroyRenderTarget(IRenderTarget* renderTarget) = 0;

	// Create a GPU query object. Used for performance profiling, occlusion queries, or primitive output queries.
	virtual IQuery* CreateQuery(IQuery::QueryType queryType = IQuery::QueryType::Timer, uint8_t numBuffers = 3) = 0;
	virtual void DestoryQuery(IQuery* query) = 0;

	virtual ISamplerState* CreateSamplerState() = 0;
	virtual void DestroySampler(ISamplerState* sampler) = 0;

	virtual IPipelineState* CreatePipelineState() = 0;
	virtual void DestoryPipelineState(IPipelineState* pipeline) = 0;

	virtual IBuffer* CreateVoidVertexBuffer(const void* data, uint32 count, uint32 offset, uint32 stride) = 0;
	virtual void DestroyVertexBuffer(IBuffer* buffer) = 0;

	virtual IBuffer* CreateVoidIndexBuffer(const void* data, uint32 count, uint32 offset, uint32 stride) = 0;
	virtual void DestroyIndexBuffer(IBuffer* buffer) = 0;

	virtual IConstantBuffer* CreateConstantBuffer(const void* data, size_t size) = 0;
	virtual void DestroyConstantBuffer(IConstantBuffer* buffer) = 0;

	virtual IStructuredBuffer* CreateStructuredBuffer(void* data, uint32 count, uint32 stride, CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false) = 0;
	virtual void DestroyStructuredBuffer(IStructuredBuffer* buffer) = 0;

	// Templates

	// Create an vertex buffer (with std::vector)
	template<typename T>
	inline IBuffer* CreateVertexBuffer(const std::vector<T>& data)
	{
		return CreateVoidVertexBuffer(data.data(), (uint32)data.size(), 0, sizeof(T));
	}
	template<typename T>
	inline IBuffer* CreateVertexBuffer(const T* data, uint32 count)
	{
		return CreateVoidVertexBuffer(data, count, 0, sizeof(T));
	}

	// Create an index buffer.
	template<typename T>
	inline IBuffer* CreateIndexBuffer(const std::vector<T>& data)
	{
		return CreateVoidIndexBuffer(data.data(), (uint32)data.size(), 0, sizeof(T));
	}
	template<typename T>
	inline IBuffer* CreateIndexBuffer(const T* data, uint32 count)
	{
		return CreateVoidIndexBuffer(data, count, 0, sizeof(T));
	}

	// Create a constant buffer (or Uniform buffer)
	template<typename T>
	inline IConstantBuffer* CreateConstantBuffer(const T& data)
	{
		return CreateConstantBuffer(&data, sizeof(T));
	}

	// Create a StructuredBuffer
	template<typename T>
	inline IStructuredBuffer* CreateStructuredBuffer(const std::vector<T>& data, CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false)
	{
		size_t stride = sizeof(T);
		size_t numElements = data.size();
		return CreateStructuredBuffer((void*)data.data(), (uint32)numElements, (uint32)stride, cpuAccess, gpuWrite);
	}
};
