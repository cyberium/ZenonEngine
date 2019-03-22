#include <stdafx.h>

// Include
#include "RenderDeviceOGL.h"
#include "TextureOGL.h"
#include "StructuredBufferOGL.h"

// General
#include "RenderTargetOGL.h"

GLenum TranslateAttachmentPoint(IRenderTarget::AttachmentPoint _attach)
{
	switch (_attach)
	{
	case IRenderTarget::AttachmentPoint::Color0:
		return GL_COLOR_ATTACHMENT0;

	case IRenderTarget::AttachmentPoint::Color1:
		return GL_COLOR_ATTACHMENT1;

	case IRenderTarget::AttachmentPoint::Color2:
		return GL_COLOR_ATTACHMENT2;

	case IRenderTarget::AttachmentPoint::Color3:
		return GL_COLOR_ATTACHMENT3;

	case IRenderTarget::AttachmentPoint::Color4:
		return GL_COLOR_ATTACHMENT4;

	case IRenderTarget::AttachmentPoint::Color5:
		return GL_COLOR_ATTACHMENT5;

	case IRenderTarget::AttachmentPoint::Color6:
		return GL_COLOR_ATTACHMENT6;

	case IRenderTarget::AttachmentPoint::Color7:
		return GL_COLOR_ATTACHMENT7;

	case IRenderTarget::AttachmentPoint::Depth:
		return GL_DEPTH_ATTACHMENT;

	case IRenderTarget::AttachmentPoint::DepthStencil:
		return GL_DEPTH_ATTACHMENT | GL_STENCIL_ATTACHMENT;
	}

	std::exception("Invalid AttachmentPoint.");
}

RenderTargetOGL::RenderTargetOGL(RenderDeviceOGL* _device)
	: m_RenderDevice(_device)
	, m_Width(0)
	, m_Height(0)
	, m_bCheckValidity(false)
{
	m_Textures.resize((size_t)IRenderTarget::AttachmentPoint::NumAttachmentPoints + 1);
	m_StructuredBuffers.resize(8);

	glGenFramebuffers(1, &m_GLObj);
}

RenderTargetOGL::~RenderTargetOGL()
{
	if (m_GLObj != 0)
	{
		glDeleteFramebuffers(1, &m_GLObj);
	}
	m_GLObj = 0;
}

void RenderTargetOGL::AttachTexture(AttachmentPoint attachment, std::shared_ptr<Texture> texture)
{
	std::shared_ptr<TextureOGL> textureOGL = std::dynamic_pointer_cast<TextureOGL>(texture);
	m_Textures[(uint8_t)attachment] = textureOGL;

	glBindFramebuffer(GL_FRAMEBUFFER, m_GLObj);
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, TranslateAttachmentPoint(attachment), GL_TEXTURE_2D, textureOGL->GetGLObject(), 0);
		OGLCheckError();

		if (attachment >= AttachmentPoint::Color0 && attachment <= AttachmentPoint::Color7)
		{
			GLenum buffers[] = { TranslateAttachmentPoint(attachment) };
			glDrawBuffers(1, buffers);
			OGLCheckError();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, m_RenderDevice->GetDefaultRB());

	m_bCheckValidity = true;
}

std::shared_ptr<Texture> RenderTargetOGL::GetTexture(AttachmentPoint attachment)
{
	return m_Textures[(uint8_t)attachment];
}


void RenderTargetOGL::Clear(AttachmentPoint attachment, ClearFlags clearFlags, cvec4 color, float depth, uint8_t stencil)
{
	GLint currentFB = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFB);
	assert1(currentFB == m_GLObj);

	uint8 clearFlags8 = (uint8)clearFlags;
	
	if (attachment >= AttachmentPoint::Color0 && attachment <= AttachmentPoint::Color7)
	{
		std::shared_ptr<TextureOGL> texture = m_Textures[(uint8)attachment];
		if (texture == nullptr)
		{
			clearFlags8 &= ~((uint8)ClearFlags::Color);
			return;
		}

		// Store state of glDrawBuffers
		GLint prevBuffers[8] = { 0 };
		for (uint32 i = 0; i <= (uint8_t)AttachmentPoint::Color7; ++i)
		{
			glGetIntegerv(GL_DRAW_BUFFER0 + i, &prevBuffers[i]);
		}

		if (clearFlags8 & (uint8)ClearFlags::Color)
		{
			GLenum buffers[] = { TranslateAttachmentPoint(attachment) };
			glDrawBuffers(1, buffers);

			glClearColor(color.r, color.g, color.b, color.a);
		}

		glClear(GL_COLOR_BUFFER_BIT);

		// Restore state of glDrawBuffers
		glDrawBuffers(8, (const GLenum *)prevBuffers);
	}
	else if (attachment == AttachmentPoint::Depth)
	{
		if (clearFlags8 & (uint8)ClearFlags::Depth)
		{
			glClearDepth(depth);
		}

		glClear(GL_DEPTH_BUFFER_BIT);
	}
	else if (attachment == AttachmentPoint::DepthStencil)
	{
		GLbitfield oglClearMask = 0;
		if (clearFlags8 & (uint8)ClearFlags::Depth)
		{
			oglClearMask |= GL_DEPTH_BUFFER_BIT;
			glClearDepth(depth);
		}
		if (clearFlags8 & (uint8)ClearFlags::Stencil)
		{
			oglClearMask |= GL_STENCIL_BUFFER_BIT;
			glClearStencil(stencil);
		}

		glClear(oglClearMask);
	}
}

