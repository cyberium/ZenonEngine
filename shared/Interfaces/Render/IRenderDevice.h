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
ZN_INTERFACE IBlendState;
ZN_INTERFACE IDepthStencilState;
ZN_INTERFACE IRasterizerState;
ZN_INTERFACE IPipelineState;
ZN_INTERFACE IBuffer;
// FORWARD END

ZN_INTERFACE ZN_API IRenderPrimitivesFactory
{
	virtual ~IRenderPrimitivesFactory() {}

	virtual std::shared_ptr<IMesh> CreateLine(cvec3 _dest) = 0; // Create a line
	virtual std::shared_ptr<IMesh> CreatePlane(cvec3 N = vec3(0, 1, 0)) = 0; // Create a plane in 3D.
	virtual std::shared_ptr<IMesh> CreateScreenQuad(float left = 0.0f, float right = 1.0f, float bottom = 0.0f, float top = 1.0f, float z = 0.0f) = 0; // Create a screen-space quad that can be used to render full-screen post-process effects to the screen.
	virtual std::shared_ptr<IMesh> CreateSphere() = 0;
	virtual std::shared_ptr<IMesh> CreateCube() = 0;
	virtual std::shared_ptr<IMesh> CreateCone() = 0;
	virtual std::shared_ptr<IMesh> CreateQuad() = 0;

	// 3D meshes
	virtual std::shared_ptr<IMesh> Create3DQuad(float width = 10.0f, float height = 10.0f) = 0;
	virtual std::shared_ptr<IMesh> Create3DBeizerLine(vec3 start = vec3(0.0f, 0.0f, 0.0f), vec3 end = vec3(500.0f, 0.0f, 500.0f)) = 0;

	// UI meshes
	virtual std::shared_ptr<IMesh> CreateUIQuad(float width = 10.0f, float height = 10.0f) = 0;
	virtual std::shared_ptr<IMesh> CreateUIBeizerLine(vec2 start = vec2(0.0f, 0.0f), vec2 end = vec2(500.0f, 500.0f)) = 0;
};


typedef long long ZN_API RenderObjectID;


/**
	- Buffers, textures, shaders and other object created by RenderDevice must implement this interface
*/
ZN_INTERFACE ZN_API IRenderObject
{
	virtual ~IRenderObject() {}

	virtual void            Hold() const = 0;
	virtual void            Unhold() const = 0;
	virtual RenderObjectID  GetID() const = 0;
};


