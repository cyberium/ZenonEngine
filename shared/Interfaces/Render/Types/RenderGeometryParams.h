#pragma once

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
};


struct ZN_API SGeometryDrawInstancedArgs
	: public SGeometryDrawArgs
{
	SGeometryDrawInstancedArgs()
		: SGeometryDrawArgs()
		, InstanceStartIndex(0)
		, InstanceCnt(UINT_MAX)
	{}

	UINT InstanceStartIndex;
	UINT InstanceCnt;
};
