#pragma once

// FORWARD BEGIN
ZN_INTERFACE IMaterial;
// FORWARD END

struct ZN_API SGeometryPartParams
{
	SGeometryPartParams()
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

struct ZN_API SRenderGeometryArgs
{
	const IMaterial* Material;
	SGeometryPartParams GeometryPartParams;

	inline SRenderGeometryArgs& operator=(const SRenderGeometryArgs& Other)
	{
		Material = Other.Material;
		GeometryPartParams = Other.GeometryPartParams;
		return *this;
	}
};