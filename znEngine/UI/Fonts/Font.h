#pragma once

// FORWARD BEGIN
class MeshWrapper;
// FORWARD END

class CFontMesh : public MeshWrapper
{
public:
	CFontMesh(std::shared_ptr<Texture> _texture, std::shared_ptr<IMesh> _fontGeometry, std::vector<uint32> _widthArray, uint32 _height);
	~CFontMesh();

	// Getters
	std::shared_ptr<Texture>  GetTexture() const;
	std::shared_ptr<IMesh>    GetMesh() const;
	uint32                    GetCharWidth(char _char) const;
	uint32                    GetStringWidth(cstring _string) const;
	uint32                    GetHeight() const;
	 
public:
	static const uint32 SPACE = 32;
	static const uint32 NUM_CHARS = 256;

private:
	std::shared_ptr<Texture>	m_Texture;
	std::shared_ptr<IMesh>	    m_Geometry;
	std::vector<uint32>	        m_WidthArray;
	uint32			            m_Height;
};

