#include <stdafx.h>

// General
#include "RasterizerStateDX11.h"

// FORWARD BEGIN
D3D11_FILL_MODE TranslateFillMode(RasterizerState::FillMode fillMode);
D3D11_CULL_MODE TranslateCullMode(RasterizerState::CullMode cullMode);
bool TranslateFrontFace(RasterizerState::FrontFace frontFace);
std::vector<D3D11_RECT> TranslateRects(const std::vector<Rect>& rects);
std::vector<D3D11_VIEWPORT> TranslateViewports(const std::vector<const Viewport *>& viewports);
// FORWARD END

RasterizerStateDX11::RasterizerStateDX11(ID3D11Device2* pDevice)
	: m_pDevice(pDevice)
{
	m_pDevice->GetImmediateContext2(&m_pDeviceContext);

	m_Viewports.resize(8, nullptr);
	m_ScissorRects.resize(8, Rect());
}

RasterizerStateDX11::RasterizerStateDX11(const RasterizerStateDX11& copy)
	: m_pDevice(copy.m_pDevice)
	, m_pDeviceContext(copy.m_pDeviceContext)
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
        D3D11_RASTERIZER_DESC1 rasterizerDesc = {};

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
        if (FAILED(m_pDevice->CreateRasterizerState1(&rasterizerDesc, &m_pRasterizerState)))
        {
            Log::Error("Failed to create rasterizer state.");
        }

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

    m_pDeviceContext->RSSetViewports((UINT)m_d3dViewports.size(), m_d3dViewports.data());
    m_pDeviceContext->RSSetScissorRects((UINT)m_d3dRects.size(), m_d3dRects.data());
    m_pDeviceContext->RSSetState(m_pRasterizerState);
}



//
// Translate
//
D3D11_FILL_MODE TranslateFillMode(RasterizerState::FillMode fillMode)
{
	D3D11_FILL_MODE result = D3D11_FILL_SOLID;
	switch (fillMode)
	{
	case RasterizerState::FillMode::Wireframe:
		result = D3D11_FILL_WIREFRAME;
		break;
	case RasterizerState::FillMode::Solid:
		result = D3D11_FILL_SOLID;
		break;
	default:
        throw std::exception("Unknown fill mode.");
	}

	return result;
}

D3D11_CULL_MODE TranslateCullMode(RasterizerState::CullMode cullMode)
{
	D3D11_CULL_MODE result = D3D11_CULL_BACK;
	switch (cullMode)
	{
	case RasterizerState::CullMode::None:
		result = D3D11_CULL_NONE;
		break;
	case RasterizerState::CullMode::Front:
		result = D3D11_CULL_FRONT;
		break;
	case RasterizerState::CullMode::Back:
		result = D3D11_CULL_BACK;
		break;
	case RasterizerState::CullMode::FrontAndBack:
		// This mode is not supported in DX11.
		break;
	default:
        throw std::exception("Unknown cull mode.");
	}

	return result;
}

bool TranslateFrontFace(RasterizerState::FrontFace frontFace)
{
	bool frontCounterClockwise = true;
	switch (frontFace)
	{
	case RasterizerState::FrontFace::Clockwise:
		frontCounterClockwise = false;
		break;
	case RasterizerState::FrontFace::CounterClockwise:
		frontCounterClockwise = true;
		break;
	default:
		throw std::exception("Unknown front face winding order.");
	}

	return frontCounterClockwise;
}

std::vector<D3D11_RECT> TranslateRects(const std::vector<Rect>& rects)
{
	std::vector<D3D11_RECT> result(rects.size());
	for (uint32 i = 0; i < rects.size(); i++)
	{
		D3D11_RECT& d3dRect = result[i];
		const Rect& rect = rects[i];

		d3dRect.top = static_cast<LONG>(rect.Y + 0.5f);
		d3dRect.bottom = static_cast<LONG>(rect.Y + rect.Height + 0.5f);
		d3dRect.left = static_cast<LONG>(rect.X + 0.5f);
		d3dRect.right = static_cast<LONG>(rect.X + rect.Width + 0.5f);
	}

	return result;
}

std::vector<D3D11_VIEWPORT> TranslateViewports(const std::vector<const Viewport *>& viewports)
{
	std::vector<D3D11_VIEWPORT> result;

	for (uint32 i = 0; i < viewports.size(); i++)
	{
		if (viewports[i] == nullptr)
			break;

		D3D11_VIEWPORT d3dViewport;
		const Viewport* viewport = viewports[i];

		// I could probably do a reinterpret cast here...
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
