#include "stdafx.h"

// General
#include "GeometryBase.h"

GeometryBase::GeometryBase(IRenderDevice& RenderDevice)
	: Object(RenderDevice.GetBaseManager())
	, m_RenderDevice(RenderDevice)
	, m_VertexBuffer(nullptr)
	, m_IndexBuffer(nullptr)
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
	m_IndexBuffer = IndeBuffer;
}

void GeometryBase::SetPrimitiveTopology(PrimitiveTopology Topology)
{
	m_PrimitiveTopology = Topology;
}

PrimitiveTopology GeometryBase::GetPrimitiveTopology() const
{
	return m_PrimitiveTopology;
}

void GeometryBase::Accept(IVisitor * visitor, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs)
{
	visitor->Visit(shared_from_this(), Material, GeometryDrawArgs);
}



//
// IObjectLoadSave
//
void GeometryBase::Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	BoundingBox bounds;
	bounds.Load(ByteBuffer);
	SetBounds(bounds);

	PrimitiveTopology primitiveTopology;
	ByteBuffer->read(&primitiveTopology);
	SetPrimitiveTopology(primitiveTopology);

	uint32 vertexBuffersCnt = 0;
	ByteBuffer->read(&vertexBuffersCnt);
	for (uint32 i = 0; i < vertexBuffersCnt; i++)
	{
		BufferBinding bufferBinding;

		{
			uint32 signature;
			ByteBuffer->read(&signature);
			ByteBuffer->readString(&bufferBinding.Name);
			ByteBuffer->read(&bufferBinding.Index);
		}

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

void GeometryBase::Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) const
{
	BoundingBox modelBBox = GetBounds();
	modelBBox.Save(ByteBuffer);

	ByteBuffer->write(&m_PrimitiveTopology);

	uint32 vertexBuffersCnt = m_VertexBuffers.size();
	ByteBuffer->write(&vertexBuffersCnt);
	for (const auto& b : m_VertexBuffers)
	{
		if (const auto& loadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(b.second))
		{
			{
				ByteBuffer->write(&cBufferBindingSignature);
				ByteBuffer->writeString(b.first.Name);
				ByteBuffer->write(&b.first.Index);
			}

			loadableFromFile->Save(ByteBuffer);
		}
		else
			_ASSERT(false);
	}

	uint8 isVertexBufferExists = (m_VertexBuffer != nullptr) ? 0x01 : 0x00;
	ByteBuffer->write(&isVertexBufferExists);
	if (isVertexBufferExists)
	{
		if (const auto& loadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(m_VertexBuffer))
		{
			loadableFromFile->Save(ByteBuffer);
		}
		else
			_ASSERT(false);
	}

	uint8 isIndexBufferExists = (m_IndexBuffer != nullptr) ? 0x01 : 0x00;
	ByteBuffer->write(&isIndexBufferExists);
	if (isIndexBufferExists)
	{
		if (const auto& loadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(m_IndexBuffer))
		{
			loadableFromFile->Save(ByteBuffer);
		}
		else
			_ASSERT(false);
	}
}

namespace
{
	std::string PrimitiveTopologyToString(PrimitiveTopology PrimitiveTopology)
	{
		switch (PrimitiveTopology)
		{
		case PrimitiveTopology::PointList:
			return "PointList";
		case PrimitiveTopology::LineList:
			return "LineList";
		case PrimitiveTopology::LineStrip:
			return "LineStrip";
		case PrimitiveTopology::TriangleList:
			return "TriangleList";
		case PrimitiveTopology::TriangleStrip:
			return "TriangleStrip";
		}
		throw CException("Primitive topology '%d' is invalid.", PrimitiveTopology);
	}

