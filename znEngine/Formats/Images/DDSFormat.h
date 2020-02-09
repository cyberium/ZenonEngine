#pragma once

namespace DDSFormat
{

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

#include __PACK_BEGIN

	typedef struct tagDDPIXELFORMAT
	{
		DWORD dwSize;	// size of this structure (must be 32)
		DWORD dwFlags;	// see DDPF_*
		DWORD dwFourCC;
		DWORD dwRGBBitCount;	// Total number of bits for RGB formats
		DWORD dwRBitMask;
		DWORD dwGBitMask;
		DWORD dwBBitMask;
		DWORD dwRGBAlphaBitMask;
	} DDPIXELFORMAT;

	// DIRECTDRAW PIXELFORMAT FLAGS
	enum
	{
		DDPF_ALPHAPIXELS = 0x00000001l,	// surface has alpha channel
		DDPF_ALPHA = 0x00000002l,	// alpha only
		DDPF_FOURCC = 0x00000004l,	// FOURCC available
		DDPF_RGB = 0x00000040l	// RGB(A) bitmap
	};

	typedef struct tagDDCAPS2
	{
		DWORD dwCaps1;	// Zero or more of the DDSCAPS_* members
		DWORD dwCaps2;	// Zero or more of the DDSCAPS2_* members
		DWORD dwReserved[2];
	} DDCAPS2;

	// DIRECTDRAWSURFACE CAPABILITY FLAGS
	enum
	{
		DDSCAPS_ALPHA = 0x00000002l, // alpha only surface
		DDSCAPS_COMPLEX = 0x00000008l, // complex surface structure
		DDSCAPS_TEXTURE = 0x00001000l, // used as texture (should always be set)
		DDSCAPS_MIPMAP = 0x00400000l  // Mipmap present
	};

	enum
	{
		DDSCAPS2_CUBEMAP = 0x00000200L,
		DDSCAPS2_CUBEMAP_POSITIVEX = 0x00000400L,
		DDSCAPS2_CUBEMAP_NEGATIVEX = 0x00000800L,
		DDSCAPS2_CUBEMAP_POSITIVEY = 0x00001000L,
		DDSCAPS2_CUBEMAP_NEGATIVEY = 0x00002000L,
		DDSCAPS2_CUBEMAP_POSITIVEZ = 0x00004000L,
		DDSCAPS2_CUBEMAP_NEGATIVEZ = 0x00008000L,
		DDSCAPS2_VOLUME = 0x00200000L
	};

	typedef struct tagDDSURFACEDESC2
	{
		DWORD dwSize;	// size of this structure (must be 124)
		DWORD dwFlags;	// combination of the DDSS_* flags
		DWORD dwHeight;
		DWORD dwWidth;
		DWORD dwPitchOrLinearSize;
		DWORD dwDepth;	// Depth of a volume texture
		DWORD dwMipMapCount;
		DWORD dwReserved1[11];
		DDPIXELFORMAT ddpfPixelFormat;
		DDCAPS2 ddsCaps;
		DWORD dwReserved2;
	} DDSURFACEDESC2;

	enum
	{
		DDSD_CAPS = 0x00000001l,
		DDSD_HEIGHT = 0x00000002l,
		DDSD_WITH = 0x00000004l,
		DDSD_PITCH = 0x00000008l,
		DDSD_ALPHABITDEPTH = 0x00000080l,
		DDSD_PIXELFORMAT = 0x00001000l,
		DDSD_MIPMAPCOUNT = 0x00020000l,
		DDSD_LINEARSIZE = 0x00080000l,
		DDSD_DEPTH = 0x00800000l
	};

	typedef struct tagDDSHEADER
	{
		DWORD dwMagic;			// FOURCC: "DDS "
		DDSURFACEDESC2 surfaceDesc;
	} DDSHEADER;

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


	void ZN_API GetBlockColors(const DXTColBlock &block, Color8888 colors[4], bool isDXT1);


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

	class DXT_BLOCKDECODER_1
		: public DXT_BLOCKDECODER_BASE <DXT1_INFO>
	{
	public:
		typedef DXT1_INFO INFO;
	};

	class DXT_BLOCKDECODER_3
		: public DXT_BLOCKDECODER_BASE <DXT3_INFO>
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

	class DXT_BLOCKDECODER_5
		: public DXT_BLOCKDECODER_BASE <DXT5_INFO>
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

#include __PACK_END

}

#include "DDSFormat.inl"