/**
	- Provide functionality for creating any render objects.
	- Some created render objects stored as std::weak_ptr<...> for future fast access (if they not be deleted before)
	- You can hold render object for retrieving this object by it RenderObjectID later
*/
ZN_INTERFACE ZN_API IRenderObjectsFactory
{
	virtual ~IRenderObjectsFactory() {}

	virtual bool                              HoldRenderObject(RenderObjectID ID) const = 0;
	virtual bool                              UnholdRenderObject(RenderObjectID ID) const = 0;
	virtual std::shared_ptr<IRenderObject>    GetRenderObject(RenderObjectID ID) const = 0;
	//virtual void                              DestroyRenderObject(RenderObjectID ID) = 0; // TODO: Investigate usability of this method


	virtual std::shared_ptr<IRenderWindow>    CreateRenderWindow(INativeWindow& NativeWindow, bool vSync) = 0;
	virtual std::shared_ptr<IMesh>            CreateMesh() = 0;
	virtual std::shared_ptr<IShader>          CreateShader(EShaderType type, const std::string& fileName, const std::string& entryPoint, const IShader::ShaderMacros& shaderMacros, const std::string& profile, IShaderInputLayout* _customLayout = nullptr) = 0;

	virtual std::shared_ptr<ITexture>         CreateEmptyTexture() = 0;
	virtual std::shared_ptr<ITexture>         CreateTexture2D(size_t width, size_t height, size_t slices, const ITexture::TextureFormat& format = ITexture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false) = 0;
	virtual std::shared_ptr<ITexture>         CreateTextureCube(size_t size, size_t numCubes = 1, const ITexture::TextureFormat& format = ITexture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false) = 0;
	virtual std::shared_ptr<ITexture>         LoadTexture2D(const std::string& fileName) = 0;
	virtual std::shared_ptr<ITexture>         LoadTextureCube(const std::string& fileName) = 0;

	virtual std::shared_ptr<IMaterial>        CreateMaterial(size_t Size) = 0;
	virtual std::shared_ptr<IRenderTarget>    CreateRenderTarget() = 0;
	virtual std::shared_ptr<IQuery>           CreateQuery(IQuery::QueryType queryType = IQuery::QueryType::Timer, size_t numBuffers = 3) = 0;
	
	virtual std::shared_ptr<ISamplerState>    CreateSamplerState() = 0;
	virtual std::shared_ptr<IBlendState>      CreateBlendState() = 0;
	virtual std::shared_ptr<IDepthStencilState> CreateDepthStencilState() = 0;
	virtual std::shared_ptr<IRasterizerState> CreateRasterizerState() = 0;
	virtual std::shared_ptr<IPipelineState>   CreatePipelineState() = 0;
	virtual std::shared_ptr<IBuffer>          CreateVoidVertexBuffer(const void* data, size_t count, size_t offset, size_t stride) = 0;
	virtual std::shared_ptr<IBuffer>          CreateVoidIndexBuffer(const void* data, size_t count, size_t offset, size_t stride) = 0;
	virtual std::shared_ptr<IConstantBuffer>  CreateConstantBuffer(const void* data, size_t size) = 0;
	virtual std::shared_ptr<IStructuredBuffer> CreateStructuredBuffer(void* data, size_t count, size_t stride, CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false) = 0;

	// Templates

	template <typename T>
	inline std::shared_ptr<T> GetRenderObjectWithCast(RenderObjectID ID) const
	{
		std::shared_ptr<IRenderObject> renderObject = GetRenderObject(ID);
		if (renderObject == nullptr)
			return nullptr;
		return std::dynamic_pointer_cast<T, IRenderObject>(renderObject);
	}

	template<typename T>
	inline std::shared_ptr<IBuffer> CreateVertexBuffer(const std::vector<T>& data)
	{
		return CreateVoidVertexBuffer(data.data(), data.size(), 0, sizeof(T));
	}
	template<typename T>
	inline std::shared_ptr<IBuffer> CreateVertexBuffer(const T* data, size_t count)
	{
		return CreateVoidVertexBuffer(data, count, 0, sizeof(T));
	}

	template<typename T>
	inline std::shared_ptr<IBuffer> CreateIndexBuffer(const std::vector<T>& data)
	{
		return CreateVoidIndexBuffer(data.data(), data.size(), 0, sizeof(T));
	}
	template<typename T>
	inline std::shared_ptr<IBuffer> CreateIndexBuffer(const T* data, size_t count)
	{
		return CreateVoidIndexBuffer(data, count, 0, sizeof(T));
	}

	template<typename T>
	inline std::shared_ptr<IConstantBuffer> CreateConstantBuffer(const T& data)
	{
		return CreateConstantBuffer(&data, sizeof(T));
	}

	template<typename T>
	inline std::shared_ptr<IStructuredBuffer> CreateStructuredBuffer(const std::vector<T>& data, CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false)
	{
		return CreateStructuredBuffer((void*)data.data(), data.size(), sizeof(T), cpuAccess, gpuWrite);
	}
};


ZN_INTERFACE ZN_API __declspec(novtable, uuid("19615D02-1592-4420-8D0C-3DB4F7C6186A")) IRenderDevice : public IManager
{
	virtual ~IRenderDevice() {}

	virtual bool Initialize() = 0;
	virtual void Finalize() = 0;

	virtual std::string                GetDeviceName() const = 0;
	virtual const RenderDeviceType     GetDeviceType() const = 0;
	virtual IBaseManager*              GetBaseManager() const = 0;

	virtual IRenderPrimitivesFactory&  GetPrimitivesFactory() const = 0;
	virtual IRenderObjectsFactory&     GetObjectsFactory() const = 0;

	virtual std::shared_ptr<ITexture>  GetDefaultTexture() const = 0;
};
