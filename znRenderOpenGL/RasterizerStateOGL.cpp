#include <stdafx.h>

// General
#include "RasterizerStateOGL.h"

// FORWARD BEGIN
GLenum GLTranslateFillMode(IRasterizerState::FillMode fillMode);
GLenum GLTranslateCullMode(IRasterizerState::CullMode cullMode);
GLenum GLTranslateFrontFace(IRasterizerState::FrontFace frontFace);
// FORWARD END

RasterizerStateOGL::RasterizerStateOGL()
{
	m_Viewports.resize(8, nullptr);
	m_ScissorRects.resize(8, Rect());
}

RasterizerStateOGL::RasterizerStateOGL(const RasterizerStateOGL& copy)
{
    m_FrontFaceFillMode = copy.m_FrontFaceFillMode;
    m_BackFaceFillMode = copy.m_BackFaceFillMode;

    m_CullMode = copy.m_CullMode;

    m_FrontFace = copy.m_FrontFace;

    m_DepthBias = copy.m_DepthBias;
    m_SlopeBias = copy.m_SlopeBias;
    m_BiasClamp = copy.m_BiasClamp;

    m_DepthClipEnabled = copy.m_DepthClipEnabled;
    m_ScissorEnabled = copy.m_ScissorEnabled;

    m_MultisampleEnabled = copy.m_MultisampleEnabled;
    m_AntialiasedLineEnabled = copy.m_AntialiasedLineEnabled;

    m_ConservativeRasterization = copy.m_ConservativeRasterization;

    m_ForcedSampleCount = copy.m_ForcedSampleCount;

    m_ScissorRects = copy.m_ScissorRects;
    m_Viewports = copy.m_Viewports;

    m_StateDirty = true;
    m_ViewportsDirty = false;
    m_ScissorRectsDirty = false;
}

RasterizerStateOGL::~RasterizerStateOGL()
{

}

const RasterizerStateOGL& RasterizerStateOGL::operator=(const RasterizerStateOGL& other)
{
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



//
// RasterizerState
//
void RasterizerStateOGL::Bind()
{
	//if (m_StateDirty)
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

	//if (m_ScissorRectsDirty)
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

	//if (m_ViewportsDirty)
	{
		for (size_t i = 0; i < m_Viewports.size(); i++)
		{
			glViewportIndexedf(i, m_Viewports[i]->GetX(), m_Viewports[i]->GetY(), m_Viewports[i]->GetWidth(), m_Viewports[i]->GetHeight());
			glDepthRangeIndexed(i, m_Viewports[i]->GetMinDepth(), m_Viewports[i]->GetMaxDepth());
		}

		m_ViewportsDirty = false;
	}
}



//
// Translate
//
GLenum GLTranslateFillMode(IRasterizerState::FillMode fillMode)
{
    switch (fillMode)
    {
        case IRasterizerState::FillMode::Wireframe:
            return GL_LINE;

        case IRasterizerState::FillMode::Solid:
            return GL_FILL;
    }

    std::exception("Unknown fill mode.");
}

GLenum GLTranslateCullMode(IRasterizerState::CullMode cullMode)
{
    switch (cullMode)
    {
        case IRasterizerState::CullMode::None:
            return GL_NONE;

        case IRasterizerState::CullMode::Front:
            return GL_FRONT;

        case IRasterizerState::CullMode::Back:
            return GL_BACK;

        case IRasterizerState::CullMode::FrontAndBack:
            return GL_FRONT_AND_BACK;
    }

    std::exception("Unknown cull mode.");
}

GLenum GLTranslateFrontFace(IRasterizerState::FrontFace frontFace)
{
    switch (frontFace)
    {
        case IRasterizerState::FrontFace::Clockwise:
            return GL_CW;

        case IRasterizerState::FrontFace::CounterClockwise:
            return GL_CCW;
    }

    std::exception("Unknown front face winding order.");
}