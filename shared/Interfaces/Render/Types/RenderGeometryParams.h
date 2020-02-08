#pragma once

struct ZN_API SGeometryDrawArgs
{
	SGeometryDrawArgs(UINT IndexStartLocation = 0, UINT IndexCnt = UINT_MAX, UINT VertexStartLocation = 0, UINT VertexCnt = UINT_MAX)
		: IndexStartLocation(IndexStartLocation)
		, IndexCnt(IndexCnt)
		, VertexStartLocation(VertexStartLocation)
		, VertexCnt(VertexCnt)
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
