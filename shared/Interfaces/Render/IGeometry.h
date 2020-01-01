#pragma once

// FORWARD BEGIN
ZN_INTERFACE IBuffer;
ZN_INTERFACE IShader;
ZN_INTERFACE IConstantBuffer;
class RenderEventArgs;
// FORWARD END

ZN_INTERFACE ZN_API IGeometry 
	: public std::enable_shared_from_this<IGeometry>
{
	typedef std::unordered_map<BufferBinding, std::shared_ptr<IBuffer>> BufferMap;

	struct ZN_API SRenderGeometryParams
	{
		SRenderGeometryParams()
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

	virtual ~IGeometry() {}

	virtual void                                    SetBounds(const BoundingBox& Bounds) = 0;
	virtual const BoundingBox&                      GetBounds() const = 0;

	virtual void                                    AddVertexBuffer(const BufferBinding& Binding, std::shared_ptr<IBuffer> Buffer) = 0;
	virtual void                                    SetVertexBuffer(std::shared_ptr<IBuffer> Buffer) = 0;
	virtual void                                    SetIndexBuffer(std::shared_ptr<IBuffer> Buffer) = 0;

	virtual void                                    SetPrimitiveTopology(PrimitiveTopology Topology) = 0;

	virtual bool                                    Render(const RenderEventArgs* RenderEventArgs, const IShader* VertexShader, const IConstantBuffer* PerObject, const SRenderGeometryParams& RenderGeometryParams) = 0;
};
