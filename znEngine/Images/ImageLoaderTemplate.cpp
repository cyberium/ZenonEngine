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

#define FI_RGBA_RED				0
#define FI_RGBA_GREEN			1
#define FI_RGBA_BLUE			2
#define FI_RGBA_ALPHA			3

std::shared_ptr<IImage> CImageBase::Convert24To32Bit()
{
	_ASSERT_EXPR(m_BitsPerPixel == 24, L"CImageBase::Convert24To32Bit() works only with 24 bit images!");

	std::shared_ptr<CImageBase> newImage = std::make_shared<CImageBase>();
	newImage->m_Width = m_Width;
	newImage->m_Height = m_Height;
	newImage->m_BitsPerPixel = 32;
	newImage->m_Stride = newImage->GetWidth() * 4;
	newImage->m_IsTransperent = false;
	newImage->m_Data = new uint8[m_Height * newImage->GetStride()];

	for (int rows = 0; rows < m_Height; rows++)
	{
		const uint8* line = GetLine(rows);
		uint8* lineNew = newImage->GetLine(rows);

		for (int cols = 0; cols < m_Width; cols++)
		{
			lineNew[FI_RGBA_RED] = line[FI_RGBA_RED];
			lineNew[FI_RGBA_GREEN] = line[FI_RGBA_GREEN];
			lineNew[FI_RGBA_BLUE] = line[FI_RGBA_BLUE];
			lineNew[FI_RGBA_ALPHA] = 0xFF;
			lineNew += 4;
			line += 3;
		}
	}

	return newImage;
}



//
// IImage
//
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
