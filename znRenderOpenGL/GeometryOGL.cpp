#include "stdafx.h"

// General
#include "GeometryOGL.h"


GeometryOGL::GeometryOGL()
	: m_PrimitiveTopology(GL_TRIANGLES)
	, m_bIsDirty(false)
{
	glGenVertexArrays(1, &m_GLObj);
}

GeometryOGL::~GeometryOGL()
{
	if (m_GLObj != 0)
	{
		glDeleteVertexArrays(1, &m_GLObj);
		m_GLObj = 0;
	}
}

void GeometryOGL::AddVertexBuffer(const BufferBinding & binding, std::shared_ptr<IBuffer> buffer)
{
	GeometryBase::AddVertexBuffer(binding, buffer);
	m_bIsDirty = true;
}

void GeometryOGL::SetVertexBuffer(std::shared_ptr<IBuffer> buffer)
{
	GeometryBase::SetVertexBuffer(buffer);
	m_bIsDirty = true;
}

void GeometryOGL::SetIndexBuffer(std::shared_ptr<IBuffer> buffer)
{
	GeometryBase::SetIndexBuffer(buffer);
	m_bIsDirty = true;
}

void GeometryOGL::SetPrimitiveTopology(PrimitiveTopology _topology)
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
		_ASSERT(false);
	}
}

