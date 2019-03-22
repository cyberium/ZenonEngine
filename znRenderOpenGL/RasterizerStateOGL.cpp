#include <stdafx.h>

// General
#include "RasterizerStateOGL.h"

GLenum GLTranslateFillMode(RasterizerState::FillMode fillMode)
{
	switch (fillMode)
	{
	case RasterizerState::FillMode::Wireframe:
		return GL_LINE;

	case RasterizerState::FillMode::Solid:
		return GL_FILL;
	}

	std::exception("Unknown fill mode.");
}

GLenum GLTranslateCullMode(RasterizerState::CullMode cullMode)
{
	switch (cullMode)
	{
	case RasterizerState::CullMode::None:
		return GL_NONE;

	case RasterizerState::CullMode::Front:
		return GL_FRONT;

	case RasterizerState::CullMode::Back:
		return GL_BACK;

	case RasterizerState::CullMode::FrontAndBack:
		return GL_FRONT_AND_BACK;
	}

	std::exception("Unknown cull mode.");
}

GLenum GLTranslateFrontFace(RasterizerState::FrontFace frontFace)
{
	switch (frontFace)
	{
	case RasterizerState::FrontFace::Clockwise:
		return GL_CW;

	case RasterizerState::FrontFace::CounterClockwise:
		return GL_CCW;
	}

	std::exception("Unknown front face winding order.");
}

RasterizerStateOGL::RasterizerStateOGL()
	: m_FrontFaceFillMode(FillMode::Solid)
	, m_BackFaceFillMode(FillMode::Solid)
	, m_CullMode(CullMode::Back)
	, m_FrontFace(FrontFace::CounterClockwise)
	, m_DepthBias(0.0f)
	, m_SlopeBias(0.0f)
	, m_BiasClamp(0.0f)
	, m_DepthClipEnabled(true)
	, m_ScissorEnabled(false)
	, m_MultisampleEnabled(false)
	, m_AntialiasedLineEnabled(false)
	, m_ConservativeRasterization(false)
	, m_ForcedSampleCount(0)
	, m_StateDirty(true)
	, m_ViewportsDirty(true)
	, m_ScissorRectsDirty(true)
{
	m_Viewports.resize(8, Viewport());
	m_ScissorRects.resize(8, Rect());
}

RasterizerStateOGL::RasterizerStateOGL(const RasterizerStateOGL& copy)
	: m_FrontFaceFillMode(copy.m_FrontFaceFillMode)
	, m_BackFaceFillMode(copy.m_BackFaceFillMode)
	, m_CullMode(copy.m_CullMode)
	, m_FrontFace(copy.m_FrontFace)
	, m_DepthBias(copy.m_DepthBias)
	, m_SlopeBias(copy.m_SlopeBias)
	, m_BiasClamp(copy.m_BiasClamp)
	, m_DepthClipEnabled(copy.m_DepthClipEnabled)
	, m_ScissorEnabled(copy.m_ScissorEnabled)
	, m_MultisampleEnabled(copy.m_MultisampleEnabled)
	, m_AntialiasedLineEnabled(copy.m_AntialiasedLineEnabled)
	, m_ConservativeRasterization(copy.m_ConservativeRasterization)
	, m_ForcedSampleCount(copy.m_ForcedSampleCount)
	, m_ScissorRects(copy.m_ScissorRects)
	, m_Viewports(copy.m_Viewports)
	, m_StateDirty(true)
	, m_ViewportsDirty(false)
	, m_ScissorRectsDirty(false)
{}

RasterizerStateOGL::~RasterizerStateOGL()
{

}

const RasterizerStateOGL& RasterizerStateOGL::operator=(const RasterizerStateOGL& other)
{
	// avoid assignment to self.
	if (this != &other)
	{
		m_FrontFaceFillMode = other.m_FrontFaceFillMode;
		m_BackFaceFillMode = other.m_BackFaceFillMode;

		m_CullMode = other.m_CullMode;

		m_FrontFace = other.m_FrontFace;

		m_DepthBias = other.m_DepthBias;
		m_SlopeBias = other.m_SlopeBias;
		m_BiasClamp = other.m_BiasClamp;

		m_DepthClipEnabled = other.m_DepthClipEnabled;
		m_ScissorEnabled = other.m_ScissorEnabled;

		m_MultisampleEnabled = other.m_MultisampleEnabled;
		m_AntialiasedLineEnabled = other.m_AntialiasedLineEnabled;

		m_ConservativeRasterization = other.m_ConservativeRasterization;

		m_ForcedSampleCount = other.m_ForcedSampleCount;

		m_ScissorRects = other.m_ScissorRects;
		m_Viewports = other.m_Viewports;

		m_StateDirty = true;
		m_ViewportsDirty = false;
		m_ScissorRectsDirty = false;
	}

	return *this;
}

void RasterizerStateOGL::SetFillMode(FillMode frontFace, FillMode backFace)
{
	m_FrontFaceFillMode = frontFace;
	m_BackFaceFillMode = backFace;
}

void RasterizerStateOGL::GetFillMode(FillMode& frontFace, FillMode& backFace) const
{
	frontFace = m_FrontFaceFillMode;
	backFace = m_BackFaceFillMode;
}

void RasterizerStateOGL::SetCullMode(CullMode cullMode)
{
	m_CullMode = cullMode;
	m_StateDirty = true;
}

RasterizerState::CullMode RasterizerStateOGL::GetCullMode() const
{
	return m_CullMode;
}

void RasterizerStateOGL::SetFrontFacing(FrontFace frontFace)
{
	m_FrontFace = frontFace;
	m_StateDirty = true;
	throw std::exception("Not implemented!");
}

