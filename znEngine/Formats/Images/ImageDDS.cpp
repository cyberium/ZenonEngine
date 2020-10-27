#include "stdafx.h"

// General
#include "ImageDDS.h"


using namespace DDSFormat;


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

	std::shared_ptr<CImageDDS> imageDDS = MakeShared(CImageDDS);
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
	m_Data = ZN_NEW uint8[m_Height * m_Stride];

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
	m_Data = ZN_NEW uint8[m_Height * m_Stride];

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

	Block *input_buffer = ZN_NEW Block[(m_Width + 3) / 4];
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