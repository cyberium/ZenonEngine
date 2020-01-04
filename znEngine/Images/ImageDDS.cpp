#include "stdafx.h"

// General
#include "ImageDDS.h"

#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
	((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
    ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))

#define FOURCC_DXT1	MAKEFOURCC('D','X','T','1')
#define FOURCC_DXT2	MAKEFOURCC('D','X','T','2')
#define FOURCC_DXT3	MAKEFOURCC('D','X','T','3')
#define FOURCC_DXT4	MAKEFOURCC('D','X','T','4')
#define FOURCC_DXT5	MAKEFOURCC('D','X','T','5')

#define FOURCC_BC4	MAKEFOURCC('A','T','I','1')
#define FOURCC_BC5	MAKEFOURCC('A','T','I','2')

// ----------------------------------------------------------
//   Structures used by DXT textures
// ----------------------------------------------------------

#include __PACK_BEGIN

typedef struct tagColor8888
{
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE a;
} Color8888;

typedef struct tagColor565
{
	WORD b : 5;
	WORD g : 6;
	WORD r : 5;
} Color565;

typedef struct tagDXTColBlock
{
	Color565 colors[2];
	BYTE row[4];
} DXTColBlock;

typedef struct tagDXTAlphaBlockExplicit
{
	WORD row[4];
} DXTAlphaBlockExplicit;

typedef struct tagDXTAlphaBlock3BitLinear
{
	BYTE alpha[2];
	BYTE data[6];
} DXTAlphaBlock3BitLinear;

typedef struct tagDXT1Block
{
	DXTColBlock color;
} DXT1Block;

typedef struct tagDXT3Block
{		// also used by dxt2
	DXTAlphaBlockExplicit alpha;
	DXTColBlock color;
} DXT3Block;

typedef struct tagDXT5Block
{		// also used by dxt4
	DXTAlphaBlock3BitLinear alpha;
	DXTColBlock color;
} DXT5Block;


#include __PACK_END

// ----------------------------------------------------------
//   Internal functions
// ----------------------------------------------------------


// Get the 4 possible colors for a block
//
static void GetBlockColors(const DXTColBlock &block, Color8888 colors[4], bool isDXT1)
{
	// expand from 565 to 888
	for (int i = 0; i < 2; i++)
	{
		colors[i].a = 0xff;
		/*
		colors[i].r = (BYTE)(block.colors[i].r * 0xff / 0x1f);
		colors[i].g = (BYTE)(block.colors[i].g * 0xff / 0x3f);
		colors[i].b = (BYTE)(block.colors[i].b * 0xff / 0x1f);
		*/
		colors[i].r = (BYTE)((block.colors[i].r << 3U) | (block.colors[i].r >> 2U));
		colors[i].g = (BYTE)((block.colors[i].g << 2U) | (block.colors[i].g >> 4U));
		colors[i].b = (BYTE)((block.colors[i].b << 3U) | (block.colors[i].b >> 2U));
	}

	WORD *wCol = (WORD *)block.colors;
	if (wCol[0] > wCol[1] || !isDXT1)
	{
		// 4 color block
		for (int i = 0; i < 2; i++)
		{
			colors[i + 2].a = 0xff;
			colors[i + 2].r = (BYTE)((WORD(colors[0].r) * (2 - i) + WORD(colors[1].r) * (1 + i)) / 3);
			colors[i + 2].g = (BYTE)((WORD(colors[0].g) * (2 - i) + WORD(colors[1].g) * (1 + i)) / 3);
			colors[i + 2].b = (BYTE)((WORD(colors[0].b) * (2 - i) + WORD(colors[1].b) * (1 + i)) / 3);
		}
	}
	else
	{
		// 3 color block, number 4 is transparent
		colors[2].a = 0xff;
		colors[2].r = (BYTE)((WORD(colors[0].r) + WORD(colors[1].r)) / 2);
		colors[2].g = (BYTE)((WORD(colors[0].g) + WORD(colors[1].g)) / 2);
		colors[2].b = (BYTE)((WORD(colors[0].b) + WORD(colors[1].b)) / 2);

		colors[3].a = 0x00;
		colors[3].g = 0x00;
		colors[3].b = 0x00;
		colors[3].r = 0x00;
	}
}