RasterizerState::FrontFace RasterizerStateOGL::GetFrontFacing() const
{
	throw std::exception("Not implemented!");

	return m_FrontFace;
}

void RasterizerStateOGL::SetDepthBias(float depthBias, float slopeBias, float biasClamp)
{
	m_DepthBias = depthBias;
	m_SlopeBias = slopeBias;
	m_BiasClamp = biasClamp;

	m_StateDirty = true;
	throw std::exception("Not implemented!");
}

void RasterizerStateOGL::GetDepthBias(float& depthBias, float& slopeBias, float& biasClamp) const
{
	throw std::exception("Not implemented!");

	depthBias = m_DepthBias;
	slopeBias = m_SlopeBias;
	biasClamp = m_BiasClamp;
}

void RasterizerStateOGL::SetDepthClipEnabled(bool depthClipEnabled)
{
	m_DepthClipEnabled = depthClipEnabled;
	m_StateDirty = true;
	throw std::exception("Not implemented!");
}

bool RasterizerStateOGL::GetDepthClipEnabled() const
{
	throw std::exception("Not implemented!");
	return m_DepthClipEnabled;
}

void RasterizerStateOGL::SetViewport(const Viewport& viewport)
{
	m_Viewports[0] = viewport;
	m_ViewportsDirty = true;
}

void RasterizerStateOGL::SetViewports(const std::vector<Viewport>& viewports)
{
	m_Viewports = viewports;
	m_ViewportsDirty = true;
}

const std::vector<Viewport>& RasterizerStateOGL::GetViewports()
{
	return m_Viewports;
}

void RasterizerStateOGL::SetScissorEnabled(bool scissorEnable)
{
	m_ScissorEnabled = scissorEnable;
	m_StateDirty = true;
}

bool RasterizerStateOGL::GetScissorEnabled() const
{
	return m_ScissorEnabled;
}

void RasterizerStateOGL::SetScissorRect(const Rect& rect)
{
	m_ScissorRects[0] = rect;
	m_ScissorRectsDirty = true;
}

void RasterizerStateOGL::SetScissorRects(const std::vector<Rect>& rects)
{
	m_ScissorRects = rects;
	m_ScissorRectsDirty = true;
}

const std::vector<Rect>& RasterizerStateOGL::GetScissorRects() const
{
	return m_ScissorRects;
}

void RasterizerStateOGL::SetMultisampleEnabled(bool multisampleEnabled)
{
	m_MultisampleEnabled = multisampleEnabled;
	m_StateDirty = true;
}

bool RasterizerStateOGL::GetMultisampleEnabled() const
{
	return m_MultisampleEnabled;
}

void RasterizerStateOGL::SetAntialiasedLineEnable(bool antialiasedLineEnabled)
{
	m_AntialiasedLineEnabled = antialiasedLineEnabled;
	m_StateDirty = true;
}

bool RasterizerStateOGL::GetAntialiasedLineEnable() const
{
	return m_AntialiasedLineEnabled;
}

void RasterizerStateOGL::SetForcedSampleCount(uint8_t sampleCount)
{
	m_ForcedSampleCount = sampleCount;
	m_StateDirty = true;
	throw std::exception("Not implemented!");
}

uint8_t RasterizerStateOGL::GetForcedSampleCount()
{
	throw std::exception("Not implemented!");
	return m_ForcedSampleCount;
}

void RasterizerStateOGL::SetConservativeRasterizationEnabled(bool conservativeRasterizationEnabled)
{
	m_ConservativeRasterization = conservativeRasterizationEnabled;
	throw std::exception("Not implemented!");
}

bool RasterizerStateOGL::GetConservativeRasterizationEnabled() const
{
	throw std::exception("Not implemented!");
	// Currently, this implementation always returns false because conservative rasterization is supported since DirectX 11.3 and 12.
	return false;
}


// Can only be invoked by the pipeline state
void RasterizerStateOGL::Bind()
{
	if (m_StateDirty)
	{
		// Fill mode
		glPolygonMode(GL_FRONT_AND_BACK, GLTranslateFillMode(m_FrontFaceFillMode));

		// Cull mode
		if (m_CullMode != CullMode::None)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GLTranslateCullMode(m_CullMode));
			glFrontFace(GLTranslateFrontFace(m_FrontFace));
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		// Scissor
		if (m_ScissorEnabled)
		{
			glEnable(GL_SCISSOR_TEST);
		}
		else
		{
			glDisable(GL_SCISSOR_TEST);
		}

		m_StateDirty = false;
	}

	if (m_ScissorRectsDirty)
	{
		for (size_t i = 0; i < m_ScissorRects.size(); i++)
		{
			glScissorIndexed(i,
				static_cast<GLint>(m_ScissorRects[i].X + 0.5f),
				static_cast<GLint>(m_ScissorRects[i].Y + 0.5f),
				static_cast<GLint>(m_ScissorRects[i].Width + 0.5f),
				static_cast<GLint>(m_ScissorRects[i].Height + 0.5f)
			);
		}

		m_ScissorRectsDirty = false;
	}

	if (m_ViewportsDirty)
	{
		for (size_t i = 0; i < m_Viewports.size(); i++)
		{
			glViewportIndexedf(i, m_Viewports[i].X, m_Viewports[i].Y, m_Viewports[i].Width, m_Viewports[i].Height);
			glDepthRangeIndexed(i, m_Viewports[i].MinDepth, m_Viewports[i].MaxDepth);
		}

		m_ViewportsDirty = false;
	}
}