void RenderTargetOGL::Clear(ClearFlags clearFlags, cvec4 color, float depth, uint8_t stencil)
{
	for (uint8_t i = 0; i < (uint8_t)AttachmentPoint::NumAttachmentPoints; ++i)
	{
		Clear((AttachmentPoint)i, clearFlags, color, depth, stencil);
	}
}

void RenderTargetOGL::GenerateMipMaps()
{
	for (auto texture : m_Textures)
	{
		if (texture)
		{
			texture->GenerateMipMaps();
		}
	}
}

void RenderTargetOGL::AttachStructuredBuffer(uint8_t slot, std::shared_ptr<StructuredBuffer> rwBuffer)
{
	std::shared_ptr<StructuredBufferOGL> rwbufferOGL = std::dynamic_pointer_cast<StructuredBufferOGL>(rwBuffer);
	m_StructuredBuffers[slot] = rwbufferOGL;

	// Next time the render target is "bound", check that it is valid.
	m_bCheckValidity = true;
}

std::shared_ptr<StructuredBuffer> RenderTargetOGL::GetStructuredBuffer(uint8_t slot)
{
	if (slot < m_StructuredBuffers.size())
	{
		return m_StructuredBuffers[slot];
	}

	return std::shared_ptr<StructuredBuffer>();
}


void RenderTargetOGL::Resize(uint16_t width, uint16_t height)
{
	if (m_Width != width || m_Height != height)
	{
		m_Width = glm::max<uint16_t>(width, 1);
		m_Height = glm::max<uint16_t>(height, 1);
		// Resize the attached textures.
		for (auto texture : m_Textures)
		{
			if (texture)
			{
				texture->Resize(m_Width, m_Height);
			}
		}
	}
}

void RenderTargetOGL::Bind()
{
	if (m_bCheckValidity)
	{
		if (!IsValid())
		{
			Log::Error("Invalid render target.");
		}
		m_bCheckValidity = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_GLObj);
	assert1(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	
	GLenum buffers[8] = { };
	GLsizei buffersCnt = 0;
	for (uint8_t i = 0; i < 8; i++)
	{
		std::shared_ptr<TextureOGL> texture = m_Textures[i];
		if (texture)
		{
			buffers[i] = TranslateAttachmentPoint((AttachmentPoint)i);
			buffersCnt++;
		}
	}
	glDrawBuffers(buffersCnt, buffers);
}

void RenderTargetOGL::UnBind()
{
	assert1(m_RenderDevice->GetDefaultRB() != m_GLObj);
	assert1(m_RenderDevice->GetDefaultRB() == 0);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GLObj);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RenderDevice->GetDefaultRB());
	glBlitFramebuffer(
		0, 0, m_Width, m_Height,
		0, 0, m_Width, m_Height,
		GL_COLOR_BUFFER_BIT, GL_NEAREST
	);

	glDrawBuffer(m_RenderDevice->IsDoubleBuffered() ? GL_BACK_LEFT : GL_FRONT_LEFT);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

bool RenderTargetOGL::IsValid() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_GLObj);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	switch (status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		return true;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		Log::Error("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		Log::Error("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		Log::Error("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		Log::Error("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		Log::Error("GL_FRAMEBUFFER_UNSUPPORTED");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		Log::Error("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
		break;
	case GL_FRAMEBUFFER_UNDEFINED:
		Log::Error("GL_FRAMEBUFFER_UNDEFINED");
		break;
	default:
		Log::Error("GL_FRAMEBUFFER_UNKNOWN");
		break;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_RenderDevice->GetDefaultRB());

	return false;
}


