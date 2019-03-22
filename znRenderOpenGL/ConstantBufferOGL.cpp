#include <stdafx.h>

// General
#include "ConstantBufferOGL.h"

ConstantBufferOGL::ConstantBufferOGL(size_t size)
	: m_BufferSize(size)
{
	glGenBuffers(1, &m_GLObj);
	glBindBuffer(GL_UNIFORM_BUFFER, m_GLObj);
	{
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

ConstantBufferOGL::~ConstantBufferOGL()
{
	glDeleteBuffers(1, &m_GLObj);
}

void ConstantBufferOGL::Set(const void* data, size_t size)
{
	assert(size == m_BufferSize);

	glBindBuffer(GL_UNIFORM_BUFFER, m_GLObj);
	{
		GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		memcpy_s(p, size, data, size);
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ConstantBufferOGL::Copy(std::shared_ptr<ConstantBuffer> other)
{
	std::shared_ptr<ConstantBufferOGL> srcBuffer = std::dynamic_pointer_cast<ConstantBufferOGL>(other);
	assert1(srcBuffer->m_GLObj != 0);

	if (srcBuffer && (srcBuffer.get() != this) &&	(m_BufferSize == srcBuffer->m_BufferSize))
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

void ConstantBufferOGL::Copy(std::shared_ptr<IBuffer> other)
{
	Copy(std::dynamic_pointer_cast<ConstantBuffer>(other));
}

bool ConstantBufferOGL::Bind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, id, m_GLObj);

	return true;
}

void ConstantBufferOGL::UnBind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, id, 0);
}