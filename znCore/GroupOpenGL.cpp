#include "stdafx.h"

// General
#include "GroupOpenGL.h"

void CGroupOpenGL::InitDefault()
{
	EXT_texture_filter_anisotropic = false;
	EXT_texture_compression_s3tc = false;
	EXT_texture_sRGB = false;

	OES_EGL_image = false;

	majorVersion = 4;
	minorVersion = 4;
}
