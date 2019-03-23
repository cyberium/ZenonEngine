// Template specializations for vertex buffers (with std::vector)
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer< std::vector<float> >(const std::vector<float>& data);
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer< std::vector<vec2> >(const std::vector<vec2>& data);
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer< std::vector<vec3> >(const std::vector<vec3>& data);
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer< std::vector<vec4> >(const std::vector<vec4>& data);


// Template specializations for vertex buffers (with common type)
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer<float>(const float* data, uint32 count);
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer<vec2>(const vec2* data, uint32 count);
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer<vec3>(const vec3* data, uint32 count);
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer<vec4>(const vec4* data, uint32 count);


// Template specializations for index buffers (with std::vector)
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateIndexBuffer< std::vector<uint16> >(const std::vector<uint16>& data);
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateIndexBuffer< std::vector<uint32> >(const std::vector<uint32>& data);


// Template specializations for index buffers (with common type)
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateIndexBuffer<uint16>(const uint16* data, uint32 count);
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateIndexBuffer<uint32>(const uint32* data, uint32 count);


// Non-specialized template methods.
template< typename T >
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer(const T& data)
{
	static_assert(false, "This function must be specialized.");
	return NULL;
}

template< typename T >
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer(const T* data, uint32 count)
{
	static_assert(false, "This function must be specialized.");
	return NULL;
}

template<typename T>
std::shared_ptr<IBuffer> IRenderDevice::CreateIndexBuffer(const T& data)
{
	static_assert(false, "This function must be specialized.");
	return NULL;
}

template<typename T>
std::shared_ptr<IBuffer> IRenderDevice::CreateIndexBuffer(const T* data, uint32 count)
{
	static_assert(false, "This function must be specialized.");
	return NULL;
}

template< typename T >
std::shared_ptr<ConstantBuffer> IRenderDevice::CreateConstantBuffer(const T& data)
{
	return CreateConstantBuffer(&data, sizeof(T));
}

template<typename T>
std::shared_ptr<StructuredBuffer> IRenderDevice::CreateStructuredBuffer(const std::vector<T>& data, CPUAccess cpuAccess, bool gpuWrite)
{
	size_t stride = sizeof(T);
	size_t numElements = data.size();
	return CreateStructuredBuffer((void*)data.data(), (uint32)numElements, (uint32)stride, cpuAccess, gpuWrite);
}