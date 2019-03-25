#include "FreeImage.h"
#include "Utilities.h"

// ----------------------------------------------------------
//   Definitions for the DDS format
// ----------------------------------------------------------

#ifdef _WIN32
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif

// ----------------------------------------------------------
//   Structures used by DXT textures
// ----------------------------------------------------------

typedef struct tagColor8888 {
	BYTE b;
	BYTE g;
	BYTE r;
	BYTE a;
} Color8888;

typedef struct tagColor565 {
	WORD b : 5;
	WORD g : 6;
	WORD r : 5;
} Color565;

typedef struct tagDXTColBlock {
	Color565 colors[2];
	BYTE row[4];
} DXTColBlock;

typedef struct tagDXTAlphaBlockExplicit {
	WORD row[4];
} DXTAlphaBlockExplicit;

typedef struct tagDXTAlphaBlock3BitLinear {
	BYTE alpha[2];
	BYTE data[6];
} DXTAlphaBlock3BitLinear;

typedef struct tagDXT1Block
{
	DXTColBlock color;
} DXT1Block;

typedef struct tagDXT3Block {		// also used by dxt2
	DXTAlphaBlockExplicit alpha;
	DXTColBlock color;
} DXT3Block;

typedef struct tagDXT5Block {		// also used by dxt4
	DXTAlphaBlock3BitLinear alpha;
	DXTColBlock color;
} DXT5Block;

// ----------------------------------------------------------
//   Structures used by BPL textures
// ----------------------------------------------------------

#define LIBBLP_MIPMAP_COUNT 16
#define LIBBLP_PALETTE_SIZE 256

enum BLPColorEncoding : uint8_t {
	COLOR_JPEG = 0, // not supported
	COLOR_PALETTE = 1,
	COLOR_DXT = 2,
	COLOR_ARGB8888 = 3,
	COLOR_ARGB8888_dup = 4,    // same decompression, likely other PIXEL_FORMAT
};

enum BLPPixelFormat : uint8_t {
	PIXEL_DXT1 = 0,
	PIXEL_DXT3 = 1,
	PIXEL_ARGB8888 = 2,
	PIXEL_ARGB1555 = 3,
	PIXEL_ARGB4444 = 4,
	PIXEL_RGB565 = 5,
	PIXEL_DXT5 = 7,
	PIXEL_UNSPECIFIED = 8,
	PIXEL_ARGB2565 = 9,
	NUM_PIXEL_FORMATS = 10,
};

/* define errors. */
#define LIBBLP_SUCCESS				     0		/* success. */
#define LIBBLP_ERROR_OPEN				-1		/* open error on file. */
#define LIBBLP_ERROR_CLOSE				-2		/* close error on file. */
#define LIBBLP_ERROR_SEEK				-3		/* lseek error on file. */
#define LIBBLP_ERROR_MALLOC				-6		/* memory allocation error. */
#define LIBBLP_ERROR_FORMAT				-7		/* format errror. */
#define LIBBLP_ERROR_SIZE				-9		/* buffer size is to small. */

struct BLPHeader
{
	uint8_t    magic[4];
	uint32_t     type;

	BLPColorEncoding colorEncoding;
	uint8_t    alphaChannelBitDepth;  // Alpha channel bit depth: 0, 1, 4 or 8
	BLPPixelFormat   pixelFormat;   // 0, 1, 7, or 8
	uint8_t    has_mips;     // 0 = no mips, 1 = has mips

	uint32_t     width;
	uint32_t     height;
	uint32_t     mipOffsets[LIBBLP_MIPMAP_COUNT];
	uint32_t     mipSizes[LIBBLP_MIPMAP_COUNT];
	uint32_t     pallete[LIBBLP_PALETTE_SIZE];
};

#ifdef _WIN32
#	pragma pack(pop)
#else
#	pragma pack()
#endif

// ==========================================================

