#pragma once

enum class ZN_API RenderDeviceType
{
	RenderDeviceType_None = 0,
	RenderDeviceType_DirectX11,
	RenderDeviceType_DirectX12,
	RenderDeviceType_OpenGL
};

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

	std::string  Name;
	uint32       Index;
};

namespace std
{
	template<>
	struct hash<BufferBinding>
	{
		inline size_t operator()(const BufferBinding& buffer) const noexcept
		{
			std::hash<std::string> hash;
			return hash(buffer.Name + std::to_string(buffer.Index));
		}
	};
}


struct ZN_API SGeometryDrawArgs
{
	SGeometryDrawArgs(uint32 IndexStartLocation = 0, uint32 IndexCnt = UINT_MAX, uint32 VertexStartLocation = 0, uint32 VertexCnt = UINT_MAX)
		: IndexStartLocation(IndexStartLocation)
		, IndexCnt(IndexCnt)
		, VertexStartLocation(VertexStartLocation)
		, VertexCnt(VertexCnt)
		, InstanceStartIndex(0)
		, InstanceCnt(UINT_MAX)
	{}

	uint32 IndexStartLocation;
	uint32 IndexCnt;
	uint32 VertexStartLocation;
	uint32 VertexCnt;
	uint32 InstanceStartIndex;
	uint32 InstanceCnt;
};