	PrimitiveTopology PrimitiveTopologyFromString(const std::string& PrimitiveTopology)
	{
		if (PrimitiveTopology == "PointList")
			return PrimitiveTopology::PointList;
		else if (PrimitiveTopology == "LineList")
			return PrimitiveTopology::LineList;
		else if(PrimitiveTopology == "LineStrip")
			return PrimitiveTopology::LineStrip;
		else if (PrimitiveTopology == "TriangleList")
			return PrimitiveTopology::TriangleList;
		else if (PrimitiveTopology == "TriangleStrip")
			return PrimitiveTopology::TriangleStrip;
		throw CException("Primitive topology '%s' is invalid.", PrimitiveTopology.c_str());
	}
}

void GeometryBase::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	BoundingBox bounds;
	bounds.Load(Reader);
	SetBounds(bounds);

	m_PrimitiveTopology = PrimitiveTopologyFromString(Reader->GetStrAttribute("PrimitiveTopology"));

	if (Reader->IsChildExists("VertexBuffers"))
	{
		auto vertexBuffersXML = Reader->GetChild("VertexBuffers");
		for (const auto& vertexBufferBindingXML : vertexBuffersXML->GetChilds())
		{
			BufferBinding bufferBinding(vertexBufferBindingXML->GetName(), vertexBufferBindingXML->GetUIntAttribute("Index"));

			auto vertexBufferXML = vertexBufferBindingXML->GetChilds().front();
			std::shared_ptr<IBuffer> vertexBuffer = m_RenderDevice.GetObjectsFactory().LoadVoidBuffer(vertexBufferXML);
			AddVertexBuffer(bufferBinding, vertexBuffer);
		}
	}

	if (Reader->IsChildExists("VertexBuffer"))
	{
		auto vertexBufferXML = Reader->GetChild("VertexBuffer");
		std::shared_ptr<IBuffer> vertexBuffer = m_RenderDevice.GetObjectsFactory().LoadVoidBuffer(vertexBufferXML);
		SetVertexBuffer(vertexBuffer);
	}

	if (Reader->IsChildExists("IndexBuffer"))
	{
		auto indexBufferXML = Reader->GetChild("IndexBuffer");
		std::shared_ptr<IBuffer> indexBuffer = m_RenderDevice.GetObjectsFactory().LoadVoidBuffer(indexBufferXML);
		SetIndexBuffer(indexBuffer);
	}
}

void GeometryBase::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	BoundingBox geometryBBox = GetBounds();
	geometryBBox.Save(Writer);

	Writer->SetStrAttribute(PrimitiveTopologyToString(m_PrimitiveTopology), "PrimitiveTopology");

	if (false == m_VertexBuffers.empty())
	{
		auto vertexBuffersXML = Writer->CreateChild("VertexBuffers");

		for (const auto& vertexBuffer : m_VertexBuffers)
		{
			auto vertexBufferBindingXML = vertexBuffersXML->CreateChild(vertexBuffer.first.Name);
			vertexBufferBindingXML->SetUIntAttribute(vertexBuffer.first.Index, "Index");

			auto vertexBufferXML = vertexBufferBindingXML->CreateChild("buffer_TEMP");
			std::dynamic_pointer_cast<IObjectLoadSave>(vertexBuffer.second)->Save(vertexBufferXML);
		}
	}

	if (m_VertexBuffer)
	{
		auto vertexbufferXML = Writer->CreateChild("VertexBuffer");

		auto bufferXML = vertexbufferXML->CreateChild("buffer_TEMP");
		std::dynamic_pointer_cast<IObjectLoadSave>(m_VertexBuffer)->Save(bufferXML);
	}

	if (m_IndexBuffer)
	{
		auto indexBufferXML = Writer->CreateChild("IndexBuffer");

		auto bufferXML = indexBufferXML->CreateChild("buffer_TEMP");
		std::dynamic_pointer_cast<IObjectLoadSave>(m_IndexBuffer)->Save(bufferXML);
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
	if (fixedArgs.IndexCnt == UINT_MAX && m_IndexBuffer != nullptr)
		fixedArgs.IndexCnt = m_IndexBuffer->GetElementCount();

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