struct DXT1_INFO
{
	typedef DXT1Block Block;
	enum
	{
		isDXT1 = 1,
		bytesPerBlock = 8
	};
};

struct DXT3_INFO
{
	typedef DXT3Block Block;
	enum
	{
		isDXT1 = 1,
		bytesPerBlock = 16
	};
};

struct DXT5_INFO
{
	typedef DXT5Block Block;
	enum
	{
		isDXT1 = 1,
		bytesPerBlock = 16
	};
};




template <class INFO> 
class DXT_BLOCKDECODER_BASE 
{
protected:
	Color8888 m_colors[4];
	const typename INFO::Block *m_pBlock;
	unsigned m_colorRow;

public:
	void Setup(const BYTE *pBlock)
	{
		m_pBlock = (const typename INFO::Block *)pBlock;
		GetBlockColors(m_pBlock->color, m_colors, INFO::isDXT1);
	}

	void SetY(int y)
	{
		m_colorRow = m_pBlock->color.row[y];
	}

	void GetColor(int x, int y, Color8888 &color)
	{
		unsigned bits = (m_colorRow >> (x * 2)) & 3;
		color = m_colors[bits];
	}
};

class DXT_BLOCKDECODER_1 : public DXT_BLOCKDECODER_BASE <DXT1_INFO>
{
public:
	typedef DXT1_INFO INFO;
};

class DXT_BLOCKDECODER_3 : public DXT_BLOCKDECODER_BASE <DXT3_INFO>
{
public:
	typedef DXT_BLOCKDECODER_BASE <DXT3_INFO> base;
	typedef DXT3_INFO INFO;

protected:
	unsigned m_alphaRow;

public:
	void SetY(int y)
	{
		base::SetY(y);
		m_alphaRow = m_pBlock->alpha.row[y];
	}

	void GetColor(int x, int y, Color8888 &color)
	{
		base::GetColor(x, y, color);
		const unsigned bits = (m_alphaRow >> (x * 4)) & 0xF;
		color.a = (BYTE)((bits * 0xFF) / 0xF);
	}
};

class DXT_BLOCKDECODER_5 : public DXT_BLOCKDECODER_BASE <DXT5_INFO>
{
public:
	typedef DXT5_INFO INFO;
	typedef DXT_BLOCKDECODER_BASE <INFO> base;
	

protected:
	unsigned m_alphas[8];
	unsigned m_alphaBits;
	int m_offset;

public:
	void Setup(const BYTE *pBlock)
	{
		base::Setup(pBlock);

		const DXTAlphaBlock3BitLinear &block = m_pBlock->alpha;
		m_alphas[0] = block.alpha[0];
		m_alphas[1] = block.alpha[1];
		if (m_alphas[0] > m_alphas[1])
		{
			// 8 alpha block
			for (int i = 0; i < 6; i++)
			{
				m_alphas[i + 2] = ((6 - i) * m_alphas[0] + (1 + i) * m_alphas[1] + 3) / 7;
			}
		}
		else
		{
			// 6 alpha block
			for (int i = 0; i < 4; i++)
			{
				m_alphas[i + 2] = ((4 - i) * m_alphas[0] + (1 + i) * m_alphas[1] + 2) / 5;
			}
			m_alphas[6] = 0;
			m_alphas[7] = 0xFF;
		}

	}

	void SetY(int y)
	{
		base::SetY(y);
		int i = y / 2;
		const DXTAlphaBlock3BitLinear &block = m_pBlock->alpha;
		m_alphaBits = unsigned(block.data[0 + i * 3]) | (unsigned(block.data[1 + i * 3]) << 8) | (unsigned(block.data[2 + i * 3]) << 16);
		m_offset = (y & 1) * 12;
	}

