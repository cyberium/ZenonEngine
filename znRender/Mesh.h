#pragma once

#include "common.h"

#include "BufferBinding.h"
#include "Buffer.h"
#include "PrimitiveTopology.h"
#include "Material.h"
#include "RenderEvents.h"
#include "ConstantBuffer.h"

class OW_ENGINE_API IMesh : public Object, public std::enable_shared_from_this<IMesh>
{
    typedef std::map<BufferBinding, std::shared_ptr<IBuffer>> BufferMap;
public:
                                                    IMesh();
	virtual                                         ~IMesh();

	virtual void                                    AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer);
	virtual void                                    SetVertexBuffer(std::shared_ptr<IBuffer> buffer);
	virtual void                                    SetIndexBuffer(std::shared_ptr<IBuffer> buffer);

	virtual void                                    SetPrimitiveTopology(PrimitiveTopology _topology) = 0;

	virtual void                                    SetMaterial(std::shared_ptr<const Material> material);
	virtual std::shared_ptr<const Material>         GetMaterial() const;

	virtual bool                                    Render(const RenderEventArgs* renderEventArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0) = 0;

	virtual bool                                    Accept(std::shared_ptr<IVisitor> visitor, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0);

protected:
    BufferMap                                       m_VertexBuffers;
    std::shared_ptr<IBuffer>                        m_VertexBuffer;
    std::shared_ptr<IBuffer>                        m_pIndexBuffer;
    std::shared_ptr<const Material>                 m_pMaterial;
};
