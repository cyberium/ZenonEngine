#include <stdafx.h>

// General
#include "FontsManager.h"

// Additional
#include "Application.h"

// Additional
#include <freetype/config/ftheader.h>
#include FT_FREETYPE_H

FontsManager::FontsManager()
{
	m_MainFont = Add("Fonts\\consola.TTF", 14);
}

FontsManager::~FontsManager()
{
}



//
// IFontsManager
//
std::shared_ptr<CFontMesh> FontsManager::GetMainFont() const
{
	return m_MainFont;
}

std::shared_ptr<CFontMesh> FontsManager::Add(cstring _fontFileName, uint32 _fontSize)
{
	return CRefManager1Dim::Add(_fontFileName + "__" + std::to_string(_fontSize));
}

//

std::shared_ptr<CFontMesh> FontsManager::CreateAction(cstring _nameAndSize)
{
	uint32_t _delimIndex = static_cast<uint32>(_nameAndSize.find_last_of("__"));
	if (_delimIndex == -1)
	{
		Log::Error("FontsManager[%s]: Incorrect font nameAndSize.", _nameAndSize.c_str());
		return nullptr;
	}

	std::string fontFileName = _nameAndSize.substr(0, _delimIndex - 1);
	uint32 fontSize = Utils::ToType<uint32>(_nameAndSize.substr(_delimIndex + 1));

	std::shared_ptr<IFile> f = GetManager<IFilesManager>()->Open(fontFileName);
	if (f == nullptr)
	{
		Log::Fatal("FontsManager[%s]: Error while loading font.", f->Path_Name().c_str());
		return nullptr;
	}

    
    std::vector<uint32> charWidth;
    charWidth.reserve(CFontMesh::NUM_CHARS);
	uint32 charHeight = 0;

	FT_Library ftLibrary;
	FT_Init_FreeType(&ftLibrary);

	FT_Face face;
	if (FT_New_Memory_Face(ftLibrary, f->getData(), f->getSize(), 0, &face) != 0)
	{
		Log::Error("FontsManager[%s]: Error while loading font. Could not load font file.", f->Path_Name().c_str());

		// Unload
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
	uint32_t imageWidth = 512;
	int maxDescent = 0;
	int maxAscent = 0;

	uint32_t lineSpace = imageWidth;
	uint32_t lines = 1;

	for (uint32 ch = 0; ch < CFontMesh::NUM_CHARS; ++ch)
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

	uint32* image = new uint32[imageHeight * imageWidth];
	memset(image, 0x00, imageHeight * imageWidth * sizeof(uint32));

	// These are the cameraPosition at which to draw the next glyph
	uint32 x = 0;
	uint32 y = maxAscent;


	DirectX::VertexCollection vertices;
	DirectX::IndexCollection indices;

	for (uint32 ch = 0; ch < CFontMesh::NUM_CHARS; ++ch)
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

		vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(charWidth[ch],  charHeight,  0.0f),  DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(texX2, texY2)));
		vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(0.0f,           charHeight,  0.0f),  DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(texX1, texY2)));
		vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(0.0f,           0.0f,        0.0f),  DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(texX1, texY1)));
		
		vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(0.0f,           0.0f,        0.0f),  DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(texX1, texY1)));
		vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(charWidth[ch],  charHeight,  0.0f),  DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(texX2, texY2)));
		vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(charWidth[ch],  0.0f,        0.0f),  DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(texX2, texY1)));
		
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


	std::shared_ptr<IMesh> __geom = _RenderDevice->CreateMesh();

	__geom->AddVertexBuffer(BufferBinding("POSITION", 0), _RenderDevice->CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal)));
    __geom->AddVertexBuffer(BufferBinding("TEXCOORD", 0), _RenderDevice->CreateVoidVertexBuffer(vertices.data(), vertices.size(), 12, sizeof(DirectX::VertexPositionTextureNormal)));

	// Font texture
	std::shared_ptr<Texture> texture = _RenderDevice->CreateTexture();
	texture->LoadTextureCustom(imageWidth, imageHeight, image);

	delete[] image;

	FT_Done_Face(face);
	FT_Done_FreeType(ftLibrary);

	//

	std::shared_ptr<CFontMesh> font = std::make_shared<CFontMesh>(texture, __geom, charWidth, charHeight);

	Log::Info("FontsManager[%s]: Font loaded. Size [%d].", f->Path_Name().c_str(), fontSize);

	return font;
}


