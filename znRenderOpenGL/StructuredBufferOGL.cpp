#include <stdafx.h>

// General
#include "StructuredBufferOGL.h"

StructuredBufferOGL::StructuredBufferOGL(UINT bindFlags, const void* data, size_t count, UINT stride, CPUAccess cpuAccess, bool /*bUAV*/)
	: m_BindFlags(bindFlags)
	, m_uiCount((UINT)count)
	, m_uiStride(stride)
	, m_bIsDirty(false)
	, m_CPUAccess(cpuAccess)
{
	m_bDynamic = (m_CPUAccess != CPUAccess::None);

	// Assign the data to the system buffer.
	size_t numBytes = m_uiCount * m_uiStride;

	glGenBuffers(1, &m_GLObj);
	assert1(m_GLObj != 0);

	glBindBuffer(m_BindFlags, m_GLObj);
	{
		if (data)
		{
			m_Data.assign((uint8_t*)data, (uint8_t*)data + numBytes);
			glBufferData(m_BindFlags, numBytes, data, m_bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		}
		else
		{
			m_Data.reserve(numBytes);
			glBufferData(m_BindFlags, numBytes, NULL, m_bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		}
	}
	glBindBuffer(m_BindFlags, 0);
}

StructuredBufferOGL::~StructuredBufferOGL()
{
	if (m_GLObj != 0)
	{
		glDeleteBuffers(1, &m_GLObj);
		m_GLObj = 0;
	}
}

bool StructuredBufferOGL::Bind(uint32 ID, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType)
{
	if (m_bIsDirty)
	{
		Commit();
		m_bIsDirty = false;
	}
	
	glBindBufferBase(m_BindFlags, ID, m_GLObj);
	
	return true;
}

void StructuredBufferOGL::UnBind(uint32 ID, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType)
{
	glBindBufferBase(m_BindFlags, ID, 0);
}

void StructuredBufferOGL::SetData(void* data, size_t elementSize, size_t offset, size_t numElements)
{
	unsigned char* first = (unsigned char*)data + (offset * elementSize);
	unsigned char* last = first + (numElements * elementSize);
	m_Data.assign(first, last);

	m_bIsDirty = true;
}

void StructuredBufferOGL::Commit()
{
	if (m_bIsDirty && m_bDynamic && (m_GLObj != 0))
	{
		glBindBuffer(m_BindFlags, m_GLObj);
		{
			GLvoid* p = glMapBuffer(m_BindFlags, GL_WRITE_ONLY);
			
			size_t sizeInBytes = m_Data.size();
			memcpy_s(p, sizeInBytes, m_Data.data(), sizeInBytes);

			glUnmapBuffer(m_BindFlags);
		}
		glBindBuffer(m_BindFlags, 0);

		m_bIsDirty = false;
	}
}

void StructuredBufferOGL::Copy(std::shared_ptr<StructuredBuffer> other)
{
	std::shared_ptr<StructuredBufferOGL> srcBuffer = std::dynamic_pointer_cast<StructuredBufferOGL>(other);
	assert1(srcBuffer->m_GLObj != 0);

	if (srcBuffer->m_bIsDirty)
	{
		srcBuffer->Commit(); // Make sure the contents of the source buffer are up-to-date
	}

	if (srcBuffer && srcBuffer.get() != this &&	m_uiCount * m_uiStride == srcBuffer->m_uiCount * srcBuffer->m_uiStride
		)
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

void StructuredBufferOGL::Copy(std::shared_ptr<IBuffer> other)
{
	Copy(std::dynamic_pointer_cast<StructuredBuffer>(other));
}

void StructuredBufferOGL::Clear()
{
	/*if (m_pUAV)
	{
		FLOAT clearColor[4] = { 0, 0, 0, 0 };
		m_pDeviceContext->ClearUnorderedAccessViewFloat(m_pUAV, clearColor);
	}*/
}

IBuffer::BufferType StructuredBufferOGL::GetType() const
{
	return IBuffer::StructuredBuffer;
}

uint32 StructuredBufferOGL::GetElementCount() const
{
	return m_uiCount;
}

uint32 StructuredBufferOGL::GetElementStride() const
{
	return m_uiStride;
}
