#include "stdafx.h"

// General
#include "GeometryBase.h"

GeometryBase::GeometryBase()
    : m_VertexBuffer(nullptr)
    , m_pIndexBuffer(nullptr)
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

bool GeometryBase::Accept(IVisitor * visitor, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	return visitor->Visit(this, Material, GeometryDrawArgs);
}



//
// Protected
//
void GeometryBase::BindVertexBuffersToVertexShader(const IShader* VertexShader) const
{
	if (m_VertexBuffer != nullptr)
	{
		m_VertexBuffer->Bind(0, VertexShader, IShaderParameter::Type::Buffer);
	}
	else
	{
		for (const auto& buffer : m_VertexBuffers)
		{
			UINT slotID = VertexShader->GetInputLayout().GetSemanticSlot(buffer.first);
			if (slotID != UINT_MAX)
				buffer.second->Bind(slotID, VertexShader, IShaderParameter::Type::Buffer);
		}
	}
}

void GeometryBase::UnbindVertexBuffersFromVertexShader(const IShader* VertexShader) const
{
	if (m_VertexBuffer != nullptr)
	{
		m_VertexBuffer->UnBind(0, VertexShader, IShaderParameter::Type::Buffer);
	}
	else
	{
		for (const auto& buffer : m_VertexBuffers)
		{
			UINT slotID = VertexShader->GetInputLayout().GetSemanticSlot(buffer.first);
			if (slotID != UINT_MAX)
				buffer.second->UnBind(slotID, VertexShader, IShaderParameter::Type::Buffer);
		}
	}
}

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

	return fixedArgs;
}

SGeometryDrawInstancedArgs GeometryBase::FixGeometryDrawInstancedArgs(const SGeometryDrawInstancedArgs & GeometryDrawInstancedArgs) const
{
	SGeometryDrawInstancedArgs fixedArgs;

	fixedArgs.InstanceStartIndex = GeometryDrawInstancedArgs.InstanceStartIndex;
	fixedArgs.InstanceCnt = GeometryDrawInstancedArgs.InstanceCnt;
	if (fixedArgs.InstanceCnt == UINT_MAX)
		_ASSERT_EXPR(false, L"GeometryBase: Specify instance count same as instance buffer size.");

	fixedArgs.IndexStartLocation = GeometryDrawInstancedArgs.IndexStartLocation;
	fixedArgs.IndexCnt = GeometryDrawInstancedArgs.IndexCnt;
	if (fixedArgs.IndexCnt == UINT_MAX && m_pIndexBuffer != nullptr)
		fixedArgs.IndexCnt = m_pIndexBuffer->GetElementCount();

	fixedArgs.VertexStartLocation = GeometryDrawInstancedArgs.VertexStartLocation;
	fixedArgs.VertexCnt = GeometryDrawInstancedArgs.VertexCnt;
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

	return fixedArgs;
}
