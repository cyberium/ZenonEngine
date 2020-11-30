#include "stdafx.h"

// General
#include "FontsManager.h"

// Additional
#include "Formats/Images/ImageBase.h"

// FreeType
#include <freetype/config/ftheader.h>
#include FT_FREETYPE_H
#pragma comment(lib, "freetype.lib")

FontsManager::FontsManager(IRenderDevice& RenderDevice, IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
	m_MainFont = Add(RenderDevice, "IDB_FONT_CONSOLAS", 14);
}

FontsManager::~FontsManager()
{
}



//
// IFontsManager
//
std::shared_ptr<IznFont> FontsManager::GetMainFont() const
{
	return m_MainFont;
}


struct VertexPT
{
	VertexPT(const glm::vec2& P, const glm::vec2& T)
		: p(P)
		, t(T)
	{}

	glm::vec2 p;
	glm::vec2 t;
};
typedef std::vector<VertexPT> VertexesPT;

std::shared_ptr<IznFont> FontsManager::Add(IRenderDevice& RenderDevice, const std::string& _fontFileName, uint32 _fontSize)
{
	uint32 fontSize = _fontSize;

	std::shared_ptr<IFile> f = m_BaseManager.GetManager<IFilesManager>()->Open(_fontFileName);
	if (f == nullptr)
	{
		Log::Fatal("FontsManager[%s]: Error while loading font.", _fontFileName.c_str());
		return nullptr;
	}

    
    std::vector<uint32> charWidth;
    charWidth.reserve(CFont::NUM_CHARS);
	uint32 charHeight = 0;

	FT_Library ftLibrary;
	FT_Init_FreeType(&ftLibrary);

	FT_Face face;
	if (FT_New_Memory_Face(ftLibrary, f->getData(), f->getSize(), 0, &face) != 0)
	{
		Log::Error("FontsManager[%s]: Error while loading font. Could not load font file.", f->Path_Name().c_str());

		FT_Done_FreeType(ftLibrary);
		return nullptr;
	}

	if (!(face->face_flags & FT_FACE_FLAG_SCALABLE) || !(face->face_flags & FT_FACE_FLAG_HORIZONTAL))
	{
		Log::Error("FontsManager[%s]: Error while loading font. Error setting font size.", f->Path_Name().c_str());

		// Unload
		FT_Done_Face(face);
		FT_Done_FreeType(ftLibrary);
		return nullptr;
	}

	//FT_Set_Char_Size(face, 0, fontSize, 1280, 1024);
	FT_Set_Pixel_Sizes(face, 0, fontSize);

	// Step 2: Find maxAscent/Descent to calculate imageHeight //
	uint32_t imageHeight = fontSize;
	uint32_t imageWidth = 1024;
	int maxDescent = 0;
	int maxAscent = 0;

	uint32_t lineSpace = imageWidth;
	uint32_t lines = 1;

	for (uint32 ch = 0; ch < CFont::NUM_CHARS; ++ch)
	{
		uint32_t charIndex = FT_Get_Char_Index(face, ch);

		// Glyph
		FT_Load_Glyph(face, charIndex, FT_LOAD_RENDER);
		FT_GlyphSlot glyphSlot = face->glyph;

		charWidth.push_back(glyphSlot->advance.x >> 6);

		if (charWidth[ch] > lineSpace)
		{
			lineSpace = imageWidth;
			lines += 1;
		}
		lineSpace -= charWidth[ch];

		maxAscent = std::max(glyphSlot->bitmap_top, maxAscent);
		maxDescent = std::max((int)glyphSlot->bitmap.rows - (int)glyphSlot->bitmap_top, maxDescent);
	}

	charHeight = maxAscent + maxDescent;

	// Get the first power of two in which it will fit
	while (imageHeight < charHeight * lines)
	{
		imageHeight <<= 1;
	}

	// Step 3: Generation of the actual texture //

	uint32* image = ZN_NEW uint32[imageHeight * imageWidth];
	memset(image, 0x00, imageHeight * imageWidth * sizeof(uint32));

	// These are the cameraPosition at which to draw the next glyph
	uint32 x = 0;
	uint32 y = maxAscent;


	VertexesPT vertices;

	for (uint32 ch = 0; ch < CFont::NUM_CHARS; ++ch)
	{
		//Log::Warn("Char [%c] %d", char(ch), ch);
		uint32_t charIndex = FT_Get_Char_Index(face, ch);

		FT_Load_Glyph(face, charIndex, FT_LOAD_DEFAULT);

		FT_GlyphSlot glyphSlot = face->glyph;
		FT_Render_Glyph(glyphSlot, FT_RENDER_MODE_NORMAL);

		if (charWidth[ch] > imageWidth - x)
		{
			x = 0;
			y += charHeight;
		}

		float texX1 = float(x) / float(imageWidth);
		float texX2 = float(x + charWidth[ch]) / float(imageWidth);

		float texY1 = float(y - maxAscent) / float(imageHeight);
		float texY2 = texY1 + float(charHeight) / float(imageHeight);

		vertices.push_back(VertexPT(glm::vec2(0.0f,           0.0f      ), glm::vec2(texX1, texY1)));
		vertices.push_back(VertexPT(glm::vec2(0.0f,           charHeight), glm::vec2(texX1, texY2)));
		vertices.push_back(VertexPT(glm::vec2(charWidth[ch],  0.0f      ), glm::vec2(texX2, texY1)));

		vertices.push_back(VertexPT(glm::vec2(charWidth[ch],  0.0f      ), glm::vec2(texX2, texY1)));
		vertices.push_back(VertexPT(glm::vec2(0.0f,           charHeight), glm::vec2(texX1, texY2)));
		vertices.push_back(VertexPT(glm::vec2(charWidth[ch],  charHeight), glm::vec2(texX2, texY2)));
		
		for (uint32 row = 0; row < glyphSlot->bitmap.rows; ++row)
		{
			for (uint32 pixel = 0; pixel < glyphSlot->bitmap.width; ++pixel)
			{
				uint8* p = (uint8*) &(image[(x + glyphSlot->bitmap_left + pixel) + (y - glyphSlot->bitmap_top + row) * imageWidth]);
				p[0] = (glyphSlot->bitmap.buffer[pixel + row * glyphSlot->bitmap.pitch]);
				p[1] = (glyphSlot->bitmap.buffer[pixel + row * glyphSlot->bitmap.pitch]);
				p[2] = (glyphSlot->bitmap.buffer[pixel + row * glyphSlot->bitmap.pitch]);
				p[3] = (glyphSlot->bitmap.buffer[pixel + row * glyphSlot->bitmap.pitch]);
			}
		}

		x += charWidth[ch];
	}


	std::shared_ptr<IGeometry> __geom = RenderDevice.GetObjectsFactory().CreateGeometry();
	__geom->AddVertexBuffer(BufferBinding("POSITION", 0), RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0,            sizeof(VertexPT)));
    __geom->AddVertexBuffer(BufferBinding("TEXCOORD", 0), RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), sizeof(glm::vec2), sizeof(VertexPT)));
	//__geom->SetVertexBuffer(RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(VertexPT)));

	// Font image
	std::shared_ptr<CImageBase> fontImage = MakeShared(CImageBase, imageWidth, imageHeight, 32, true);
	std::memcpy(fontImage->GetDataEx(), image, imageHeight * imageWidth * 4);
	delete[] image;

	// Font texture
	std::shared_ptr<ITexture> texture = RenderDevice.GetObjectsFactory().CreateEmptyTexture();
	texture->LoadTexture2DFromImage(fontImage);

	FT_Done_Face(face);
	FT_Done_FreeType(ftLibrary);

	//

	std::shared_ptr<IznFont> font = MakeShared(CFont, texture, __geom, charWidth, charHeight);

	Log::Info("FontsManager: Font '%s' loaded. Size [%d].", f->Path_Name().c_str(), fontSize);

	return font;
}
