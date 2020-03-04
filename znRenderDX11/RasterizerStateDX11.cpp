#include "stdafx.h"

// General
#include "RasterizerStateDX11.h"

// FORWARD BEGIN
D3D11_FILL_MODE TranslateFillMode(IRasterizerState::FillMode fillMode);
D3D11_CULL_MODE TranslateCullMode(IRasterizerState::CullMode cullMode);
bool TranslateFrontFace(IRasterizerState::FrontFace frontFace);
std::vector<D3D11_RECT> TranslateRects(const std::vector<Rect>& rects);
std::vector<D3D11_VIEWPORT> TranslateViewports(const std::vector<const Viewport *>& viewports);
// FORWARD END

RasterizerStateDX11::RasterizerStateDX11(IRenderDeviceDX11& RenderDeviceDX11)
	: m_RenderDeviceDX11(RenderDeviceDX11)
{

}

RasterizerStateDX11::RasterizerStateDX11(const RasterizerStateDX11& copy)
	: m_RenderDeviceDX11(copy.m_RenderDeviceDX11)
	, m_d3dRects(copy.m_d3dRects)
	, m_d3dViewports(copy.m_d3dViewports)
{
    m_d3dRects = copy.m_d3dRects;
    m_d3dViewports = copy.m_d3dViewports;

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

RasterizerStateDX11::~RasterizerStateDX11()
{

}

const RasterizerStateDX11& RasterizerStateDX11::operator=(const RasterizerStateDX11& other)
{
	// avoid assignment to self.
	if (this != &other)
	{
		m_d3dRects = other.m_d3dRects;
		m_d3dViewports = other.m_d3dViewports;

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

		m_RenderDeviceDX11 = other.m_RenderDeviceDX11;
	}

	return *this;
}


//
// RasterizerState
//
void RasterizerStateDX11::Bind()
{
    if (m_StateDirty)
    {
        D3D11_RASTERIZER_DESC1 rasterizerDesc = { };
        rasterizerDesc.FillMode = TranslateFillMode(m_FrontFaceFillMode);
        rasterizerDesc.CullMode = TranslateCullMode(m_CullMode);
        rasterizerDesc.FrontCounterClockwise = TranslateFrontFace(m_FrontFace);
        rasterizerDesc.DepthBias = (m_DepthBias < 0.0f) ? static_cast<INT>(m_DepthBias - 0.5f) : static_cast<INT>(m_DepthBias + 0.5f);
        rasterizerDesc.DepthBiasClamp = m_BiasClamp;
        rasterizerDesc.SlopeScaledDepthBias = m_SlopeBias;
        rasterizerDesc.DepthClipEnable = m_DepthClipEnabled;
        rasterizerDesc.ScissorEnable = m_ScissorEnabled;
        rasterizerDesc.MultisampleEnable = m_MultisampleEnabled;
        rasterizerDesc.AntialiasedLineEnable = m_AntialiasedLineEnabled;
        rasterizerDesc.ForcedSampleCount = m_ForcedSampleCount;

        m_pRasterizerState = NULL;
        if (FAILED(m_RenderDeviceDX11.GetDeviceD3D11()->CreateRasterizerState1(&rasterizerDesc, &m_pRasterizerState)))
            throw CException("Failed to create rasterizer state.");

        m_StateDirty = false;
    }

    if (m_ScissorRectsDirty)
    {
        m_d3dRects = TranslateRects(m_ScissorRects);
        m_ScissorRectsDirty = false;
    }

    if (m_ViewportsDirty)
    {
        m_d3dViewports = TranslateViewports(m_Viewports);
        m_ViewportsDirty = false;
    }

    m_RenderDeviceDX11.GetDeviceContextD3D11()->RSSetViewports((UINT)m_d3dViewports.size(), m_d3dViewports.data());
    m_RenderDeviceDX11.GetDeviceContextD3D11()->RSSetScissorRects((UINT)m_d3dRects.size(), m_d3dRects.data());
    m_RenderDeviceDX11.GetDeviceContextD3D11()->RSSetState(m_pRasterizerState);
}

void RasterizerStateDX11::Unbind()
{
	m_RenderDeviceDX11.GetDeviceContextD3D11()->RSSetViewports(0, NULL);
	m_RenderDeviceDX11.GetDeviceContextD3D11()->RSSetScissorRects(0, NULL);
	m_RenderDeviceDX11.GetDeviceContextD3D11()->RSSetState(NULL);
}



//
// Translate
//
D3D11_FILL_MODE TranslateFillMode(IRasterizerState::FillMode fillMode)
{
	switch (fillMode)
	{
		case IRasterizerState::FillMode::Wireframe:
			return D3D11_FILL_WIREFRAME;
		case IRasterizerState::FillMode::Solid:
			return D3D11_FILL_SOLID;
		default:
			throw std::exception("Unknown fill mode.");
	}
	return D3D11_FILL_SOLID;
}

D3D11_CULL_MODE TranslateCullMode(IRasterizerState::CullMode cullMode)
{
	switch (cullMode)
	{
		case IRasterizerState::CullMode::None:
			return D3D11_CULL_NONE;
		case IRasterizerState::CullMode::Front:
			return D3D11_CULL_FRONT;
		case IRasterizerState::CullMode::Back:
			return D3D11_CULL_BACK;
		default:
			throw std::exception("Unknown cull mode.");
	}
	return D3D11_CULL_NONE;
}

bool TranslateFrontFace(IRasterizerState::FrontFace frontFace)
{
	switch (frontFace)
	{
		case IRasterizerState::FrontFace::Clockwise:
			return false;
		case IRasterizerState::FrontFace::CounterClockwise:
			return true;
		default:
			throw std::exception("Unknown front face winding order.");
	}
	return true;
}

std::vector<D3D11_RECT> TranslateRects(const std::vector<Rect>& rects)
{
	std::vector<D3D11_RECT> result;
	for (uint32 i = 0; i < rects.size(); i++)
	{
		const Rect& rect = rects[i];

		D3D11_RECT d3dRect;
		d3dRect.top = static_cast<LONG>(rect.Y + 0.5f);
		d3dRect.bottom = static_cast<LONG>(rect.Y + rect.Height + 0.5f);
		d3dRect.left = static_cast<LONG>(rect.X + 0.5f);
		d3dRect.right = static_cast<LONG>(rect.X + rect.Width + 0.5f);
		result.push_back(d3dRect);
	}

	return result;
}

std::vector<D3D11_VIEWPORT> TranslateViewports(const std::vector<const Viewport *>& viewports)
{
	std::vector<D3D11_VIEWPORT> result;
	for (uint32 i = 0; i < viewports.size(); i++)
	{
		const Viewport* viewport = viewports[i];
		if (viewport == nullptr)
			break;

		// I could probably do a reinterpret cast here...
		D3D11_VIEWPORT d3dViewport;
		d3dViewport.TopLeftX = viewport->GetX();
		d3dViewport.TopLeftY = viewport->GetY();
		d3dViewport.Width = viewport->GetWidth();
		d3dViewport.Height = viewport->GetHeight();
		d3dViewport.MinDepth = viewport->GetMinDepth();
		d3dViewport.MaxDepth = viewport->GetMaxDepth();

		result.push_back(d3dViewport);
	}

	return result;
}
