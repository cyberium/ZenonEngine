#include <stdafx.h>

// General
#include "BufferOGL.h"

static const uint32 bufferMappingTypes[3] = { GL_MAP_READ_BIT, GL_MAP_WRITE_BIT, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT };

BufferOGL::BufferOGL(UINT bindFlags, const void* data, size_t count, UINT stride)
	: m_BindFlags(bindFlags)
	, m_uiCount((UINT)count)
	, m_uiStride(stride)
	, m_bIsBound(false)
{
	glGenBuffers(1, &m_GLObj);
	assert1(m_GLObj != 0);
	glBindBuffer(m_BindFlags, m_GLObj);
	glBufferData(m_BindFlags, count * stride, data, /*_isDynamic ? */GL_DYNAMIC_DRAW/* : GL_STATIC_DRAW*/);
	glBindBuffer(m_BindFlags, 0);
}

BufferOGL::~BufferOGL()
{
	if (m_GLObj != 0)
	{
		glDeleteBuffers(1, &m_GLObj);
		m_GLObj = 0;
	}
}

bool BufferOGL::Bind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType)
{
	glBindBuffer(m_BindFlags, m_GLObj);

	return true;
}

void BufferOGL::UnBind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType)
{
	glBindBuffer(m_BindFlags, 0);
}

void BufferOGL::Copy(std::shared_ptr<IBuffer> other)
{
	std::shared_ptr<BufferOGL> srcBuffer = std::dynamic_pointer_cast<BufferOGL>(other);
	assert1(srcBuffer->m_GLObj != 0);

	if (srcBuffer && (srcBuffer.get() != this) && ((m_uiCount * m_uiStride) == (srcBuffer->m_uiCount * srcBuffer->m_uiStride)))
	{
		glBindBuffer(GL_COPY_READ_BUFFER, srcBuffer->m_GLObj);
		{
			GLint size;
			glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);

			glBindBuffer(GL_COPY_WRITE_BUFFER, m_GLObj);
			{
				glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, GL_STATIC_DRAW);
				glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
			}
			glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
		}
		glBindBuffer(GL_COPY_READ_BUFFER, 0);
	}
	else
	{
		std::exception("Source buffer is not compatible with this buffer.");
	}
}

IBuffer::BufferType BufferOGL::GetType() const
{
	switch (m_BindFlags)
	{
	case GL_ARRAY_BUFFER:
		return IBuffer::VertexBuffer;
	case GL_ELEMENT_ARRAY_BUFFER:
		return IBuffer::IndexBuffer;
	case GL_SHADER_STORAGE_BUFFER:
		return IBuffer::StructuredBuffer;
	case GL_UNIFORM_BUFFER:
		return IBuffer::ConstantBuffer;
	default:
		std::exception("Unknown buffer type");
	}

	return IBuffer::Unknown;
}

uint32 BufferOGL::GetElementCount() const
{
	return m_uiCount;
}

uint32 BufferOGL::GetElementStride() const
{
	return m_uiStride;
}
