#pragma once

// FORWARD BEGIN
ZN_INTERFACE IRenderWindow;
ZN_INTERFACE IGeometry;
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

	virtual std::shared_ptr<IGeometry> CreateLine(const glm::vec3& _dest) = 0; // Create a line
	virtual std::shared_ptr<IGeometry> CreateLines(size_t count = 100) = 0;
	virtual std::shared_ptr<IGeometry> CreateLines(const std::vector<glm::vec3>& Points) = 0;
	virtual std::shared_ptr<IGeometry> CreateLines2(const std::vector<glm::vec3>& Points) = 0;
	virtual std::shared_ptr<IGeometry> CreatePlane(const glm::vec3& N = glm::vec3(0, 1, 0)) = 0; // Create a plane in 3D.
	virtual std::shared_ptr<IGeometry> CreateSphere(float Radius = 1.0f) = 0;
	virtual std::shared_ptr<IGeometry> CreateCube() = 0;
	virtual std::shared_ptr<IGeometry> CreateBBox() = 0;
	virtual std::shared_ptr<IGeometry> CreateFrustum(const Frustum& Frustum) = 0;
	virtual std::shared_ptr<IGeometry> CreateCone(float diameter = 0.5f, float height = 1.0f) = 0;
	virtual std::shared_ptr<IGeometry> CreateTorus(float Radius = 5.0f, float Thickness = 1.0f) = 0;
	virtual std::shared_ptr<IGeometry> CreateQuad() = 0;

	// UI meshes
	virtual std::shared_ptr<IGeometry> CreateUIQuad(glm::vec2 Size, glm::vec2 TexCoordStart = glm::vec2(0.0f), glm::vec2 TexCoordEnd = glm::vec2(1.0f)) = 0;
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
	virtual void                              ClearCache() = 0;

	virtual std::shared_ptr<IRenderWindow>    CreateRenderWindow(std::unique_ptr<IznNativeWindow> NativeWindow, bool vSync) = 0;
	virtual std::shared_ptr<IGeometry>        CreateGeometry() = 0;
	virtual std::shared_ptr<IModel>           CreateModel() = 0;

	virtual std::shared_ptr<ITexture>         CreateEmptyTexture() = 0;
	virtual std::shared_ptr<ITexture>         CreateTexture2D(size_t width, size_t height, size_t slices, const ITexture::TextureFormat& format = ITexture::TextureFormat(), EAccess cpuAccess = EAccess::None) = 0;
	virtual std::shared_ptr<ITexture>         CreateTextureCube(size_t size, const ITexture::TextureFormat& format = ITexture::TextureFormat(), EAccess cpuAccess = EAccess::None) = 0;


	virtual std::shared_ptr<IMaterial>        CreateMaterial(const std::string& MaterialName) = 0;
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
	virtual std::shared_ptr<IStructuredBuffer> CreateStructuredBuffer(const void* data, size_t count, size_t stride, EAccess cpuAccess = EAccess::None) = 0;

	virtual std::shared_ptr<IShader>            LoadShader(EShaderType type, const std::string& fileName, const std::string& entryPoint, const IShader::ShaderMacros& shaderMacros = IShader::ShaderMacros(), IShaderInputLayout* CustomLayout = nullptr) = 0;

	virtual std::shared_ptr<IBuffer>            LoadVoidBuffer(const std::shared_ptr<IByteBuffer>& ByteBuffer) = 0;
	virtual std::shared_ptr<IConstantBuffer>    LoadConstantBuffer(const std::shared_ptr<IByteBuffer>& ByteBuffer) = 0;
	virtual std::shared_ptr<IStructuredBuffer>  LoadStructuredBuffer(const std::shared_ptr<IByteBuffer>& ByteBuffer) = 0;

	virtual std::shared_ptr<IBuffer>            LoadVoidBuffer(const std::shared_ptr<IXMLReader>& Reader) = 0;
	virtual std::shared_ptr<IConstantBuffer>    LoadConstantBuffer(const std::shared_ptr<IXMLReader>& Reader) = 0;
	virtual std::shared_ptr<IStructuredBuffer>  LoadStructuredBuffer(const std::shared_ptr<IXMLReader>& Reader) = 0;

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
	inline std::shared_ptr<IStructuredBuffer> CreateStructuredBuffer(const std::vector<T>& data, EAccess cpuAccess = EAccess::None)
	{
		return CreateStructuredBuffer((const void*)data.data(), data.size(), sizeof(T), cpuAccess);
	}
};


ZN_INTERFACE ZN_API IRenderDevice
{
	virtual ~IRenderDevice() {}

	virtual bool Initialize() = 0;
	virtual void Finalize() = 0;

	virtual std::string                GetDeviceName() const = 0;
	virtual const RenderDeviceType     GetDeviceType() const = 0;
	virtual IBaseManager&              GetBaseManager() const = 0;

	virtual IRenderPrimitivesFactory&  GetPrimitivesFactory() const = 0;
	virtual IRenderObjectsFactory&     GetObjectsFactory() const = 0;
};

ZN_INTERFACE ZN_API IRenderDeviceOwner
{
	virtual void SetRenderDevice(IRenderDevice * RenderDevice) = 0;
	virtual IRenderDevice& GetRenderDevice() const = 0;
};
