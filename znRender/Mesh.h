#pragma once

#include "BufferBinding.h"
#include "Buffer.h"
#include "PrimitiveTopology.h"
#include "Material.h"
#include "RenderEvents.h"
#include "ConstantBuffer.h"

enum SceneNodeTypes : uint8
{
	SN_TYPE_NONE = 0,
	SN_TYPE_DEBUG,
	SN_TYPE_FONT,

	SN_TYPE_SKY,
	SN_TYPE_WDL_NODE,
	SN_TYPE_ADT_CHUNK,
	SN_TYPE_WMO,
	SN_TYPE_M2,
	SN_TYPE_LQ,

	SN_TYPE_STLK
};

class IMesh : public Object
{
public:
	virtual ~IMesh() {};

	virtual SceneNodeTypes GetType() const = 0;
	virtual void SetType(SceneNodeTypes type) = 0;

	virtual void AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer) = 0;
	virtual void SetVertexBuffer(std::shared_ptr<IBuffer> buffer) = 0;
	virtual void SetIndexBuffer(std::shared_ptr<IBuffer> buffer) = 0;

	virtual void SetPrimitiveTopology(PrimitiveTopology _topology) = 0;

	virtual void SetMaterial(std::shared_ptr<const Material> material) = 0;
	virtual std::shared_ptr<const Material> GetMaterial() const = 0;

	virtual bool Render(RenderEventArgs& renderEventArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0) = 0;

	virtual bool Accept(IVisitor& visitor, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0) = 0;

private:
	SceneNodeTypes m_Type;
};