	void GetColor(int x, int y, Color8888 &color)
	{
		base::GetColor(x, y, color);
		unsigned bits = (m_alphaBits >> (x * 3 + m_offset)) & 7;
		color.a = (BYTE)m_alphas[bits];
	}
};


template <class DECODER>
void DecodeDXTBlock(BYTE *dstData, const BYTE *srcBlock, long dstPitch, int bw, int bh)
{
	DECODER decoder;
	decoder.Setup(srcBlock);
	for (int y = 0; y < bh; y++)
	{
		BYTE *dst = dstData - y * dstPitch;
		decoder.SetY(y);
		for (int x = 0; x < bw; x++)
		{
			decoder.GetColor(x, y, (Color8888 &)*dst);
			dst += 4;
		}
	}
}



//
// CImageDDS
//
CImageDDS::CImageDDS()
{
}

CImageDDS::~CImageDDS()
{
}


//
// Static
//
bool CImageDDS::IsFileSupported(std::shared_ptr<IFile> File)
{
	if (!File->Extension().empty())
	{
		return Utils::ToLower(File->Extension()) == "dds";
	}

	DDSHEADER header = { 0 };
	File->seek(0);
	File->readBytes(&header, sizeof(DDSHEADER));
	File->seek(0);

	if (header.dwMagic != MAKEFOURCC('D', 'D', 'S', ' '))
		return false;

	if (header.surfaceDesc.dwSize != sizeof(header.surfaceDesc) || header.surfaceDesc.ddpfPixelFormat.dwSize != sizeof(header.surfaceDesc.ddpfPixelFormat))
		return false;

	return true;
}

std::shared_ptr<CImageDDS> CImageDDS::CreateImage(std::shared_ptr<IFile> File)
{
	_ASSERT(IsFileSupported(File));

	std::shared_ptr<CImageDDS> imageDDS = std::make_shared<CImageDDS>();
	if (!imageDDS->LoadImageData(File))
	{
		Log::Error("CImageDDS: Unable to load DDS file '%s'.", File->Name().c_str());
		return nullptr;
	}

	return imageDDS;
}



//
// Protected
//
bool CImageDDS::LoadImageData(std::shared_ptr<IFile> File)
{
	DDSHEADER header = { 0 };
	File->seek(0);
	File->readBytes(&header, sizeof(DDSHEADER));

	if (header.surfaceDesc.ddpfPixelFormat.dwFlags & DDPF_RGB)
	{
		return LoadRGB(header.surfaceDesc, File);
	}
	else if (header.surfaceDesc.ddpfPixelFormat.dwFlags & DDPF_FOURCC)
	{
		switch (header.surfaceDesc.ddpfPixelFormat.dwFourCC)
		{
		case FOURCC_DXT1:
			return LoadDXT(1, header.surfaceDesc, File);
		case FOURCC_DXT3:
			return LoadDXT(3, header.surfaceDesc, File);
		case FOURCC_DXT5:
			return LoadDXT(5, header.surfaceDesc, File);
		default:
		{
			char fourcc[5];
			memcpy(fourcc, &(header.surfaceDesc.ddpfPixelFormat.dwFourCC), 4);
			fourcc[4] = '\0';

			Log::Error("CImageDDS: Value '%s' of 'dwFourCC' is inknown.", fourcc);
			return false;
		}
		}
	}

	Log::Error("CImageDDS: Unknown DDS format.");
	return false;
}

