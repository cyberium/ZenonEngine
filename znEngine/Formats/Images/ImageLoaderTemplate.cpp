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

CImageBase::CImageBase(uint32 Width, uint32 Height, uint32 BitsPerPixel, bool IsTransperent)
	: m_Width(Width)
	, m_Height(Height)
	, m_BitsPerPixel(BitsPerPixel)
	, m_Stride(0)
	, m_IsTransperent(IsTransperent)
	, m_Data(nullptr)
{
	m_Stride = GetWidth() * (GetBitsPerPixel() / 8);
	m_Data = DEBUG_NEW uint8[m_Height * GetStride()];	
	std::memset(m_Data, 0x00, m_Height * GetStride());
}

CImageBase::~CImageBase()
{
	if (m_Data != nullptr)
	{
		delete[] m_Data;
	}
}

uint8* CImageBase::GetDataEx()
{
	return m_Data;
}

#define FI_RGBA_RED				0
#define FI_RGBA_GREEN			1
#define FI_RGBA_BLUE			2
#define FI_RGBA_ALPHA			3

std::shared_ptr<IImage> CImageBase::Convert8To32Bit()
{
	_ASSERT_EXPR(m_BitsPerPixel == 8, L"CImageBase::Convert8To32Bit() works only with 8 bit images!");

	std::shared_ptr<CImageBase> newImage = MakeShared(CImageBase);
	newImage->m_Width = m_Width;
	newImage->m_Height = m_Height;
	newImage->m_BitsPerPixel = 32;
	newImage->m_Stride = newImage->GetWidth() * (newImage->m_BitsPerPixel / 8);
	newImage->m_IsTransperent = false;
	newImage->m_Data = DEBUG_NEW uint8[m_Height * newImage->GetStride()];

	for (int rows = 0; rows < m_Height; rows++)
	{
		const uint8* line = GetLine(rows);
		uint8* lineNew = newImage->GetLine(rows);

		for (int cols = 0; cols < m_Width; cols++)
		{
			lineNew[FI_RGBA_RED] = line[FI_RGBA_RED];
			lineNew[FI_RGBA_GREEN] = line[FI_RGBA_RED];
			lineNew[FI_RGBA_BLUE] = line[FI_RGBA_RED];
			lineNew[FI_RGBA_ALPHA] = 0xFF;
			lineNew += 4;
			line += 1;
		}
	}

	return newImage;
}

std::shared_ptr<IImage> CImageBase::Convert24To32Bit()
{
	_ASSERT_EXPR(m_BitsPerPixel == 24, L"CImageBase::Convert24To32Bit() works only with 24 bit images!");

	std::shared_ptr<CImageBase> newImage = MakeShared(CImageBase);
	newImage->m_Width = m_Width;
	newImage->m_Height = m_Height;
	newImage->m_BitsPerPixel = 32;
	newImage->m_Stride = newImage->GetWidth() * 4;
	newImage->m_IsTransperent = false;
	newImage->m_Data = DEBUG_NEW uint8[m_Height * newImage->GetStride()];

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

void CImageBase::Resize(uint32 NewWidth, uint32 NewHeight)
{
	size_t bytesPerPixel = m_BitsPerPixel / 8;
	uint8* newData = DEBUG_NEW uint8[NewWidth * NewHeight * bytesPerPixel];

	size_t cntr = 0;
	for (uint32 j = 0; j < NewHeight; ++j)
	{
		for (uint32 i = 0; i < NewWidth; ++i)
		{
			float gx = static_cast<float>(i) / float(NewWidth) * m_Width;
			float gy = static_cast<float>(j) / float(NewHeight) * m_Height;
			int gxi = static_cast<int>(gx);
			int gyi = static_cast<int>(gy);

			int col = gxi + 1;
			if (col == m_Width)
				col = gxi;

			int row = gyi + 1;
			if (row == m_Height)
				row = gyi;

			for (size_t c = 0; c < bytesPerPixel; c++)
			{
				float c00 = m_Data[(gyi * m_Width + gxi) * bytesPerPixel + c] / 255.0f;
				float c10 = m_Data[(gyi * m_Width + col) * bytesPerPixel + c] / 255.0f;
				float c01 = m_Data[(row * m_Width + gxi) * bytesPerPixel + c] / 255.0f;
				float c11 = m_Data[(row * m_Width + col) * bytesPerPixel + c] / 255.0f;

				float tx = gx - gxi;
				float ty = gy - gyi;
				float a = glm::mix(c00, c10, tx);  //c00 * (1.0f - tx) + c10 * tx;
				float b = glm::mix(c01, c11, tx);  //c01 * (1.0f - tx) + c11 * tx;
				float result = glm::mix(a, b, ty);  //a * (1.0f - ty) + b * ty;

				newData[cntr * bytesPerPixel + c] = static_cast<uint8>(glm::round(result * 255.0f));
			}
			cntr++;
		}
	}

	// Delete old data
	if (m_Data != nullptr)
		delete[] m_Data;

	m_Width = NewWidth;
	m_Height = NewHeight;
	m_Data = newData;

}

uint8* CImageBase::GetLine(uint32 Line) const
{
	_ASSERT(m_Data != nullptr);
	_ASSERT(Line < m_Height);
	return (uint8*)m_Data + ((m_Height - Line - 1) * m_Stride);
}
