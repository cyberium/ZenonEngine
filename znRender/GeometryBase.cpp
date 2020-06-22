#include "stdafx.h"

// General
#include "GeometryBase.h"

GeometryBase::GeometryBase(IRenderDevice& RenderDevice)
	: m_RenderDevice(RenderDevice)
	, m_VertexBuffer(nullptr)
	, m_pIndexBuffer(nullptr)
	, m_PrimitiveTopology(PrimitiveTopology::TriangleList)
{
}

GeometryBase::~GeometryBase()
{
}


void GeometryBase::SetBounds(const BoundingBox& Bounds)
{
	m_Bounds = Bounds;
}

BoundingBox GeometryBase::GetBounds() const
{
	return m_Bounds;
}

void GeometryBase::AddVertexBuffer(const BufferBinding& binding, const std::shared_ptr<IBuffer>& VertexBuffer)
{
    m_VertexBuffers[binding] = VertexBuffer;
}

void GeometryBase::SetVertexBuffer(const std::shared_ptr<IBuffer>& GlobalVertexBuffer)
{
    m_VertexBuffer = GlobalVertexBuffer;
}

void GeometryBase::SetIndexBuffer(const std::shared_ptr<IBuffer>& IndeBuffer)
{
    m_pIndexBuffer = IndeBuffer;
}

void GeometryBase::SetPrimitiveTopology(PrimitiveTopology Topology)
{
	m_PrimitiveTopology = Topology;
}

PrimitiveTopology GeometryBase::GetPrimitiveTopology() const
{
	return m_PrimitiveTopology;
}

void GeometryBase::Accept(IVisitor * visitor, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	visitor->Visit(this, Material, GeometryDrawArgs);
}

void GeometryBase::Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	PrimitiveTopology primitiveTopology;
	ByteBuffer->read(&primitiveTopology);
	SetPrimitiveTopology(primitiveTopology);

	uint32 vertexBuffersCnt = 0;
	ByteBuffer->read(&vertexBuffersCnt);
	for (uint32 i = 0; i < vertexBuffersCnt; i++)
	{
		BufferBinding bufferBinding;
		bufferBinding.Load(ByteBuffer);

		std::shared_ptr<IBuffer> vertexBuffer = m_RenderDevice.GetObjectsFactory().LoadVoidBuffer(ByteBuffer);
		AddVertexBuffer(bufferBinding, vertexBuffer);
	}

	uint8 isVertexBufferExists = 0x00;
	ByteBuffer->read(&isVertexBufferExists);
	if (isVertexBufferExists)
	{
		std::shared_ptr<IBuffer> vertexBuffer = m_RenderDevice.GetObjectsFactory().LoadVoidBuffer(ByteBuffer);
		SetVertexBuffer(vertexBuffer);
	}

	uint8 isIndexBufferExists = 0x00;
	ByteBuffer->read(&isIndexBufferExists);
	if (isIndexBufferExists)
	{
		std::shared_ptr<IBuffer> indexBuffer = m_RenderDevice.GetObjectsFactory().LoadVoidBuffer(ByteBuffer);
		SetIndexBuffer(indexBuffer);
	}
}

void GeometryBase::Save(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	ByteBuffer->write(&m_PrimitiveTopology);

	uint32 vertexBuffersCnt = m_VertexBuffers.size();
	ByteBuffer->write(&vertexBuffersCnt);
	for (const auto& b : m_VertexBuffers)
	{
		if (const auto& loadableFromFile = std::dynamic_pointer_cast<ILoadableFromFile>(b.second))
		{
			b.first.Save(ByteBuffer);
			loadableFromFile->Save(ByteBuffer);
		}
		else
			_ASSERT(false);
	}

	uint8 isVertexBufferExists = (m_VertexBuffer != nullptr) ? 0x01 : 0x00;
	ByteBuffer->write(&isVertexBufferExists);
	if (isVertexBufferExists)
	{
		if (const auto& loadableFromFile = std::dynamic_pointer_cast<ILoadableFromFile>(m_VertexBuffer))
		{
			loadableFromFile->Save(ByteBuffer);
		}
		else
			_ASSERT(false);
	}

	uint8 isIndexBufferExists = (m_pIndexBuffer != nullptr) ? 0x01 : 0x00;
	ByteBuffer->write(&isIndexBufferExists);
	if (isIndexBufferExists)
	{
		if (const auto& loadableFromFile = std::dynamic_pointer_cast<ILoadableFromFile>(m_pIndexBuffer))
		{
			loadableFromFile->Save(ByteBuffer);
		}
		else
			_ASSERT(false);
	}
}



//
// Protected
//

SGeometryDrawArgs GeometryBase::FixGeometryDrawArgs(const SGeometryDrawArgs & GeometryDrawArgs) const
{
	SGeometryDrawArgs fixedArgs;

	fixedArgs.IndexStartLocation = GeometryDrawArgs.IndexStartLocation;
	fixedArgs.IndexCnt = GeometryDrawArgs.IndexCnt;
	if (fixedArgs.IndexCnt == UINT_MAX && m_pIndexBuffer != nullptr)
		fixedArgs.IndexCnt = m_pIndexBuffer->GetElementCount();

	fixedArgs.VertexStartLocation = GeometryDrawArgs.VertexStartLocation;
	fixedArgs.VertexCnt = GeometryDrawArgs.VertexCnt;
	if (fixedArgs.VertexCnt == UINT_MAX)
	{
		if (m_VertexBuffer != nullptr)
		{
			fixedArgs.VertexCnt = m_VertexBuffer->GetElementCount();
		}
		else if (!m_VertexBuffers.empty())
		{
			fixedArgs.VertexCnt = (*m_VertexBuffers.begin()).second->GetElementCount();
		}
		else
		{
			_ASSERT(false);
		}
	}

	fixedArgs.InstanceStartIndex = GeometryDrawArgs.InstanceStartIndex;
	fixedArgs.InstanceCnt = GeometryDrawArgs.InstanceCnt;

	return fixedArgs;
}