bool CImageDDS::LoadRGB(const DDSURFACEDESC2& desc, std::shared_ptr<IFile> io)
{
	m_Width = (uint32)desc.dwWidth;
	m_Height = (uint32)desc.dwHeight;
	m_BitsPerPixel = (uint32)desc.ddpfPixelFormat.dwRGBBitCount;
	m_Stride = m_Width * (m_BitsPerPixel / 8);
	m_IsTransperent = (desc.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS) == 1;
	m_Data = new uint8[m_Height * m_Stride];

	// read the file
	int filePitch = (desc.dwFlags & DDSD_PITCH) ? (int)desc.dwPitchOrLinearSize : m_Stride;
	size_t delta = filePitch - m_Stride;
	for (uint32 i = 0; i < m_Height; i++)
	{
		io->readBytes(GetLine(i), m_Stride);
		io->seekRelative(delta);
	}

	return true;
}

bool CImageDDS::LoadDXT(int type, const DDSURFACEDESC2& desc, std::shared_ptr<IFile> io)
{
	m_Width = (uint32)desc.dwWidth & ~3;
	m_Height = (uint32)desc.dwHeight & ~3;
	m_BitsPerPixel = 32;
	m_Stride = m_Width * (m_BitsPerPixel / 8);
	m_IsTransperent = (desc.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS) == 1;
	m_Data = new uint8[m_Height * m_Stride];

	switch (type)
	{
	case 1:
		return LoadDXT_Helper <DXT_BLOCKDECODER_1>(io);
	case 3:
		return LoadDXT_Helper <DXT_BLOCKDECODER_3>(io);
	case 5:
		return LoadDXT_Helper <DXT_BLOCKDECODER_5>(io);
	default:
		Log::Error("CImageDDS: Unknown DXT type '%d'.", type);
		return false;
	}

	return false;
}

template <class DECODER>
bool CImageDDS::LoadDXT_Helper(std::shared_ptr<IFile> io)
{
	typedef typename DECODER::INFO INFO;
	typedef typename INFO::Block Block;

	Block *input_buffer = new Block[(m_Width + 3) / 4];
	if (!input_buffer)
	{
		Log::Error("CImageDDS: LoadDXT_Helper: Unable to create 'Block' array with size '%d'.", (m_Width + 3) / 4);
		return false;
	}

	int widthRest = (int)m_Width & 3;
	int heightRest = (int)m_Height & 3;
	int inputLine = (m_Width + 3) / 4;
	uint32 y = 0;

	if (m_Height >= 4)
	{
		for (; y < m_Height; y += 4)
		{
			io->readBytes(input_buffer, sizeof(typename INFO::Block) * inputLine);

			// TODO: probably need some endian work here
			BYTE *pbSrc = (BYTE *)input_buffer;
			BYTE *pbDst = GetLine(y);

			if (m_Width >= 4)
			{
				for (uint32 x = 0; x < m_Width; x += 4)
				{
					DecodeDXTBlock <DECODER>(pbDst, pbSrc, m_Stride, 4, 4);
					pbSrc += INFO::bytesPerBlock;
					pbDst += 4 * 4;
				}
			}
			if (widthRest)
			{
				DecodeDXTBlock <DECODER>(pbDst, pbSrc, m_Stride, widthRest, 4);
			}
		}
	}

	if (heightRest)
	{
		io->readBytes(input_buffer, sizeof(typename INFO::Block) * inputLine);
		// TODO: probably need some endian work here
		BYTE *pbSrc = (BYTE *)input_buffer;
		BYTE *pbDst = GetLine(y);

		if (m_Width >= 4)
		{
			for (int x = 0; x < m_Width; x += 4)
			{
				DecodeDXTBlock <DECODER>(pbDst, pbSrc, m_Stride, 4, heightRest);
				pbSrc += INFO::bytesPerBlock;
				pbDst += 4 * 4;
			}
		}
		if (widthRest)
		{
			DecodeDXTBlock <DECODER>(pbDst, pbSrc, m_Stride, widthRest, heightRest);
		}

	}

	delete[] input_buffer;

	for (size_t pixelIndex = 0; pixelIndex < m_Height * m_Width * 4; pixelIndex += 4)
	{
		if (m_Data[pixelIndex + 3] < 0xFF)
		{
			m_IsTransperent = true;
			break;
		}
	}

	return true;
}