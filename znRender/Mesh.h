#pragma once

#include "BufferBinding.h"
#include "Buffer.h"
#include "PrimitiveTopology.h"
#include "Material.h"
#include "RenderEvents.h"
#include "ConstantBuffer.h"

class IMesh : public Object, public std::enable_shared_from_this<IMesh>
{
public:
	virtual ~IMesh() {};

	virtual void AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer) = 0;
	virtual void SetVertexBuffer(std::shared_ptr<IBuffer> buffer) = 0;
	virtual void SetIndexBuffer(std::shared_ptr<IBuffer> buffer) = 0;

	virtual void SetPrimitiveTopology(PrimitiveTopology _topology) = 0;

	virtual void SetMaterial(std::shared_ptr<const Material> material) = 0;
	virtual std::shared_ptr<const Material> GetMaterial() const = 0;

	virtual bool Render(const RenderEventArgs* renderEventArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0) = 0;

	virtual bool Accept(IVisitor& visitor, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0) = 0;
};
