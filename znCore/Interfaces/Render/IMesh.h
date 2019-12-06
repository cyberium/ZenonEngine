#pragma once

// FORWARD BEGIN
ZN_INTERFACE IBuffer;
ZN_INTERFACE IMaterial;
ZN_INTERFACE IConstantBuffer;
class RenderEventArgs;
// FORWARD END

ZN_INTERFACE OW_ENGINE_API IMesh : public std::enable_shared_from_this<IMesh>
{
	typedef std::map<BufferBinding, std::shared_ptr<IBuffer>> BufferMap;

	virtual ~IMesh() {}

	virtual void                                    AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer) = 0;
	virtual void                                    SetVertexBuffer(std::shared_ptr<IBuffer> buffer) = 0;
	virtual void                                    SetIndexBuffer(std::shared_ptr<IBuffer> buffer) = 0;

	virtual void                                    SetPrimitiveTopology(PrimitiveTopology _topology) = 0;

	virtual void                                    SetMaterial(std::shared_ptr<const IMaterial> material) = 0;
	virtual std::shared_ptr<const IMaterial>        GetMaterial() const = 0;

	virtual bool                                    Render(const RenderEventArgs* renderEventArgs, const IConstantBuffer* perObject, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0) = 0;

	virtual bool                                    Accept(IVisitor* visitor, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0) = 0;
};

typedef std::vector<std::shared_ptr<IMesh>> MeshList;