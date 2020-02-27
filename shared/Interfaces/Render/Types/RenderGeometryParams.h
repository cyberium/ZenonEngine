#pragma once

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
