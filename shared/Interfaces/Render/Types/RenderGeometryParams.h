#pragma once

// FORWARD BEGIN
ZN_INTERFACE IMaterial;
// FORWARD END

struct ZN_API SGeometryDrawArgs
{
	SGeometryDrawArgs()
		: IndexStartLocation(0)
		, IndexCnt(UINT_MAX)
		, VertexStartLocation(0)
		, VertexCnt(UINT_MAX)
	{}

	UINT IndexStartLocation;
	UINT IndexCnt;
	UINT VertexStartLocation;
	UINT VertexCnt;

	inline bool IsDefault() const
	{
		return (IndexStartLocation == 0) && (IndexCnt == UINT_MAX) && (VertexStartLocation == 0) && (VertexCnt == UINT_MAX);
	}
};


struct ZN_API SGeometryDrawInstancedArgs
{
	SGeometryDrawInstancedArgs()
		: InstanceStartIndex(0)
		, InstanceCnt(UINT_MAX)
		, IndexStartLocation(0)
		, IndexCnt(UINT_MAX)
		, VertexStartLocation(0)
		, VertexCnt(UINT_MAX)
	{}

	UINT InstanceStartIndex;
	UINT InstanceCnt;
	UINT IndexStartLocation;
	UINT IndexCnt;
	UINT VertexStartLocation;
	UINT VertexCnt;

	inline bool IsDefault() const
	{
		return (InstanceStartIndex == 0) && (InstanceCnt == UINT_MAX) && (IndexStartLocation == 0) && (IndexCnt == UINT_MAX) && (VertexStartLocation == 0) && (VertexCnt == UINT_MAX);
	}
};


struct ZN_API SRenderGeometryArgs
{
	std::shared_ptr<const IMaterial> Material;
	SGeometryDrawArgs GeometryDrawArgs;

	inline SRenderGeometryArgs& operator=(const SRenderGeometryArgs& Other)
	{
		Material = Other.Material;
		GeometryDrawArgs = Other.GeometryDrawArgs;
		return *this;
	}
};