/*
std::shared_ptr<CFontMesh> FontsManager::CreateAction(cstring _nameAndSize)
{
	uint32_t _delimIndex = static_cast<uint32>(_nameAndSize.find_last_of("__"));
	if (_delimIndex == -1)
	{
		Log::Error("FontsManager[%s]: Incorrect font nameAndSize.", _nameAndSize.c_str());
		return nullptr;
	}

	std::string fontFileName = _nameAndSize.substr(0, _delimIndex - 1);
	uint32 fontSize = Utils::ToType<uint32>(_nameAndSize.substr(_delimIndex + 1));

	std::shared_ptr<IFile> f = GetManager<IFilesManager>()->Open(fontFileName);
	if (f == nullptr)
	{
		Log::Fatal("FontsManager[%s]: Error while loading font.", f->Path_Name().c_str());
		return nullptr;
	}


	std::vector<uint32> charWidth;
	charWidth.reserve(CFontMesh::NUM_CHARS);
	uint32 charHeight = 0;

	FT_Library ftLibrary;
	FT_Init_FreeType(&ftLibrary);

	FT_Face face;
	if (FT_New_Memory_Face(ftLibrary, f->getData(), f->getSize(), 0, &face) != 0)
	{
		Log::Error("FontsManager[%s]: Error while loading font. Could not load font file.", f->Path_Name().c_str());

		// Unload
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
	uint32_t charIndex;

	for (uint32 ch = 0; ch < CFontMesh::NUM_CHARS; ++ch)
	{
		// Look up the character in the font file.
		charIndex = FT_Get_Char_Index(face, ch);

		// RenderPhase the current glyph.
		FT_Load_Glyph(face, charIndex, FT_LOAD_RENDER);

		charWidth.push_back(face->glyph->metrics.horiAdvance >> 6);
		// If the line is full go to the next line
		if (charWidth[ch] > lineSpace)
		{
			lineSpace = imageWidth;
			++lines;
		}
		lineSpace -= charWidth[ch];

		maxAscent = std::max(face->glyph->bitmap_top, maxAscent);
		maxDescent = std::max((int)face->glyph->bitmap.rows - (int)face->glyph->bitmap_top, maxDescent);
	}

	charHeight = maxAscent + maxDescent;

	uint32_t neededHeight = (maxAscent + maxDescent) * lines;

	// Get the first power of two in which it will fit
	while (imageHeight < neededHeight)
	{
		imageHeight <<= 1;
	}

	// Step 3: Generation of the actual texture //

	uint32* image = new uint32[imageHeight * imageWidth];
	memset(image, 0x00, imageHeight * imageWidth * sizeof(uint32));

	// These are the cameraPosition at which to draw the next glyph
	uint32_t x = 0;
	uint32_t y = maxAscent;


	DirectX::VertexCollection vertices;
	DirectX::IndexCollection indices;
	uint32 indicesCntr = 0;

	float xOffset = 0.0f;

	for (uint32 ch = 0; ch < CFontMesh::NUM_CHARS; ++ch)
	{
		//Log::Warn("Char [%c] %d", char(ch), ch);
		uint32_t charIndex = FT_Get_Char_Index(face, ch);

		FT_Load_Glyph(face, charIndex, FT_LOAD_DEFAULT);
		FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

		if (charWidth[ch] > imageWidth - x)
		{
			x = 0;
			y += (maxAscent + maxDescent);
		}

		float texX1 = (float)(x) / (float)(imageWidth);
		float texX2 = (float)(x + charWidth[ch]) / (float)(imageWidth);
		float texY1 = (float)(y - maxAscent) / (float)(imageHeight);
		float texY2 = texY1 + (float)(charHeight) / (float)(imageHeight);

		vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(charWidth[ch],  charHeight,  0.0f),  DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(texX2, texY2)));
		vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(0.0f,           charHeight,  0.0f),  DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(texX1, texY2)));
		vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(0.0f,           0.0f,        0.0f),  DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(texX1, texY1)));

		vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(0.0f,           0.0f,        0.0f),  DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(texX1, texY1)));
		vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(charWidth[ch],  charHeight,  0.0f),  DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(texX2, texY2)));
		vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(charWidth[ch],  0.0f,        0.0f),  DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(texX2, texY1)));

		for (uint32 row = 0; row < face->glyph->bitmap.rows; ++row)
		{
			for (uint32 pixel = 0; pixel < face->glyph->bitmap.width; ++pixel)
			{
				uint8* p = (uint8*) &(image[(x + face->glyph->bitmap_left + pixel) + (y - face->glyph->bitmap_top + row) * imageWidth]);
				p[0] = (face->glyph->bitmap.buffer[pixel + row * face->glyph->bitmap.pitch]);
				p[1] = (face->glyph->bitmap.buffer[pixel + row * face->glyph->bitmap.pitch]);
				p[2] = (face->glyph->bitmap.buffer[pixel + row * face->glyph->bitmap.pitch]);
				p[3] = (face->glyph->bitmap.buffer[pixel + row * face->glyph->bitmap.pitch]);
			}
		}

		x += charWidth[ch];
	}


	std::shared_ptr<IMesh> __geom = _RenderDevice->CreateMesh();

	std::shared_ptr<IBuffer> __vb = _RenderDevice->CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	__geom->SetVertexBuffer(__vb);

	// Font texture
	std::shared_ptr<Texture> texture = _RenderDevice->CreateTexture();
	texture->LoadTextureCustom(imageWidth, imageHeight, image);

	delete[] image;

	FT_Done_Face(face);
	FT_Done_FreeType(ftLibrary);

	//

	std::shared_ptr<CFontMesh> font = std::make_shared<CFontMesh>(texture, __geom, charWidth, charHeight);

	Log::Info("FontsManager[%s]: Font loaded. Size [%d].", f->Path_Name().c_str(), fontSize);

	return font;
}
*/

bool FontsManager::DeleteAction(cstring name)
{
	return true;
}