// Get the 4 possible colors for a block
//
static void 
GetBlockColors (const DXTColBlock &block, Color8888 colors[4], bool isDXT1) {
	int i;
	// expand from 565 to 888
	for (i = 0; i < 2; i++)	{
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
	if (wCol[0] > wCol[1] || !isDXT1) {
		// 4 color block
		for (i = 0; i < 2; i++)	{
			colors[i + 2].a = 0xff;
			colors[i + 2].r = (BYTE)((WORD (colors[0].r) * (2 - i) + WORD (colors[1].r) * (1 + i)) / 3);
			colors[i + 2].g = (BYTE)((WORD (colors[0].g) * (2 - i) + WORD (colors[1].g) * (1 + i)) / 3);
			colors[i + 2].b = (BYTE)((WORD (colors[0].b) * (2 - i) + WORD (colors[1].b) * (1 + i)) / 3);
		}
	}
	else {
		// 3 color block, number 4 is transparent
		colors[2].a = 0xff;
		colors[2].r = (BYTE)((WORD (colors[0].r) + WORD (colors[1].r)) / 2);
		colors[2].g = (BYTE)((WORD (colors[0].g) + WORD (colors[1].g)) / 2);
		colors[2].b = (BYTE)((WORD (colors[0].b) + WORD (colors[1].b)) / 2);

		colors[3].a = 0x00;
		colors[3].g = 0x00;
		colors[3].b = 0x00;
		colors[3].r = 0x00;
	}
}

struct DXT_INFO_1 {
	typedef DXT1Block Block;
	enum {
		isDXT1 = 1,
		bytesPerBlock = 8
	};
};

struct DXT_INFO_3 {
	typedef DXT3Block Block;
	enum {
		isDXT1 = 1,
		bytesPerBlock = 16
	};
};

struct DXT_INFO_5 {
	typedef DXT5Block Block;
	enum
	{
		isDXT1 = 1,
		bytesPerBlock = 16
	};
};

template <class INFO> class DXT_BLOCKDECODER_BASE {
protected:
	Color8888 m_colors[4];
	const typename INFO::Block *m_pBlock;
	unsigned m_colorRow;

public:
	void Setup (const BYTE *pBlock) {
		m_pBlock = (const typename INFO::Block *)pBlock;
		GetBlockColors (m_pBlock->color, m_colors, INFO::isDXT1);
	}

	void SetY (int y) {
		m_colorRow = m_pBlock->color.row[y];
	}

	void GetColor (int x, int y, Color8888 &color) {
		unsigned bits = (m_colorRow >> (x * 2)) & 3;
		color = m_colors[bits];
	}
};

class DXT_BLOCKDECODER_1 : public DXT_BLOCKDECODER_BASE <DXT_INFO_1> {
public:
	typedef DXT_INFO_1 INFO;
};

class DXT_BLOCKDECODER_3 : public DXT_BLOCKDECODER_BASE <DXT_INFO_3> {
public:
	typedef DXT_BLOCKDECODER_BASE <DXT_INFO_3> base;
	typedef DXT_INFO_3 INFO;

protected:
	unsigned m_alphaRow;

public:
	void SetY (int y) {
		base::SetY (y);
		m_alphaRow = m_pBlock->alpha.row[y];
	}

	void GetColor (int x, int y, Color8888 &color) {
		base::GetColor (x, y, color);
		const unsigned bits = (m_alphaRow >> (x * 4)) & 0xF;
		color.a = (BYTE)((bits * 0xFF) / 0xF);
	}
};

class DXT_BLOCKDECODER_5 : public DXT_BLOCKDECODER_BASE <DXT_INFO_5> {
public:
	typedef DXT_BLOCKDECODER_BASE <DXT_INFO_5> base;
	typedef DXT_INFO_5 INFO;

protected:
	unsigned m_alphas[8];
	unsigned m_alphaBits;
	int m_offset;

public:
	void Setup (const BYTE *pBlock) {
		base::Setup (pBlock);

		const DXTAlphaBlock3BitLinear &block = m_pBlock->alpha;
		m_alphas[0] = block.alpha[0];
		m_alphas[1] = block.alpha[1];
		if (m_alphas[0] > m_alphas[1]) {
			// 8 alpha block
			for (int i = 0; i < 6; i++) {
				m_alphas[i + 2] = ((6 - i) * m_alphas[0] + (1 + i) * m_alphas[1] + 3) / 7;
			}
		}
		else {
			// 6 alpha block
			for (int i = 0; i < 4; i++) {
				m_alphas[i + 2] = ((4 - i) * m_alphas[0] + (1 + i) * m_alphas[1] + 2) / 5;
			}
			m_alphas[6] = 0;
			m_alphas[7] = 0xFF;
		}

	}

	void SetY (int y) {
		base::SetY (y);
		int i = y / 2;
		const DXTAlphaBlock3BitLinear &block = m_pBlock->alpha;
		m_alphaBits = unsigned(block.data[0 + i * 3]) | (unsigned(block.data[1 + i * 3]) << 8)
			| (unsigned(block.data[2 + i * 3]) << 16);
		m_offset = (y & 1) * 12;
	}

	void GetColor (int x, int y, Color8888 &color) {
		base::GetColor (x, y, color);
		unsigned bits = (m_alphaBits >> (x * 3 + m_offset)) & 7;
		color.a = (BYTE)m_alphas[bits];
	}
};

template <class DECODER> void DecodeDXTBlock (BYTE *dstData, const BYTE *srcBlock, long dstPitch, int bw, int bh) {
	DECODER decoder;
	decoder.Setup (srcBlock);
	for (int y = 0; y < bh; y++) {
		BYTE *dst = dstData - y * dstPitch;
		decoder.SetY (y);
		for (int x = 0; x < bw; x++) {
			decoder.GetColor (x, y, (Color8888 &)*dst);

#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB 
			INPLACESWAP(dst[FI_RGBA_RED], dst[FI_RGBA_BLUE]);
#endif 
			dst += 4;
		}
	}
}

// ==========================================================
// Plugin Interface
// ==========================================================

static int s_format_id;

// ==========================================================
// Internal functions
// ==========================================================

template <class DECODER> static void 
LoadDXT_Helper (FreeImageIO *io, fi_handle handle, int /*page*/, int /*flags*/, void * /*data*/, FIBITMAP *dib, int width, int height, int line) {
	typedef typename DECODER::INFO INFO;
	typedef typename INFO::Block Block;

	Block *input_buffer = new(std::nothrow) Block[(width + 3) / 4];
	if(!input_buffer) return;

	int widthRest = (int) width & 3;
	int heightRest = (int) height & 3;
	int inputLine = (width + 3) / 4;
	int y = 0;

	if (height >= 4) {
		for (; y < height; y += 4) {
			io->read_proc (input_buffer, sizeof (typename INFO::Block), inputLine, handle);
			// TODO: probably need some endian work here
			BYTE *pbSrc = (BYTE *)input_buffer;
			BYTE *pbDst = FreeImage_GetScanLine (dib, height - y - 1);

			if (width >= 4) {
				for (int x = 0; x < width; x += 4) {
					DecodeDXTBlock <DECODER> (pbDst, pbSrc,	line, 4, 4);
					pbSrc += INFO::bytesPerBlock;
					pbDst += 4 * 4;
				}
			}
			if (widthRest) {
				DecodeDXTBlock <DECODER> (pbDst, pbSrc, line, widthRest, 4);
			}
		}
	}
	if (heightRest)	{
		io->read_proc (input_buffer, sizeof (typename INFO::Block), inputLine, handle);
		// TODO: probably need some endian work here
		BYTE *pbSrc = (BYTE *)input_buffer;
		BYTE *pbDst = FreeImage_GetScanLine (dib, height - y - 1);

		if (width >= 4) {
			for (int x = 0; x < width; x += 4) {
				DecodeDXTBlock <DECODER> (pbDst, pbSrc,	line, 4, heightRest);
				pbSrc += INFO::bytesPerBlock;
				pbDst += 4 * 4;
			}
		}
		if (widthRest) {
			DecodeDXTBlock <DECODER> (pbDst, pbSrc,	line, widthRest, heightRest);
		}

	}

	delete [] input_buffer;
}

FIBITMAP * LoadBPL(const BLPHeader& header, FreeImageIO *io, fi_handle handle, int page, int flags, void *data)
{
	if (header.width & (header.width - 1))
	{
		//view->IsTexture3D = true;
		// 3D texture
	}
	if (header.height & (header.height - 1))
	{
		_ASSERT(false);
		return NULL;
	}

	FIBITMAP *dib = FreeImage_Allocate(header.width, header.height, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);
	if (dib == NULL)
	{
		_ASSERT(false);
		return NULL;
	}

	BYTE* dibUINT8 = FreeImage_GetBits(dib);

	bool    hasAlpha = (header.alphaChannelBitDepth != 0);
	uint8_t mipmax   = 1; //(header.has_mips ? LIBBLP_MIPMAP_COUNT : 1);

	for (uint8_t currentMip = 0; currentMip < mipmax; currentMip++)
	{
		if ((header.mipOffsets[currentMip] == 0) || (header.mipSizes[currentMip] == 0))
			break;

		uint16_t mipWidth = std::max(header.width >> currentMip, 1u);
		uint16_t mipHeight = std::max(header.height >> currentMip, 1u);

		switch (header.colorEncoding)
		{
		case BLPColorEncoding::COLOR_PALETTE:
		{
			// Data in mipmaps in indices info pallete
			uint8_t* indexInPalleteBuffer = new uint8_t[header.mipSizes[currentMip]];
			io->seek_proc(handle, header.mipOffsets[currentMip], SEEK_SET);
			io->read_proc(indexInPalleteBuffer, 1, header.mipSizes[currentMip], handle);

			//view->MipData[currentMip] = new uint8_t[header.width * header.height * 4];
			uint32_t resultBufferCntr = 0;

			uint8_t* indexInPalleteColor = indexInPalleteBuffer;
			int alphaBitCntr = 0;
			uint8_t* indexInPalleteAlpha = &indexInPalleteBuffer[0] + mipWidth * mipHeight;

			for (uint32_t y = 0; y < mipWidth; y++)
			{
				for (uint32_t x = 0; x < mipHeight; x++)
				{
					// Read color
					uint32_t color = header.pallete[*indexInPalleteColor++];
					//color = ((color & 0x00FF0000) >> 16) | ((color & 0x0000FF00)) | ((color & 0x000000FF) << 16);

					// Read alpha
					uint8_t alpha;
					switch (header.alphaChannelBitDepth)
					{
					case 0:
						alpha = 0xff;
						break;
					case 1:
						alpha = (*indexInPalleteAlpha & (1 << alphaBitCntr++)) ? 0xff : 0x00;
						if (alphaBitCntr == 8)
						{
							alphaBitCntr = 0;
							indexInPalleteAlpha++;
						}
						break;
					case 8:
						alpha = (*indexInPalleteAlpha++);
						break;
					default:
						_ASSERT(false); //LIBBLP_ERROR_FORMAT
						return NULL;
					}
					
					dibUINT8[resultBufferCntr++] = ((color & 0x000000FF));
					dibUINT8[resultBufferCntr++] = ((color & 0x0000FF00) >> 8);
					dibUINT8[resultBufferCntr++] = ((color & 0x00FF0000) >> 16);
					dibUINT8[resultBufferCntr++] = ((alpha & 0x000000FF));
				}
			}

			delete[] indexInPalleteBuffer;
		}
		break;

		case BLPColorEncoding::COLOR_DXT:
		{
			io->seek_proc(handle, header.mipOffsets[currentMip], SEEK_SET);

			int bpp  = FreeImage_GetBPP(dib);
			int line = CalculateLine(mipWidth, bpp);

			switch (header.pixelFormat)
			{
			case BLPPixelFormat::PIXEL_DXT1:
				LoadDXT_Helper <DXT_BLOCKDECODER_1>(io, handle, page, flags, data, dib, mipWidth, mipHeight, line);
				break;
			case BLPPixelFormat::PIXEL_DXT3:
				LoadDXT_Helper <DXT_BLOCKDECODER_3>(io, handle, page, flags, data, dib, mipWidth, mipHeight, line);
				break;
			case BLPPixelFormat::PIXEL_DXT5:
				LoadDXT_Helper <DXT_BLOCKDECODER_5>(io, handle, page, flags, data, dib, mipWidth, mipHeight, line);
				break;
			default:
				_ASSERT(false); //LIBBLP_ERROR_FORMAT
				return NULL;
			}
		}
		break;

		case BLPColorEncoding::COLOR_ARGB8888:
		{
			io->seek_proc(handle, header.mipOffsets[currentMip], SEEK_SET);
			io->read_proc(dibUINT8, 1, header.mipSizes[currentMip], handle);
		}
		break;

		default:
			_ASSERT(false); //LIBBLP_ERROR_FORMAT
			return NULL;
		}
	}

	return dib;
}

// ==========================================================
// Plugin Implementation
// ==========================================================

static const char * DLL_CALLCONV
Format() {
	return "BLP";
}

static const char * DLL_CALLCONV
Description() {
	return "Blizzard picture";
}

static const char * DLL_CALLCONV
Extension() {
	return "blp";
}

static const char * DLL_CALLCONV
RegExpr() {
	return NULL;
}

static const char * DLL_CALLCONV
MimeType() {
	return "image/x-blp";
}

static BOOL DLL_CALLCONV
Validate(FreeImageIO *io, fi_handle handle) {
	BLPHeader header;
	memset(&header, 0, sizeof(BLPHeader));
	io->read_proc(&header, 1, sizeof(header), handle);

	if (header.magic[0] == 'B' && header.magic[1] == 'L' && header.magic[2] == 'P' && header.magic[3] == '2' && header.type == 1)
		return TRUE;

	return FALSE;
}

static BOOL DLL_CALLCONV
SupportsExportDepth(int depth) {
	return FALSE;
}

static BOOL DLL_CALLCONV 
SupportsExportType(FREE_IMAGE_TYPE type) {
	return FALSE;
}

// ----------------------------------------------------------

static void * DLL_CALLCONV
Open(FreeImageIO *io, fi_handle handle, BOOL read) {
	return NULL;
}

static void DLL_CALLCONV
Close(FreeImageIO *io, fi_handle handle, void *data) {
}

// ----------------------------------------------------------

static FIBITMAP * DLL_CALLCONV
Load(FreeImageIO *io, fi_handle handle, int page, int flags, void *data) {
	BLPHeader header;
	memset(&header, 0, sizeof(BLPHeader));
	io->read_proc(&header, 1, sizeof(header), handle);

	return LoadBPL(header, io, handle, page, flags, data);
}

// ==========================================================
//   Init
// ==========================================================

void DLL_CALLCONV
InitBLP(Plugin *plugin, int format_id) {
	s_format_id = format_id;

	plugin->format_proc = Format;
	plugin->description_proc = Description;
	plugin->extension_proc = Extension;
	plugin->regexpr_proc = RegExpr;
	plugin->open_proc = Open;
	plugin->close_proc = Close;
	plugin->pagecount_proc = NULL;
	plugin->pagecapability_proc = NULL;
	plugin->load_proc = Load;
	plugin->save_proc = NULL;
	plugin->validate_proc = Validate;
	plugin->mime_proc = MimeType;
	plugin->supports_export_bpp_proc = SupportsExportDepth;
	plugin->supports_export_type_proc = SupportsExportType;
	plugin->supports_icc_profiles_proc = NULL;
}
