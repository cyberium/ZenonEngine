#include "stdafx.h"

// Include
#include "BufferOGL.h"
#include "ShaderOGL.h"
#include "PipelineStateOGL.h"

// General
#include "MeshOGL.h"

MeshOGL::MeshOGL()
	: m_PrimitiveTopology(GL_TRIANGLES)
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

void MeshOGL::AddVertexBuffer(const BufferBinding & binding, std::shared_ptr<IBuffer> buffer)
{
    MeshBase::AddVertexBuffer(binding, buffer);
    m_bIsDirty = true;
}

void MeshOGL::SetVertexBuffer(std::shared_ptr<IBuffer> buffer)
{
	MeshBase::SetVertexBuffer(buffer);
    m_bIsDirty = true;
}

void MeshOGL::SetIndexBuffer(std::shared_ptr<IBuffer> buffer)
{
	MeshBase::SetIndexBuffer(buffer);
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
		_ASSERT(false);
	}
}

bool MeshOGL::Render(const RenderEventArgs* renderArgs, const IConstantBuffer* perObject, UINT indexStartLocation, UINT indexCnt, UINT vertexStartLocation, UINT vertexCnt)
{
	if (indexCnt == 0 && m_pIndexBuffer != nullptr)
		indexCnt = m_pIndexBuffer->GetElementCount();

	if (vertexCnt == 0)
	{
		if (m_VertexBuffer != nullptr)
			vertexCnt = m_VertexBuffer->GetElementCount();
		else
			vertexCnt = (*m_VertexBuffers.begin()).second->GetElementCount();
	}

	std::shared_ptr<IShader> pVS = nullptr;
	std::shared_ptr<IShader> pPS = nullptr;
	ShaderMap shadersMap;

	if (m_pMaterial)
	{
		pVS = m_pMaterial->GetShader(IShader::ShaderType::VertexShader);
		pPS = m_pMaterial->GetShader(IShader::ShaderType::PixelShader);
		shadersMap = m_pMaterial->GetShaders();
	}

	if (pVS == nullptr && pPS == nullptr)
	{
		pVS = renderArgs->PipelineState->GetShader(IShader::ShaderType::VertexShader);
		pPS = renderArgs->PipelineState->GetShader(IShader::ShaderType::PixelShader);
		shadersMap = renderArgs->PipelineState->GetShaders();
	}

	if (m_pMaterial)
	{
		m_pMaterial->Bind(shadersMap);
	}

    if (m_bIsDirty)
    {
        Commit(pVS);
        m_bIsDirty = false;
    }

    if (pVS)
    {
        const std::shared_ptr<IShaderParameter>& perObjectParameter = pVS->GetShaderParameterByName("PerObject");
        if (perObjectParameter->IsValid() && perObject != nullptr)
        {
            perObjectParameter->Set(perObject);
            perObjectParameter->Bind();
        }

        if (m_VertexBuffer != nullptr)
        {
            m_VertexBuffer->Bind(0, pVS.get(), IShaderParameter::Type::Buffer);
        }
        else
        {
            for (BufferMap::value_type buffer : m_VertexBuffers)
            {
                BufferBinding binding = buffer.first;
                if (pVS->GetInputLayout()->HasSemantic(binding))
                {
                    UINT slotID = pVS->GetInputLayout()->GetSemanticSlot(binding);
                    buffer.second->Bind(slotID, pVS.get(), IShaderParameter::Type::Buffer);
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
            m_VertexBuffer->UnBind(0, pVS.get(), IShaderParameter::Type::Buffer);
        }
        else
        {
            for (BufferMap::value_type buffer : m_VertexBuffers)
            {
                BufferBinding binding = buffer.first;
                if (pVS->GetInputLayout()->HasSemantic(binding))
                {
                    UINT slotID = pVS->GetInputLayout()->GetSemanticSlot(binding);
                    buffer.second->UnBind(slotID, pVS.get(), IShaderParameter::Type::Buffer);
                }
            }
        }
    }

    if (m_pMaterial)
    {
        //m_pMaterial->Unbind();
    }

    return true;
}

void MeshOGL::Commit(std::weak_ptr<IShader> _shader)
{
	std::shared_ptr<IShader> pVS = _shader.lock();

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
            if (!pVS->GetInputLayout()->HasSemantic(buffer.first))
            {
                throw std::exception("Mesh don't have semantics.");
            }

			const InputSemantic& semantic = pVS->GetInputLayout()->GetSemantic(buffer.first);
			UINT             semanticSlot = pVS->GetInputLayout()->GetSemanticSlot(buffer.first);

			// Bind the vertex buffer to a particular slot ID.
			buffer.second->Bind(semanticSlot, pVS.get(), IShaderParameter::Type::Buffer);

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
			m_pIndexBuffer->Bind(0, pVS.get(), IShaderParameter::Type::Buffer);
		}


		for (BufferMap::value_type buffer : m_VertexBuffers)
		{
			if (!pVS->GetInputLayout()->HasSemantic(buffer.first))
				continue;

			UINT semanticSlot = pVS->GetInputLayout()->GetSemanticSlot(buffer.first);

			buffer.second->UnBind(semanticSlot, pVS.get(), IShaderParameter::Type::Buffer);
		}
	}
	glBindVertexArray(0);
}
