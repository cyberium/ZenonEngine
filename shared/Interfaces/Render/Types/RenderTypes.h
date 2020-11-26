#pragma once

/**
 * Flags to specify which value should be cleared.
 */
enum class ZN_API ClearFlags : uint32
{
	Color = 1 << 0,
	Depth = 1 << 1,
	Stencil = 1 << 2,
	DepthStencil = Depth | Stencil,
	All = Color | Depth | Stencil,
};


/**
  * CPU Access. Used for textures and Buffers
 */
enum ZN_API EAccess : uint32
{
	None = 0,                 // No CPU or GPU access to this texture is necessary.
	CPURead = (1 << 0),          // CPU reads permitted.
	CPUWrite = (1 << 1),         // CPU writes permitted.
	GPUWrite = (1 << 2)
};


enum class ZN_API EShaderType : uint32
{
	UnknownShaderType = 0,
	VertexShader,
	TessellationControlShader,      // Hull Shader in DirectX
	TessellationEvaluationShader,   // Domain Shader in DirectX
	GeometryShader,
	PixelShader,
	ComputeShader,
};

/**
 * Used for meshes
 */
enum class ZN_API PrimitiveTopology : uint32
{
	PointList,
	LineList,
	LineStrip,
	TriangleList,
	TriangleStrip
};


struct ZN_API BufferBinding
{
	BufferBinding()
		: Index(0)
	{}
	BufferBinding(const std::string& name, uint32 index)
		: Name(name)
		, Index(index)
	{}
	virtual ~BufferBinding()
	{}

	inline bool operator==(const BufferBinding& rhs) const
	{
		return (Name == rhs.Name) && (Index == rhs.Index);
	}

	inline void Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
	{
		uint32 signature;
		ByteBuffer->read(&signature);
		ByteBuffer->readString(&Name);
		ByteBuffer->read(&Index);
	}

	inline void Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) const
	{
		ByteBuffer->write(&cBufferBindingSignature);
		ByteBuffer->writeString(Name);
		ByteBuffer->write(&Index);
	}

	std::string  Name;
	uint32       Index;
};

namespace std
{
	template<>
	struct hash<BufferBinding>
	{
		size_t operator()(const BufferBinding& buffer) const noexcept
		{
			std::hash<std::string> hash;
			return hash(buffer.Name + std::to_string(buffer.Index));
		}
	};
}


struct ZN_API SGeometryDrawArgs
{
	SGeometryDrawArgs(UINT IndexStartLocation = 0, UINT IndexCnt = UINT_MAX, UINT VertexStartLocation = 0, UINT VertexCnt = UINT_MAX)
		: IndexStartLocation(IndexStartLocation)
		, IndexCnt(IndexCnt)
		, VertexStartLocation(VertexStartLocation)
		, VertexCnt(VertexCnt)
		, InstanceStartIndex(0)
		, InstanceCnt(UINT_MAX)
	{}

	UINT IndexStartLocation;
	UINT IndexCnt;
	UINT VertexStartLocation;
	UINT VertexCnt;
	UINT InstanceStartIndex;
	UINT InstanceCnt;
};