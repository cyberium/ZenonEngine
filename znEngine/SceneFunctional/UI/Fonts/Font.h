#pragma once

class ZN_API CFontMesh : public MeshProxie
{
public:
	CFontMesh(ITexture* _texture, IMesh* _fontGeometry, std::vector<uint32> _widthArray, uint32 _height);
	virtual ~CFontMesh();

	// Getters
	ITexture*  GetTexture() const;
	IMesh*    GetMesh() const;
	uint32                    GetCharWidth(char _char) const;
	uint32                    GetStringWidth(const std::string& _string) const;
	uint32                    GetHeight() const;
	 
public:
	static const uint32 SPACE = 32;
	static const uint32 NUM_CHARS = 256;

private:
	ITexture*	m_Texture;
	IMesh*	    m_Geometry;
	std::vector<uint32>	        m_WidthArray;
	uint32			            m_Height;
};

