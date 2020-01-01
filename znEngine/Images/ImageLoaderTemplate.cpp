#include "stdafx.h"

// General
#include "ImageLoaderTemplate.h"

CImageBase::CImageBase()
	: m_Width(0)
	, m_Height(0)
	, m_BitsPerPixel(0)
	, m_Stride(0)
	, m_IsTransperent(false)
	, m_Data(nullptr)
{
}

CImageBase::~CImageBase()
{
	if (m_Data != nullptr)
	{
		delete[] m_Data;
	}
}

uint32 CImageBase::GetWidth() const
{
	return m_Width;
}

uint32 CImageBase::GetHeight() const
{
	return m_Height;
}

uint32 CImageBase::GetBitsPerPixel() const
{
	return m_BitsPerPixel;
}

uint32 CImageBase::GetStride() const
{
	return m_Stride;
}

bool CImageBase::IsTransperent() const
{
	return m_IsTransperent;
}

const uint8* CImageBase::GetData() const
{
	return m_Data;
}

uint8* CImageBase::GetLine(uint32 Line) const
{
	_ASSERT(m_Data != nullptr);
	_ASSERT(Line < m_Height);
	return (uint8*)m_Data + ((m_Height - Line - 1) * m_Stride);
}
