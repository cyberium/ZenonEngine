#pragma once

#include "ImageLoaderTemplate.h"

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

#include __PACK_END

class CImageDDS
	: public CImageBase
{
public:
	CImageDDS();
	virtual ~CImageDDS();

protected:
	bool LoadImageData(std::shared_ptr<IFile> File);
	bool LoadRGB(const DDSURFACEDESC2& desc, std::shared_ptr<IFile> io);
	bool LoadDXT(int type, const DDSURFACEDESC2& desc, std::shared_ptr<IFile> io);
	bool LoadATI(int type, const DDSURFACEDESC2& desc, std::shared_ptr<IFile> io);

	template <class DECODER>
	bool LoadDXT_Helper(std::shared_ptr<IFile> io);

	template <class DECODER>
	bool LoadATI_Helper(std::shared_ptr<IFile> io);

public: // Static
	static bool IsFileSupported(std::shared_ptr<IFile> File);
	static std::shared_ptr<CImageDDS> CreateImage(std::shared_ptr<IFile> File);
};