bool GeometryOGL::Render(const RenderEventArgs * renderArgs, const IConstantBuffer * PerObject, const std::unordered_map<EShaderType, std::shared_ptr<IShader>>& ShadersMap, const IMaterial * Material, const SGeometryPartParams & GeometryPartParams) const
{
	UINT indexStartLocation = GeometryPartParams.IndexStartLocation;
	UINT indexCnt = GeometryPartParams.IndexCnt;
	if (indexCnt == UINT_MAX && m_pIndexBuffer != nullptr)
		indexCnt = m_pIndexBuffer->GetElementCount();

	UINT vertexStartLocation = GeometryPartParams.VertexStartLocation;
	UINT vertexCnt = GeometryPartParams.VertexCnt;
	if (vertexCnt == UINT_MAX)
	{
		if (m_VertexBuffer != nullptr)
		{
			vertexCnt = m_VertexBuffer->GetElementCount();
		}
		else if (!m_VertexBuffers.empty())
		{
			vertexCnt = (*m_VertexBuffers.begin()).second->GetElementCount();
		}
		else
		{
			_ASSERT(false);
		}
	}

	const IShader* vertexShader = ShadersMap.at(EShaderType::VertexShader).get();
	_ASSERT(vertexShader != nullptr);

	const auto& geomShaderIt = ShadersMap.find(EShaderType::GeometryShader);
	const IShader* geomShader = nullptr;
	if (geomShaderIt != ShadersMap.end())
	{
		geomShader = geomShaderIt->second.get();
	}

	IShaderParameter* perObjectParameter = vertexShader->GetShaderParameterByName("PerObject").get();
	if (perObjectParameter->IsValid() && PerObject != nullptr)
	{
		perObjectParameter->SetConstantBuffer(PerObject);
		perObjectParameter->Bind();
	}

	if (geomShader)
	{
		const std::shared_ptr<IShaderParameter>& perObjectParameterGS = geomShader->GetShaderParameterByName("PerObject");
		if (perObjectParameterGS->IsValid() && PerObject != nullptr)
		{
			perObjectParameterGS->SetConstantBuffer(PerObject);
			perObjectParameterGS->Bind();
		}
	}

	if (m_bIsDirty)
	{
		Commit(vertexShader);
		m_bIsDirty = false;
	}

	if (vertexShader)
	{
		const std::shared_ptr<IShaderParameter>& perObjectParameter = vertexShader->GetShaderParameterByName("PerObject");
		if (perObjectParameter->IsValid() && PerObject != nullptr)
		{
			perObjectParameter->Set(PerObject);
			perObjectParameter->Bind();
		}

		if (m_VertexBuffer != nullptr)
		{
			m_VertexBuffer->Bind(0, vertexShader, IShaderParameter::Type::Buffer);
		}
		else
		{
			for (BufferMap::value_type buffer : m_VertexBuffers)
			{
				BufferBinding binding = buffer.first;
				if (vertexShader->GetInputLayout()->HasSemantic(binding))
				{
					UINT slotID = vertexShader->GetInputLayout()->GetSemanticSlot(binding);
					buffer.second->Bind(slotID, vertexShader, IShaderParameter::Type::Buffer);
				}
			}
		}

		glBindVertexArray(m_GLObj);
		{
			if (m_pIndexBuffer != NULL)
			{
				indexStartLocation *= 2u;

				glDrawRangeElements
				(
					m_PrimitiveTopology,
					vertexStartLocation,
					vertexStartLocation + vertexCnt,
					indexCnt,
					GL_UNSIGNED_SHORT,
					(char*)0 + indexStartLocation
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

			OGLCheckError();
		}
		glBindVertexArray(0);


		if (m_VertexBuffer != nullptr)
		{
			m_VertexBuffer->UnBind(0, vertexShader, IShaderParameter::Type::Buffer);
		}
		else
		{
			for (BufferMap::value_type buffer : m_VertexBuffers)
			{
				BufferBinding binding = buffer.first;
				if (vertexShader->GetInputLayout()->HasSemantic(binding))
				{
					UINT slotID = vertexShader->GetInputLayout()->GetSemanticSlot(binding);
					buffer.second->UnBind(slotID, vertexShader, IShaderParameter::Type::Buffer);
				}
			}
		}
	}

	return true;
}

bool GeometryOGL::RenderInstanced(const RenderEventArgs * renderArgs, const IStructuredBuffer * InstancesBuffer, const std::unordered_map<EShaderType, std::shared_ptr<IShader>>& ShadersMap, const IMaterial * Material, SGeometryPartParams GeometryPartParams) const
{
	return false;
}

void GeometryOGL::Commit(const IShader* _shader) const
{
	glBindVertexArray(m_GLObj);
	{
		for (uint32 i = 0; i < 16; ++i)
		{
			glDisableVertexAttribArray(i);
		}

		uint32 newVertexAttribMask = 0;
		uint32 i = 0;
		for (BufferMap::value_type buffer : m_VertexBuffers)
		{
			if (!_shader->GetInputLayout()->HasSemantic(buffer.first))
			{
				throw std::exception("Mesh don't have semantics.");
			}

			const InputSemantic& semantic = _shader->GetInputLayout()->GetSemantic(buffer.first);
			UINT             semanticSlot = _shader->GetInputLayout()->GetSemanticSlot(buffer.first);

			// Bind the vertex buffer to a particular slot ID.
			buffer.second->Bind(semanticSlot, _shader, IShaderParameter::Type::Buffer);

			// (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
			glVertexAttribPointer(semanticSlot, semantic.GLSize, semantic.GLType, false, buffer.second->GetElementStride(), (char*)0 + buffer.second->GetElementOffset());
			newVertexAttribMask |= (1 << (i++));
		}

		for (uint32 i = 0; i < 16; ++i)
		{
			uint32 curBit = 1 << i;
			if (newVertexAttribMask & curBit)
				glEnableVertexAttribArray(i);
		}

		if (m_pIndexBuffer != nullptr)
		{
			m_pIndexBuffer->Bind(0, _shader, IShaderParameter::Type::Buffer);
		}


		for (BufferMap::value_type buffer : m_VertexBuffers)
		{
			if (!_shader->GetInputLayout()->HasSemantic(buffer.first))
				continue;

			UINT semanticSlot = _shader->GetInputLayout()->GetSemanticSlot(buffer.first);

			buffer.second->UnBind(semanticSlot, _shader, IShaderParameter::Type::Buffer);
		}
	}
	glBindVertexArray(0);
}
