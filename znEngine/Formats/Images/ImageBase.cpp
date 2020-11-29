#include "stdafx.h"

// General
#include "ImageBase.h"


CImageBase::CImageBase()
	: m_Width(0)
	, m_Height(0)
	, m_BitsPerPixel(0)
	, m_Stride(0)
	, m_IsTransperent(false)
{
}

CImageBase::CImageBase(uint32 Width, uint32 Height, uint32 BitsPerPixel, bool IsTransperent)
	: m_Width(Width)
	, m_Height(Height)
	, m_BitsPerPixel(BitsPerPixel)
	, m_Stride(0)
	, m_IsTransperent(IsTransperent)
{
	m_Stride = GetWidth() * (GetBitsPerPixel() / 8);
	m_Data.resize(m_Height * GetStride());
	//std::memset(m_Data, 0x00, m_Height * GetStride());
}

CImageBase::~CImageBase()
{
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
	return m_Data.data();
}

void CImageBase::Resize(uint32 NewWidth, uint32 NewHeight)
{
	size_t bytesPerPixel = m_BitsPerPixel / 8;

	std::vector<uint8> newData;
	newData.resize(NewWidth * NewHeight * bytesPerPixel);

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
	m_Data.clear();

	m_Width = NewWidth;
	m_Height = NewHeight;
	m_Data.assign(newData.begin(), newData.end());

}

uint8* CImageBase::GetDataEx()
{
	return m_Data.data();
}

#define FI_RGBA_RED				0
#define FI_RGBA_GREEN			1
#define FI_RGBA_BLUE			2
#define FI_RGBA_ALPHA			3

std::shared_ptr<IImage> CImageBase::ConvertAnyTo32Bit()
{
	if (GetBitsPerPixel() == 32)
		return shared_from_this();

	uint32 sourceBytesCnt = GetBitsPerPixel() / 8;

	std::shared_ptr<CImageBase> destImage = MakeShared(CImageBase, m_Width, m_Height, 32, false);

	for (uint32 rows = 0; rows < m_Height; rows++)
	{
		const uint8* sourceLine = GetLine(rows);
		uint8* destLine = destImage->GetLine(rows);

		for (uint32 cols = 0; cols < m_Width; cols++)
		{
			for (uint32 ch = 0; ch < 4; ch++)
			{
				uint32 sourceIndexOffset = std::min(ch, sourceBytesCnt - 1u);
				destLine[FI_RGBA_RED + ch] = sourceLine[FI_RGBA_RED + sourceIndexOffset];
			}

			if (sourceBytesCnt < 4)
				destLine[FI_RGBA_ALPHA] = 0xFF;

			destLine += destImage->GetBitsPerPixel() / 8;
			sourceLine += GetBitsPerPixel() / 8;
		}
	}

	return destImage;
}

std::shared_ptr<IByteBuffer> CImageBase::SaveToMemory()
{
	throw CException("Not implemented.");
}



//
// Protected
//
uint8* CImageBase::GetLine(uint32 Line) const
{
	if (m_Data.empty())
		throw CException("Data is empty.");

	if (Line >= m_Height)
		throw CException("Line '%d' is bigger then height '%d'.", Line, m_Height);

	return (uint8*)m_Data.data() + (Line * m_Stride);
}
