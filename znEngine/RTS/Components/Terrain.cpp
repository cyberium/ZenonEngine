#include "stdafx.h"

// General
#include "Terrain.h"


namespace
{
	const uint32 C_UnitSize = 1;

	uint8 GetHeight(const std::shared_ptr<IImage>& Image, int32 X, int32 Y)
	{
		if (X < 0)
			X = 0;

		if (Y < 0)
			Y = 0;

		if (X >= Image->GetWidth())
			X = Image->GetWidth() - 1;

		if (Y >= Image->GetHeight())
			Y = Image->GetHeight() - 1;

		const uint8* data = (const uint8*)Image->GetData() + (Y * (Image->GetBitsPerPixel() / 8) * Image->GetWidth()) + X * (Image->GetBitsPerPixel() / 8);
		return data[0];

		//const uint8* line = Image->GetLine(Y);

		//size_t offset = (Image->GetBitsPerPixel() / 8) * X;

		//return (line + offset)[0];	
	}

}



/*



*/


CTerrain::CTerrain(const IBaseManager& BaseManager)
	: Object(BaseManager)
	, m_BaseManager(BaseManager)
{
	IRenderDevice& renderDevice = m_BaseManager.GetApplication().GetRenderDevice();

	//m_TerrainGeometry = renderDevice.GetObjectsFactory().CreateGeometry();

	std::shared_ptr<IImage> heightMapImage = m_BaseManager.GetManager<IImagesFactory>()->CreateImage("terrain32.png");
	m_Width = heightMapImage->GetWidth();
	m_Height = heightMapImage->GetHeight();

	m_HeightsArray = new float[heightMapImage->GetHeight() * heightMapImage->GetWidth()];
	for (uint32 Y = 0; Y < heightMapImage->GetHeight(); Y += 1)
	{
		for (uint32 X = 0; X < heightMapImage->GetWidth(); X += 1)
		{
			size_t heightsArrayOffset = Y * m_Width + X;

			size_t imageBBP = heightMapImage->GetBitsPerPixel() / 8;
			size_t imageArrayOffset = (Y * imageBBP * heightMapImage->GetWidth()) + (X * imageBBP);


			//m_HeightsArray[heightsArrayOffset] = Random::Range(0, 12);


			m_HeightsArray[heightsArrayOffset] = (float)((const uint8*)heightMapImage->GetData() + imageArrayOffset)[0];
		}
	}


	/*
	glm::vec3* tempVertexes = new glm::vec3[heightMapImage->GetHeight() * heightMapImage->GetWidth() * 4];
	glm::vec3* ttv = tempVertexes;

	uint32* tempIndices = new uint32[heightMapImage->GetHeight() * heightMapImage->GetWidth() * 6];
	uint32* tti = tempIndices;


	size_t indexCntr = 0;
	for (uint32 Y = 0; Y < heightMapImage->GetHeight() - 1; Y+=1)
	{
		for (uint32 X = 0; X < heightMapImage->GetWidth() - 1; X+=1)
		{
			// Get the (X, Y, Z) value for the bottom left vertex
			float x = X;
			float y = GetHeight(heightMapImage, X, Y);
			float z = Y;

			glm::vec3 v = glm::vec3(x, y, z);
			*ttv++ = v;

			// Get the (X, Y, Z) value for the top left vertex
			x = X;
			y = GetHeight(heightMapImage, X, Y + C_UnitSize);
			z = Y + C_UnitSize;
			v = glm::vec3(x, y, z);
			*ttv++ = v;

			// Get the (X, Y, Z) value for the top right vertex
			x = X + C_UnitSize;
			y = GetHeight(heightMapImage, X + C_UnitSize, Y + C_UnitSize);
			z = Y + C_UnitSize;
			v = glm::vec3(x, y, z);
			*ttv++ = v;

			// Get the (X, Y, Z) value for the bottom right vertex
			x = X + C_UnitSize;
			y = GetHeight(heightMapImage, X + C_UnitSize, Y);
			z = Y;
			v = glm::vec3(x, y, z);
			*ttv++ = v;


			uint32 ind = indexCntr + 0;
			*tti++ = ind;

			ind = indexCntr + 1;
			*tti++ = ind;

			ind = indexCntr + 2;
			*tti++ = ind;


			ind = indexCntr + 0;
			*tti++ = ind;

			ind = indexCntr + 2;
			*tti++ = ind;

			ind = indexCntr + 3;
			*tti++ = ind;

			indexCntr += 4;
		}
	}

	m_TerrainGeometry->AddVertexBuffer(BufferBinding("POSITION", 0), renderDevice.GetObjectsFactory().CreateVertexBuffer(tempVertexes, heightMapImage->GetHeight() * heightMapImage->GetWidth() * 4));
	m_TerrainGeometry->SetIndexBuffer(renderDevice.GetObjectsFactory().CreateIndexBuffer(tempIndices, heightMapImage->GetHeight() * heightMapImage->GetWidth() * 6));
	m_TerrainGeometry->SetBounds(BoundingBox(glm::vec3(-100000.0f), glm::vec3(100000.0f)));
	*/
}

CTerrain::~CTerrain()
{
	delete[] m_HeightsArray;
}



//
// ITerrain
//
const float * CTerrain::GetHeightsArray() const
{
	return m_HeightsArray;
}

size_t CTerrain::GetTerrainWidth() const
{
	return m_Width;
}

size_t CTerrain::GetTerrainHeight() const
{
	return m_Height;
}

std::shared_ptr<IGeometry> CTerrain::GetTerrainGeometry() const
{
	return nullptr;
}
