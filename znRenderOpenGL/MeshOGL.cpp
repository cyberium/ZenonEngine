#include <stdafx.h>

// Include
#include "BufferOGL.h"
#include "ShaderOGL.h"
#include "PipelineStateOGL.h"

// General
#include "MeshOGL.h"

MeshOGL::MeshOGL()
	: m_pIndexBuffer(nullptr)
	, m_PrimitiveTopology(GL_TRIANGLES)
	, m_pMaterial(nullptr)
	, m_bIsDirty(false)
{
	glGenVertexArrays(1, &m_GLObj);
}

MeshOGL::~MeshOGL()
{
	if (m_GLObj != 0)
	{
		glDeleteVertexArrays(1, &m_GLObj);
		m_GLObj = 0;
	}
}

void MeshOGL::AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer)
{
	m_VertexBuffers[binding] = buffer;
	m_bIsDirty = true;
}

void MeshOGL::SetIndexBuffer(std::shared_ptr<IBuffer> buffer)
{
	m_pIndexBuffer = buffer;
	m_bIsDirty = true;
}

void MeshOGL::SetPrimitiveTopology(PrimitiveTopology _topology)
{
	switch (_topology)
	{
	case PrimitiveTopology::PointList:
		m_PrimitiveTopology = GL_POINTS;
		break;
	case PrimitiveTopology::LineList:
		m_PrimitiveTopology = GL_LINES;
		break;
	case PrimitiveTopology::LineStrip:
		m_PrimitiveTopology = GL_LINE_STRIP;
		break;
	case PrimitiveTopology::TriangleList:
		m_PrimitiveTopology = GL_TRIANGLES;
		break;
	case PrimitiveTopology::TriangleStrip:
		m_PrimitiveTopology = GL_TRIANGLE_STRIP;
		break;
	default:
		assert1(false);
	}
}

void MeshOGL::SetMaterial(std::shared_ptr<const Material> material)
{
	m_pMaterial = material;
}

std::shared_ptr<const Material> MeshOGL::GetMaterial() const
{
	return m_pMaterial;
}

bool MeshOGL::Render(RenderEventArgs& renderArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation, UINT indexCnt, UINT vertexStartLocation, UINT vertexCnt)
{
	if (m_pMaterial)
	{
		m_pMaterial->Bind();

		std::shared_ptr<ShaderOGL> pVS = std::dynamic_pointer_cast<ShaderOGL>(m_pMaterial->GetShader(Shader::VertexShader));

		if (pVS)
			pVS->GetShaderParameterByName("PerObject").Set(perObject);

		if (m_bIsDirty)
		{
			Commit(pVS);
			//m_bIsDirty = false;
		}
	}

	glBindVertexArray(m_GLObj);
	{
		if (m_pIndexBuffer != NULL)
		{
			UINT vertexCount = (*m_VertexBuffers.begin()).second->GetElementCount();
			glDrawRangeElements
			(
				m_PrimitiveTopology,
				0,
				vertexCount,
				m_pIndexBuffer->GetElementCount(),
				GL_UNSIGNED_SHORT,
				(char*)0
			);
		}
		else
		{
			if (vertexCnt == 0)
			{
				UINT vertexCount = (*m_VertexBuffers.begin()).second->GetElementCount();
				glDrawArrays
				(
					m_PrimitiveTopology,
					0,
					vertexCount
				);
			}
			else
			{
				glDrawArrays
				(
					m_PrimitiveTopology,
					vertexStartLocation,
					vertexCnt
				);
			}
		}
	}
	glBindVertexArray(0);

	return true;
}

bool MeshOGL::Accept(IVisitor& visitor)
{
	return visitor.Visit(*this);
}

void MeshOGL::Commit(std::weak_ptr<ShaderOGL> _shader)
{
	std::shared_ptr<ShaderOGL> pVS = _shader.lock();

	glBindVertexArray(m_GLObj);
	{
		uint32 newVertexAttribMask = 0;
		uint32 i = 0;
		for (BufferMap::value_type buffer : m_VertexBuffers)
		{
			if (!pVS->HasSemantic(buffer.first))
				continue;

			const InputSemantic& semantic = pVS->GetSemantic(buffer.first);
			UINT             semanticSlot = pVS->GetSemanticSlot(buffer.first);

			// Bind the vertex buffer to a particular slot ID.
			buffer.second->Bind(semanticSlot, pVS, ShaderParameter::Type::Buffer);

			// (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
			glVertexAttribPointer(semanticSlot, semantic.GLSize, semantic.GLType, false, buffer.second->GetElementStride(), (char*)0);
			newVertexAttribMask |= (1 << (i++));
		}

		for (uint32 i = 0; i < 16; ++i)
		{
			uint32 curBit = 1 << i;
			if (newVertexAttribMask & curBit)
				glEnableVertexAttribArray(i);
			else
				glDisableVertexAttribArray(i);
		}

		if (m_pIndexBuffer != NULL)
		{
			m_pIndexBuffer->Bind(0, pVS, ShaderParameter::Type::Buffer);
		}


		for (BufferMap::value_type buffer : m_VertexBuffers)
		{
			if (!pVS->HasSemantic(buffer.first))
				continue;

			UINT semanticSlot = pVS->GetSemanticSlot(buffer.first);

			buffer.second->UnBind(semanticSlot, pVS, ShaderParameter::Type::Buffer);
		}
	}
	glBindVertexArray(0);
}
