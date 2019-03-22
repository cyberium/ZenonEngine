#pragma once

class
	__declspec(uuid("509D7645-8B0C-461D-9A4C-869EB875B9C9"))
	CGroupOpenGL : public ISettingGroup
{
public:
	bool EXT_texture_filter_anisotropic;
	bool EXT_texture_compression_s3tc;
	bool EXT_texture_sRGB;

	bool OES_EGL_image;

	int	majorVersion;
	int	minorVersion;

	// ISettingGroup
	void InitDefault() override;
};